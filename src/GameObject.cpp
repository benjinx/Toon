#include "GameObject.hpp"

#include <App.hpp>
#include <Camera.hpp>
#include <Log.hpp>
#include <Material.hpp>
#include <Model.hpp>
#include <Shader.hpp>
#include <Texture.hpp>

#include <imgui/imgui.h>
#include <nlohmann/json.hpp>
#include <stb/stb_image.h>

#define TINYGLTF_NO_STB_IMAGE_WRITE
#include <tinygltf/tiny_gltf.h>

GameObject::GameObject()
{
}

GameObject::GameObject(glm::vec3 position)
{
    SetPosition(position);
}

GameObject::~GameObject()
{
    _mChildren.clear();
    delete _mModel;
}

void GameObject::Update(const float dt)
{
}

void GameObject::Render()
{
    if (_mShader != nullptr && _mModel != nullptr)
    {
        _mModel->Render(_mShader, GetWorldTransform());
    }
}

void GameObject::SetShader(Shader* shader)
{
    _mShader = shader;
}

bool GameObject::Load(std::string filename)
{
    std::string ext = Utils::GetExtension(filename);
    bool binary = (ext == "glb");

    std::string fullPath;
    const auto& paths = Utils::GetResourcePaths();

    tinygltf::Model model;
    tinygltf::TinyGLTF loader;
    std::string err, warn;

    bool loaded = false;
    for (auto& p : paths)
    {
        fullPath = p + filename;

        LogVerbose("Checking %s\n", fullPath);
        if (binary)
        {
            loaded = loader.LoadBinaryFromFile(&model, &err, &warn, fullPath);
        }
        else
        {
            loaded = loader.LoadASCIIFromFile(&model, &err, &warn, fullPath);
        }

        if (loaded) break;
    }

    _mLoadedModel = &model;

    if (!loaded)
    {
        LogError("Failed to load model, '%s'\n", filename);
        return false;
    }

    LogLoad("Loading Mesh from %s\n", fullPath);
    LogVerbose("Model Generator %s\n", model.asset.generator);

    bool loadedTextures = processTextures();

    bool loadedMaterials = processMaterials();

    const auto& scene = _mLoadedModel->scenes[_mLoadedModel->defaultScene];
    for (int id : scene.nodes) {
        auto& node = _mLoadedModel->nodes[id];
        LogVerbose("Loading node(GameObject): %s\n", node.name);
        LogLoad("Loading GameObject: %s\n", node.name);
        AddChild(processNode(node).release());
    }

    glBindVertexArray(0);

    for (auto vbo : _mVBOS)
    {
        glDeleteBuffers(1, &vbo);
    }

    _mTextures.clear();
    _mMaterials.clear();

    return true;
}

bool GameObject::processTextures()
{
    for (auto& texture : _mLoadedModel->textures)
    {
        tinygltf::Image& image = _mLoadedModel->images[texture.source];
        LogVerbose("Loading Texture from buffer (%d, %d, %d)\n", image.width, image.height, image.component);
        _mTextures.push_back(std::make_shared<Texture>(image.image.data(), glm::ivec2(image.width, image.height), image.component));
    }

    if (_mTextures.empty())
    {
        LogWarn("No Textures found, adding default\n");
        _mTextures.push_back(std::make_shared<Texture>());
        return false;
    }

    return true;
}

bool GameObject::processMaterials()
{
    for (auto& material : _mLoadedModel->materials)
    {
        auto mat = std::make_shared<Material>();

        auto& vals = material.values;
        auto it = vals.end();

        auto& addVals = material.additionalValues;
        auto addIt = addVals.end();

        it = vals.find("baseColorFactor");
        if (it != vals.end() && !it->second.number_array.empty())
        {
            const auto& c = it->second.ColorFactor();
            mat->SetDiffuse(glm::make_vec4(it->second.ColorFactor().data()));
        }

        it = vals.find("baseColorTexture");
        if (it != vals.end())
        {
            mat->SetDiffuseMap(_mTextures[it->second.TextureIndex()]);
        }

        it = vals.find("metallicFactor");
        if (it != vals.end())
        {
            mat->SetMetallic((float)it->second.Factor());
        }

        it = vals.find("roughnessFactor");
        if (it != vals.end())
        {
            mat->SetRoughness((float)it->second.Factor());
        }

        it = vals.find("metallicRoughnessTexture");
        if (it != vals.end())
        {
            mat->SetMetallicRoughnessMap(_mTextures[it->second.TextureIndex()]);
        }

        addIt = addVals.find("normalTexture");
        if (addIt != addVals.end())
        {
            mat->SetNormalMap(_mTextures[addIt->second.TextureIndex()]);
            mat->SetNormalScale((float)addIt->second.Factor());
        }

        addIt = addVals.find("emissiveFactor");
        if (it != vals.end() && !it->second.number_array.empty())
        {
            const auto& c = it->second.ColorFactor();
            mat->SetEmissive(glm::make_vec3(addIt->second.ColorFactor().data()));
        }

        addIt = addVals.find("emissiveTexture");
        if (addIt != addVals.end())
        {
            mat->SetEmissiveMap(_mTextures[addIt->second.TextureIndex()]);
        }

        addIt = addVals.find("occlusionTexture");
        if (addIt != addVals.end())
        {
            mat->SetOcclusionMap(_mTextures[addIt->second.TextureIndex()]);
            mat->SetOcclusionStrength((float)addIt->second.Factor());
        }

        for (const auto& val : material.values)
        {
            LogVerbose("Material value %s\n", val.first);
        }

        for (const auto& val : material.additionalValues)
        {
            LogVerbose("Material additional value %s\n", val.first);
        }

        _mMaterials.push_back(std::move(mat));
    }

    if (_mMaterials.empty())
    {
        LogWarn("No Materials found, adding default\n");
        _mMaterials.push_back(std::make_shared<Material>());
        return false;
    }

    return true;
}

std::unique_ptr<GameObject> GameObject::processNode(tinygltf::Node& node)
{
    GameObject* gobj = nullptr;

    // Check if camera
    if (node.camera >= 0)
    {
        // Set all camera values here for if it's a camera
        Camera* camera = new Camera();
        gobj = camera;

        // Load scene cameras
        for (auto modelcamera : _mLoadedModel->cameras)
        {
            LogVerbose("%i Camera(s) found.", node.camera);
            LogInfo("Loading Camera: %s\n", modelcamera.name);
            LogInfo("Type: %s\n", modelcamera.type);

            if (modelcamera.type == "perspective")
            {
                // Loaded aspect ratio was coming up as 0.000? hm
                //camera->SetAspect(modelcamera.perspective.aspectRatio);
                camera->SetFOVY((float)modelcamera.perspective.yfov);
                camera->SetClip(glm::vec2(modelcamera.perspective.znear, modelcamera.perspective.zfar));
                camera->SetPosition(glm::vec3(0.0f, 0.0f, 5.0f));

                if (modelcamera.name == "Main Camera")
                    App::Inst()->SetCurrentCamera(camera);
            }
            else if (modelcamera.type == "orthographic")
            {
                // This hasn't been tested but i'm pretty sure this is right.
                camera->SetViewportSize(glm::vec2(modelcamera.orthographic.xmag, modelcamera.orthographic.ymag));
                camera->SetClip(glm::vec2(modelcamera.orthographic.znear, modelcamera.orthographic.zfar));

                if (modelcamera.name == "Main Camera")
                    App::Inst()->SetCurrentCamera(camera);
            }
        }
    }
    else
    {
        gobj = new GameObject();
    }

    // Check if we have a mesh
    if (node.mesh >= 0)
    {
        auto& mesh = _mLoadedModel->meshes[node.mesh];

        std::vector<Mesh*> meshes;
        meshes.clear();
        for (size_t pInd = 0; pInd < mesh.primitives.size(); ++pInd)
        {
            auto& primitive = mesh.primitives[pInd];
            auto& indexAccessor = _mLoadedModel->accessors[primitive.indices];

            GLuint vao;
            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);

            {
                auto& bufferView = _mLoadedModel->bufferViews[indexAccessor.bufferView];
                auto& buffer = _mLoadedModel->buffers[bufferView.buffer];

                GLuint vbo;
                glGenBuffers(1, &vbo);
                _mVBOS.push_back(vbo);

                glBindBuffer(bufferView.target, vbo);
                glBufferData(bufferView.target, bufferView.byteLength, buffer.data.data() + bufferView.byteOffset, GL_STATIC_DRAW);
            }

            for (auto& attrib : primitive.attributes)
            {
                auto& accessor = _mLoadedModel->accessors[attrib.second];
                auto& bufferView = _mLoadedModel->bufferViews[accessor.bufferView];
                auto& buffer = _mLoadedModel->buffers[bufferView.buffer];
                int byteStride = accessor.ByteStride(bufferView);

                LogVerbose("Attribute %s\n", attrib.first);

                GLuint vbo;
                glGenBuffers(1, &vbo);
                _mVBOS.push_back(vbo);

                glBindBuffer(bufferView.target, vbo);
                glBufferData(bufferView.target, bufferView.byteLength, buffer.data.data() + bufferView.byteOffset, GL_STATIC_DRAW);

                GLint size = (accessor.type == TINYGLTF_TYPE_SCALAR ? 1 : accessor.type);

                GLint vaa = -1;
                if (attrib.first.compare("POSITION") == 0)
                {
                    vaa = AttributeID::POSITION;
                }

                if (attrib.first.compare("NORMAL") == 0)
                {
                    vaa = AttributeID::NORMAL;
                }

                if (attrib.first.compare("TEXCOORD_0") == 0)
                {
                    vaa = AttributeID::TEXCOORD;
                }

                if (attrib.first.compare("TANGENT") == 0)
                {
                    vaa = AttributeID::TANGENT;
                }

                if (vaa > -1)
                {
                    glEnableVertexAttribArray(vaa);
                    glVertexAttribPointer(vaa, size, accessor.componentType, accessor.normalized ? GL_TRUE : GL_FALSE, byteStride, (void*)accessor.byteOffset);
                }
                else
                {
                    LogWarn("Ignoring attribute %s\n", attrib.first);
                }
            }

            meshes.push_back(new Mesh(vao,
                (GLenum)primitive.mode,
                (GLsizei)indexAccessor.count,
                (GLenum)indexAccessor.componentType,
                (GLsizei)indexAccessor.byteOffset,
                _mMaterials[(primitive.material < 0 ? 0 : primitive.material)]));
        }

        gobj->SetModel(new Model(meshes));

    }

    // Check position and assign it
    if (node.translation.size() == 3)
    {
        gobj->SetPosition(glm::make_vec3(node.translation.data()));
    }

    // Check rotation and assign it
    if (node.rotation.size() == 4)
    {
        glm::vec4 data = glm::make_vec4(node.rotation.data());
        gobj->SetRotation(glm::quat(data[3], data[0], data[1], data[2]));
    }

    // Check scale and assign it
    if (node.scale.size() == 3)
    {
        gobj->SetScale(glm::make_vec3(node.scale.data()));
    }

    // Get the name
    gobj->SetName(node.name.c_str());

    // SET TEMP SHADER
    gobj->SetShader(App::Inst()->GetShader("defaultLighting"));

    // Process all children
    for (int id : node.children) {
        auto& child = _mLoadedModel->nodes[id];
        gobj->AddChild(processNode(child).release());
    }

    // TRS, check if has mesh.
    return std::unique_ptr<GameObject>(gobj);
}