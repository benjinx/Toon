#include "Scene.hpp"

#include <App.hpp>
#include <Camera.hpp>
#include <Log.hpp>
#include <Material.hpp>
#include <Model.hpp>
#include <Shader.hpp>
#include <Texture.hpp>
#include <Utils.hpp>

#include <imgui/imgui.h>

#include <nlohmann/json.hpp>

#include <stb/stb_image.h>

#define TINYGLTF_NO_STB_IMAGE_WRITE
#include <tinygltf/tiny_gltf.h>

bool Scene::_sShowAxis = false;

Scene::~Scene() {
    delete _mSceneAxis;

    for (auto& go : _mGameObjects) {
        delete go.second;
    }
    _mGameObjects.clear();
}

void Scene::Start()
{
    _mSceneAxis = new Axis();
}

void Scene::Update(float dt)
{
    // Use default shader
    App* app = App::Inst();
    Shader* defaultLighting = app->GetShader("defaultLighting");

    defaultLighting->Use();
    glm::vec3 defaultLightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    defaultLighting->SetVec3("lightColor", defaultLightColor);

    glm::vec4 defaultLightPosition = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    defaultLighting->SetVec4("lightPosition", defaultLightPosition);

    for (auto& gobj : _mGameObjects)
    {
        // Update gobjs
        gobj.second->Update(dt);
    }
}

void Scene::Render()
{
    for (auto& gameObject : _mGameObjects)
    {
        // Render gobjs
        gameObject.second->Render();
    }

    if (_sShowAxis)
    {
        for (auto& gameObject : _mGameObjects)
        {
            //gameObject.second->DrawAxis();
            // Render the gobjs axis
            _mSceneAxis->Render(gameObject.second->GetWorldTransform());

            // Render the scenes axis
            _mSceneAxis->Render(_mSceneTransform);
        }
    }
}

bool Scene::Load(std::string filename)
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

    if (!loaded)
    {
        LogError("Failed to load model, '%s'\n", filename);
        return false;
    }

    LogLoad("Loading Mesh from %s\n", fullPath);
    LogVerbose("Model Generator %s\n", model.asset.generator);

    std::vector<std::shared_ptr<Texture>> textures;
    for (auto& texture : model.textures)
    {
        tinygltf::Image& image = model.images[texture.source];
        LogVerbose("Loading Texture from buffer (%d, %d, %d)", image.width, image.height, image.component);
        textures.push_back(std::make_shared<Texture>(image.image.data(), glm::ivec2(image.width, image.height), image.component));
    }

    std::vector<std::shared_ptr<Material>> materials;
    for (auto& material : model.materials)
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
            mat->SetDiffuseMap(textures[it->second.TextureIndex()]);
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
            mat->SetMetallicRoughnessMap(textures[it->second.TextureIndex()]);
        }

        addIt = addVals.find("normalTexture");
        if (addIt != addVals.end())
        {
            mat->SetNormalMap(textures[addIt->second.TextureIndex()]);
            mat->SetNormalScale(addIt->second.Factor());
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
            mat->SetEmissiveMap(textures[addIt->second.TextureIndex()]);
        }

        addIt = addVals.find("occlusionTexture");
        if (addIt != addVals.end())
        {
            mat->SetOcclusionMap(textures[addIt->second.TextureIndex()]);
            mat->SetOcclusionStrength(addIt->second.Factor());
        }

        for (const auto& val : material.values)
        {
            LogVerbose("Material value %s\n", val.first);
        }

        for (const auto& val : material.additionalValues)
        {
            LogVerbose("Material additional value %s\n", val.first);
        }

        materials.push_back(std::move(mat));
    }

    if (materials.empty())
    {
        LogWarn("No Materials found, adding default\n");
        materials.push_back(std::make_shared<Material>());
    }

    std::vector<GLuint> vbos;

    std::function<std::unique_ptr<GameObject>(tinygltf::Node&)> processNode;
    processNode = [&](tinygltf::Node& node) -> std::unique_ptr<GameObject> 
    {
        GameObject* gobj = nullptr;

        // Check if camera
        if (node.camera >= 0)
        {
            // Set all camera values here for if it's a camera
            Camera* camera = new Camera();
            gobj = camera;
        }
        else
        {
            gobj = new GameObject();
        }

        // Check if we have a mesh
        if (node.mesh >= 0)
        {
            auto& mesh = model.meshes[node.mesh];

            // std::vector<Mesh> meshes;
            _mMeshes.clear();
            for (size_t pInd = 0; pInd < mesh.primitives.size(); ++pInd)
            {
                auto& primitive = mesh.primitives[pInd];
                auto& indexAccessor = model.accessors[primitive.indices];

                GLuint vao;
                glGenVertexArrays(1, &vao);
                glBindVertexArray(vao);

                {
                    auto& bufferView = model.bufferViews[indexAccessor.bufferView];
                    auto& buffer = model.buffers[bufferView.buffer];

                    GLuint vbo;
                    glGenBuffers(1, &vbo);
                    vbos.push_back(vbo);

                    glBindBuffer(bufferView.target, vbo);
                    glBufferData(bufferView.target, bufferView.byteLength, buffer.data.data() + bufferView.byteOffset, GL_STATIC_DRAW);
                }

                for (auto& attrib : primitive.attributes)
                {
                    auto& accessor = model.accessors[attrib.second];
                    auto& bufferView = model.bufferViews[accessor.bufferView];
                    auto& buffer = model.buffers[bufferView.buffer];
                    int byteStride = accessor.ByteStride(bufferView);

                    LogVerbose("Attribute %s\n", attrib.first);

                    GLuint vbo;
                    glGenBuffers(1, &vbo);
                    vbos.push_back(vbo);

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

                _mMeshes.push_back( new Mesh(vao,
                                            (GLenum)primitive.mode,
                                            (GLsizei)indexAccessor.count,
                                            (GLenum)indexAccessor.componentType,
                                            (GLsizei)indexAccessor.byteOffset,
                                            materials[(primitive.material < 0 ? 0 : primitive.material)]));
            }

            gobj->SetModel(new Model(_mMeshes));

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

        // Get the name (TEMP)
        gobj->SetName(node.name.c_str());

        // SET TEMP SHADER
        gobj->SetShader(App::Inst()->GetShader("defaultLighting"));

        // Process all children
        for (int id : node.children) {
            auto& child = model.nodes[id];
            gobj->AddChild(processNode(child).release());
        }

        // TRS, check if has mesh.
        return std::unique_ptr<GameObject>(gobj);
    };

    const auto& scene = model.scenes[model.defaultScene];
    for (int id : scene.nodes) {
        auto& node = model.nodes[id];
        LogLoad("Loading node(GameObject): %s\n", node.name);
        AddGameObject(processNode(node));
    }

    glBindVertexArray(0);

    for (auto vbo : vbos)
    {
        glDeleteBuffers(1, &vbo);
    }

    return true;
}

void Scene::AddGameObject(std::unique_ptr<GameObject> gameObject)
{
    if (gameObject)
    {
        GameObject* tmp = gameObject.release();
        _mGameObjects.emplace(tmp->GetName(), tmp);
    }
}

void Scene::Options()
{
    ImGui::Checkbox("Show GameObject Axis", &_sShowAxis);
}