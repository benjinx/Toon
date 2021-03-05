#include <Toon/GLTF2/glTF2File.hpp>

#include <Toon/GraphicsDriver.hpp>
#include <Toon/Benchmark.hpp>
#include <Toon/Log.hpp>

#include <Base64.hpp>
#include <fstream>
#include <gsl/gsl>

namespace Toon::GLTF2 {

TOON_GLTF2_API
bool glTF2File::LoadFromFile(const string& filename)
{
    ToonBenchmarkStart();

    Filename = filename;
    BaseDir = GetDirname(Filename);

    std::ifstream file;
    file.open(Filename, std::ios::in | std::ios::binary);

    if (!file.is_open()) {
        ToonLogError("Failed to load glTF2, '%s'", Filename);
        return false;
    }

    const auto& ext = GetExtension(Filename);
    
    if (ext == "glb") {
        // Header
        uint32_t magic;
        file.read(reinterpret_cast<char *>(&magic), sizeof(magic));
        if (magic != MAGIC) {
            ToonLogError("Invalid binary glTF2 file header");
            return false;
        }

        uint32_t version;
        file.read(reinterpret_cast<char *>(&version), sizeof(version));
        if (version != 2) {
            ToonLogError("Invalid binary glTF2 container version, %u", version);
            return false;
        }

        uint32_t length;
        file.read(reinterpret_cast<char *>(&length), sizeof(length));

        // JSON Chunk

        uint32_t jsonChunkLength;
        file.read(reinterpret_cast<char *>(&jsonChunkLength), sizeof(jsonChunkLength));

        ChunkType jsonChunkType;
        file.read(reinterpret_cast<char *>(&jsonChunkType), sizeof(jsonChunkType));

        if (jsonChunkType != ChunkType::JSON) {
            ToonLogError("Invalid binary glTF2 file. The first chunk must be JSON, found '%08x'", jsonChunkType);
            return false;
        }

        std::vector<char> jsonChunk(jsonChunkLength + 1);
        file.read(jsonChunk.data(), jsonChunkLength);
        jsonChunk.back() = '\0';

        JSON = json::parse(jsonChunk.data());

        // Buffer Chunk (Optional)

        if (file.tellg() < length) {
            uint32_t bufferChunkLength;
            file.read(reinterpret_cast<char *>(&bufferChunkLength), sizeof(bufferChunkLength));

            ChunkType bufferChunkType;
            file.read(reinterpret_cast<char *>(&bufferChunkType), sizeof(bufferChunkType));

            if (bufferChunkType != ChunkType::BIN) {
                ToonLogError("Invalid binary glTF2 file. The second chunk must be BIN, found '%08x'", bufferChunkType);
                return false;
            }

            Buffers.push_back(std::vector<uint8_t>(bufferChunkLength));
            file.read(reinterpret_cast<char *>(Buffers.back().data()), bufferChunkLength);
        }
    }
    else {
        file >> JSON;

        if (!LoadBuffers()) {
            return false;
        }
    }

    const auto VERSION_PATH = json::json_pointer("/asset/version");

    if (JSON.contains(VERSION_PATH)) {
        auto version = JSON[VERSION_PATH].get<string>();
        ToonLogVerbose("glTF2 Version: %s", version);
    }

    const auto GENERATOR_PATH = json::json_pointer("/asset/generator");

    if (JSON.contains(GENERATOR_PATH)) {
        auto generator = JSON[GENERATOR_PATH].get<string>();
        ToonLogVerbose("glTF2 Generator: %s", generator);
    }

    if (!LoadBufferViews()) {
        return false;
    }

    if (!LoadAccessors()) {
        return false;
    }

    if (!LoadImages()) {
        return false;
    }

    if (!LoadSamplers()) {
        return false;
    }

    if (!LoadTextures()) {
        return false;
    }

    if (!LoadCameras()) {
        return false;
    }

    if (!LoadMeshes()) {
        return false;
    }

    ToonLogLoad("glTF2 file '%s'", Filename);

    ToonBenchmarkEnd();
    return true;
}

TOON_GLTF2_API
bool glTF2File::LoadBuffers()
{
    const auto BUFFERS_PATH = json::json_pointer("/buffers");

    if (JSON.contains(BUFFERS_PATH)) {
        for (const auto& object : JSON[BUFFERS_PATH]) {
            const auto& byteLength = object.value<size_t>("byteLength", 0);
            const auto& uri = object.value("uri", "");

            if (uri.rfind("data:", 0) == 0) {
                size_t pivot = uri.find(',');
                Buffers.push_back(macaron::Base64::Decode(uri.substr(pivot + 1)));
            }
            else {
                ToonLogVerbose("Loading glTF2 buffer from '%s'", uri);

                string path = BaseDir + TOON_PATH_SLASH + uri;
                std::ifstream file(path, std::ios::in | std::ios::binary);
                if (!file.is_open()) {
                    ToonLogError("Failed to read glTF2 buffer from '%s'", uri);
                    return false;
                }

                Buffers.push_back(std::vector<uint8_t>(byteLength));
                file.read(reinterpret_cast<char *>(Buffers.back().data()), byteLength);

                ToonLogLoad("glTF2 buffer '%s'", path);
            }
        }
    }
    else {
        return false;
    }

    ToonLogVerbose("Loaded %d Buffer(s)", Buffers.size());

    return true;
}

TOON_GLTF2_API
bool glTF2File::LoadBufferViews()
{
    const auto BUFFER_VIEWS_PATH = json::json_pointer("/bufferViews");

    if (JSON.contains(BUFFER_VIEWS_PATH)) {
        for (const auto& object : JSON[BUFFER_VIEWS_PATH]) {
            BufferViews.push_back(BufferViewData{
                object.value<int>("buffer", -1),
                object.value<size_t>("byteLength", 0),
                object.value<size_t>("byteOffset", 0),
                object.value<size_t>("byteStride", 0),
                object.value<GLenum>("target", GL_INVALID_ENUM),
            });
        }
    }
    else {
        return false;
    }

    ToonLogVerbose("Loaded %d Buffer View(s)", BufferViews.size());

    return true;
}

TOON_GLTF2_API
bool glTF2File::LoadAccessors()
{
    const auto ACCESSORS_PATH = json::json_pointer("/accessors");

    if (JSON.contains(ACCESSORS_PATH)) {
        for (const auto& object : JSON[ACCESSORS_PATH]) {
            Accessors.push_back(AccessorData{
                object.value<int>("bufferView", -1),
                object.value<size_t>("byteOffset", 0),
                object.value<size_t>("count", 0),
                object.value<string>("type", ""),
                object.value<GLenum>("componentType", GL_INVALID_ENUM),
                object.value<bool>("normalized", false),
            });
        }
    }
    else {
        return false;
    }

    ToonLogVerbose("Loaded %d Accessor(s)", Accessors.size());

    return true;
}

TOON_GLTF2_API
bool glTF2File::LoadImages()
{
    const auto IMAGES_PATH = json::json_pointer("/images");

    if (JSON.contains(IMAGES_PATH)) {
        for (const auto& object : JSON[IMAGES_PATH]) {
            Images.push_back(ImageData{
                object.value("uri", ""),
                object.value("bufferView", -1),
                object.value("mimeType", ""),
            });
        }
    }

    ToonLogVerbose("Loaded %d Image(s)", Images.size());

    return true;
}

TOON_GLTF2_API
bool glTF2File::LoadSamplers()
{
    const auto SAMPLERS_PATH = json::json_pointer("/samplers");

    if (JSON.contains(SAMPLERS_PATH)) {
        for (const auto& object : JSON[SAMPLERS_PATH]) {
            Toon::Texture::Options options;
            options.WrapS = GetWrapType(object.value("wrapS", 0));
            options.WrapT = GetWrapType(object.value("wrapT", 0));
            options.MagFilter = GetFilterType(object.value("magFilter", 0));
            options.MinFilter = GetFilterType(object.value("minFilter", 0));
            options.GenerateMipmaps = false;
            Samplers.push_back(options);
        }
    }

    ToonLogVerbose("Loaded %d Samplers(s)", Samplers.size());

    return true;
}

TOON_GLTF2_API
bool glTF2File::LoadTextures()
{
    const auto TEXTURES_PATH = json::json_pointer("/textures");

    GraphicsDriver * gfx = Toon::GetGraphicsDriver();
    if (!gfx) {
        ToonLogError("Unable to load textures from a glTF2 file without a graphics driver");
        return false;
    }

    if (JSON.contains(TEXTURES_PATH)) {
        for (const auto& object : JSON[TEXTURES_PATH]) {
            int sampler = object.value("sampler", -1);
            int source = object.value("source", -1);

            if (source < 0 || source >= Images.size()) {
                ToonLogError("Invalid glTF2 sourceimage: %d\n", source);
                return false;
            }

            Toon::Texture::Options opts;
            if (sampler >= 0 && sampler < Samplers.size()) {
                opts = Samplers[sampler];
            }

            const auto& image = Images[source];
            if (image.bufferView >= 0) {
                ToonLogVerbose("Loading glTF2 texture from bufferView");

                const auto& bufferView = BufferViews[image.bufferView];
                const auto& buffer = Buffers[bufferView.buffer];

                if (bufferView.byteStride > 0) {
                    ToonLogError("Unsupported glTF2 bufferView, byteStride != 0");
                    return false;
                }

                const uint8_t * ptr = buffer.data() + bufferView.byteOffset;

                auto texture = LoadTextureFromMemory(ptr, bufferView.byteLength, opts);
                if (!texture) {
                    return false;
                }

                Textures.push_back(texture);
            }
            else if (image.uri.rfind("data:", 0) == 0) {
                ToonLogVerbose("Loading glTF2 texture from base64 encoded uri");
                
                size_t pivot = image.uri.find(',');
                const auto& data = macaron::Base64::Decode(image.uri.substr(pivot + 1));

                auto texture = LoadTextureFromMemory(data.data(), data.size(), opts);
                if (!texture) {
                    return false;
                }

                Textures.push_back(texture);
            }
            else {
                string path = BaseDir + TOON_PATH_SLASH + image.uri;
                ToonLogVerbose("Loading glTF2 texture from file: '%s'", path);
                
                auto texture = LoadTextureFromFile(path, false, opts);
                if (!texture) {
                    return false;
                }

                Textures.push_back(texture);
            }
        }
    }

    ToonLogVerbose("Loaded %d Texture(s)", Textures.size());

    return true;
}

TOON_GLTF2_API
bool glTF2File::LoadMaterials()
{
    const auto MATERIALS_PATH = json::json_pointer("/materials");

    // if (JSON.contains(MATERIALS_PATH)) {
    //     for (const auto& object : JSON[MATERIALS_PATH]) {

            // extensions
            /// KHR_materials_transmission
            /// KHR_materials_clearcoat
            /// KHR_materials_pbrSpecularGlossiness
            /// KHR_materials_variants
            /// KHR_techniques_webgl

            // each texture
            /// [scale] = 1
            /// index
            /// [texCoord] = 0

            // name

            // [alphaMode] ?
            /// OPAQUE, MASK, BLEND

            // [doubleSided]

            // [normal]
            // [occlusionTexture]
            // [emissiveTexture]
            // [emissiveFactor]

            // [pbrMetallicRoughness]
            /// [baseColorFactor]
            /// [baseColorTexture]
            /// [metallicFactor]
            /// [roughnessFactor]
            /// [metallicRoughnessTexture]

    //     }
    // }

    ToonLogVerbose("Loaded %d Material(s)", Materials.size());

    return true;
}

TOON_GLTF2_API
bool glTF2File::LoadCameras()
{
    const auto CAMERAS_PATH = json::json_pointer("/cameras");

    if (JSON.contains(CAMERAS_PATH)) {
        for (const auto& object : JSON[CAMERAS_PATH]) {
            Cameras.push_back(CameraData{
                object.value<string>("name", ""),
                object.value<string>("type", ""),
            });
            auto& camera = Cameras.back();

            if (camera.type == "perspective") {
                auto it = object.find("perspective");
                if (it == object.end()) {
                    ToonLogError("Invalid glTF2 perspective camera");
                    return false;
                }

                const auto& perspective = it.value();
                camera.aspectRatio = perspective.value<float>("aspectRatio", 0.0f);
                camera.yfov = perspective.value<float>("yfov", 0.0f);
                camera.zfar = perspective.value<float>("zfar", 0.0f);
                camera.znear = perspective.value<float>("znear", 0.0f);
            }
            else if (camera.type == "orthographic") {
                auto it = object.find("orthographic");
                if (it == object.end()) {
                    ToonLogError("Invalid glTF2 orthographic camera");
                    return false;
                }

                const auto& orthographic = it.value();
                camera.xmag = orthographic.value("xmag", 0.0f);
                camera.ymag = orthographic.value("ymag", 0.0f);
                camera.zfar = orthographic.value("zfar", 0.0f);
                camera.znear = orthographic.value("znear", 0.0f);
            }
            else {
                ToonLogWarn("Unknown glTF2 camera type: '%s'", camera.type);
            }
        }
    }

    ToonLogVerbose("Loaded %d Camera(s)", Cameras.size());

    return true;
}

// gltf2 Node
    // camera (Camera Component?)
    // children[]
    // skin (needs mesh)
    // matrix (rip apart into TRS? Error out instead)
    // mesh (Mesh Component)
    // translate
    // rotation
    // scale
    // weights (needs mesh)

// gltf2 

// Entity -> glft2 Node
// Mesh -> gltf2 Primitive
// MeshComponent -> gltf2 Mesh

TOON_GLTF2_API
bool glTF2File::LoadMeshes()
{

    auto gfx = GetGraphicsDriver();

    const auto MESHES_PATH = json::json_pointer("/meshes");

    if (JSON.contains(MESHES_PATH)) {
        for (const auto& object : JSON[MESHES_PATH]) {

            auto it = object.find("primitives");
            if (it != object.end()) {
                const auto& primitives = it.value();
                if (!primitives.is_array()) {
                    ToonLogError("Invalid glTF2 mesh, missing primitives array");
                    return false;
                }

                std::vector<std::unique_ptr<PrimitiveData>> PrimitiveDataList;

                for (const auto& primitive : primitives) {
                    int indicesIndex = primitive.value("indices", -1);
                    
    
                    auto data = std::unique_ptr<GLTF2PrimitiveData>(new GLTF2PrimitiveData());

                    if (indicesIndex >= 0) {
                        const auto& accessor = Accessors[indicesIndex];
                        const auto& bufferView = BufferViews[accessor.bufferView];
                        auto& buffer = Buffers[bufferView.buffer];

                        if (bufferView.byteStride != 0) {
                            ToonLogFatal("index ByteStride != 0");
                        }

                        if (accessor.componentType == GL_UNSIGNED_INT) {
                            auto indices = gsl::span<uint32_t>(
                                reinterpret_cast<uint32_t *>(buffer.data() + bufferView.byteOffset),
                                accessor.count
                            );

                            data->IndexList.assign(indices.begin(), indices.end());
                        }
                        else if (accessor.componentType == GL_UNSIGNED_SHORT) {
                            auto indices = gsl::span<uint16_t>(
                                reinterpret_cast<uint16_t *>(buffer.data() + bufferView.byteOffset),
                                accessor.count
                            );

                            data->IndexList.assign(indices.begin(), indices.end());
                        }
                        else {
                            ToonLogFatal("Unsupported index format type: %04X, %s", accessor.componentType, accessor.type);
                        }


                    }

                    it = primitive.find("attributes");
                    if (it != primitive.end()) {
                        const auto& attributes = it.value();
                        if (!attributes.is_object()) {
                            ToonLogError("Invalid glTF2 primitive, missing attributes dictionary");
                            return false;
                        }
                        
                        int positionIndex = attributes.value("POSITION", -1);

                        if (positionIndex >= 0) {
                            const auto& accessor = Accessors[positionIndex];
                            const auto& bufferView = BufferViews[accessor.bufferView];
                            auto& buffer = Buffers[bufferView.buffer];

                            if (bufferView.byteStride != 0) {
                                ToonLogFatal("position byteStride != 0");
                            }

                            if (accessor.componentType != GL_FLOAT) {
                                ToonLogFatal("Unsupported format type: %04X, %s", accessor.componentType, accessor.type);
                            }
                            
                            data->VertexList.resize(accessor.count);

                            if (accessor.type == "VEC3") {
                                gsl::span<glm::vec3> posData = gsl::span<glm::vec3>(
                                    reinterpret_cast<glm::vec3 *>(buffer.data() + bufferView.byteOffset),
                                    accessor.count
                                );

                                for (size_t i = 0; i < posData.size(); ++i) {
                                    data->VertexList[i].Position = glm::vec4(posData[i], 1.0f);
                                    ToonLogWarn("posData: %s", glm::to_string(posData[i]));
                                }
                            }
                            else if (accessor.type == "VEC4") {
                                gsl::span<glm::vec4> posData = gsl::span<glm::vec4>(
                                    reinterpret_cast<glm::vec4 *>(buffer.data() + bufferView.byteOffset),
                                    accessor.count
                                );

                                for (size_t i = 0; i < posData.size(); ++i) {
                                    data->VertexList[i].Position = posData[i];
                                }
                            }
                            else {
                                ToonLogFatal("Unsupported accessor type: %s", accessor.type);
                            }
                        }
                        
                        PrimitiveDataList.push_back(std::move(data));
                    }
                }
                
                Meshes.push_back(gfx->CreateMesh());
                Meshes.back()->Load(PrimitiveDataList);

                if (auto it = object.find("attributes"); it != object.end()) {
                    // const auto& attributes = it.value();

                }




                // attributes
                // material
                // mode
                // targets (animation)
            }

            // Meshes.push_back()
        }
    }

    return true;
}

} // namespace Toon::GLTF2