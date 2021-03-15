#ifndef TOON_GLTF2_GLTF2_FILE_HPP
#define TOON_GLTF2_GLTF2_FILE_HPP

#include <Toon/GLTF2/GLTF2Config.hpp>

#include <Toon/Math.hpp>
#include <Toon/JSON.hpp>
#include <Toon/Texture.hpp>
#include <Toon/TextureImporter.hpp>
#include <Toon/Material.hpp>
#include <Toon/PrimitiveData.hpp>

#include "OpenGLStub.hpp"

#include <cstdint>
#include <iterator>

namespace Toon::GLTF2 {

TOON_GLTF2_API
inline TextureWrapType GetWrapType(const GLenum& type)
{
    switch (type) {
    case GL_REPEAT:
        return TextureWrapType::Repeat;
    case GL_MIRRORED_REPEAT:
        return TextureWrapType::MirroredRepeat;
    case GL_CLAMP_TO_EDGE:
        return TextureWrapType::ClampToEdge;
    case GL_CLAMP_TO_BORDER:
        return TextureWrapType::ClampToBorder;
    }

    return TextureWrapType::Repeat;
}

TOON_GLTF2_API
inline TextureFilterType GetFilterType(const GLenum& type)
{
    switch (type) {
    case GL_NEAREST:
        return TextureFilterType::Nearest;
    case GL_NEAREST_MIPMAP_NEAREST:
        return TextureFilterType::NearestMipmapNearest;
    case GL_NEAREST_MIPMAP_LINEAR:
        return TextureFilterType::NearestMipmapLinear;
    case GL_LINEAR:
        return TextureFilterType::Linear;
    case GL_LINEAR_MIPMAP_NEAREST:
        return TextureFilterType::LinearMipmapNearest;
    case GL_LINEAR_MIPMAP_LINEAR:
        return TextureFilterType::LinearMipmapLinear;
    }

    return TextureFilterType::Nearest;
}

struct BufferViewData
{
    int buffer;
    size_t byteLength;
    size_t byteOffset;
    size_t byteStride;
    GLenum target;
};

struct AccessorData
{
    int bufferView;
    size_t byteOffset;
    size_t count;
    string type;
    GLenum componentType;
    bool normalized;
    // TODO: min, max
};

struct ImageData
{
    string uri;
    
    int bufferView;
    string mimeType;
};

struct CameraData
{
    string name;
    string type;

    // Perspective
    float aspectRatio;
    float yfov;

    // Orthographic
    float xmag;
    float ymag;

    // Both
    float zfar;
    float znear;
};

class TOON_GLTF2_API glTF2File
{
public:

    DISALLOW_COPY_AND_ASSIGN(glTF2File);

    glTF2File() = default;

    virtual ~glTF2File() = default;

    bool LoadFromFile(const string& filename);

    const uint32_t MAGIC = 0x46546C67; // glTF

    enum class ChunkType : uint32_t
    {
        JSON    = 0x4E4F534A, // JSON
        BIN     = 0x004E4942, // BIN
    };

    string Filename;

    string BaseDir;

    json JSON;

    std::vector<std::vector<uint8_t>> Buffers;

    std::vector<BufferViewData> BufferViews;

    std::vector<AccessorData> Accessors;

    std::vector<ImageData> Images;

    std::vector<Texture::Options> Samplers;

    std::vector<std::shared_ptr<Texture>> Textures;

    std::vector<std::shared_ptr<Material>> Materials;

    std::vector<CameraData> Cameras;
    
    // std::vector<>

    bool IsValidTexture(int index, int texCoord);

    bool LoadBuffers();

    bool LoadBufferViews();

    bool LoadAccessors();

    bool LoadImages();
    
    bool LoadSamplers();

    bool LoadTextures();

    bool LoadMaterials();
    
    bool LoadCameras();

    std::vector<std::unique_ptr<PrimitiveData>> LoadMesh();

};

class AccessorIterator
{
public:

    AccessorIterator(glTF2File * file, int index)
    {
        if (index < 0) {
            _ptr = nullptr;
            return;
        }

        _accessor = &file->Accessors[index];
        _bufferView = &file->BufferViews[_accessor->bufferView];
        _buffer = &file->Buffers[_bufferView->buffer];

        _ptr = _buffer->data() + _accessor->byteOffset + _bufferView->byteOffset;

        if (_accessor->type == "SCALAR") {
            _componentCount = 1;
        }
        else if (_accessor->type == "VEC2") {
            _componentCount = 2;
        }
        else if (_accessor->type == "VEC3") {
            _componentCount = 3;
        }
        else if (_accessor->type == "VEC4") {
            _componentCount = 4;
        }

        _stride = _bufferView->byteStride;
        if (_stride == 0) {
            switch (_accessor->componentType) {
            case GL_UNSIGNED_BYTE:
                _stride = _componentCount * sizeof(uint8_t);
                break;
            case GL_UNSIGNED_SHORT:
                _stride = _componentCount * sizeof(uint16_t);
                break;
            case GL_UNSIGNED_INT:
                _stride = _componentCount * sizeof(uint32_t);
                break;
            case GL_FLOAT:
                _stride = _componentCount * sizeof(float);
                break;
            }
        }
    }

    inline float getFloat(size_t index = 0, float def = 0.0f)
    {
        if (_ptr && index < _componentCount) {
            switch (_accessor->componentType) {
            case GL_UNSIGNED_BYTE:
                return NormalizeInteger(getAs<uint8_t>(index));
            case GL_UNSIGNED_SHORT:
                return NormalizeInteger(getAs<uint16_t>(index));
            case GL_UNSIGNED_INT:
                return NormalizeInteger(getAs<uint32_t>(index));
            case GL_FLOAT:
                return getAs<float>(index);
            }
        }

        return def;
    }

    inline uint32_t getInteger(size_t index = 0, unsigned def = 0)
    {
        if (_ptr && index < _componentCount) {
            switch (_accessor->componentType) {
            case GL_UNSIGNED_BYTE:
                return getAs<uint8_t>(index);
            case GL_UNSIGNED_SHORT:
                return getAs<uint16_t>(index);
            case GL_UNSIGNED_INT:
                return getAs<uint32_t>(index);
            case GL_FLOAT:
                return getAs<float>(index);
            }
        }

        return def;
    }

    inline glm::vec2 getVec2(glm::vec2 def = glm::vec2()) {
        return glm::vec2(
            getFloat(0, def[0]),
            getFloat(1, def[1])
        );
    }

    inline glm::vec4 getVec4(glm::vec4 def = glm::vec4()) {
        return glm::vec4(
            getFloat(0, def[0]),
            getFloat(1, def[1]),
            getFloat(2, def[2]),
            getFloat(3, def[3])
        );
    }

    inline glm::uvec4 getUVec4(glm::uvec4 def = glm::uvec4()) {
        return glm::vec4(
            getInteger(0, def[0]),
            getInteger(1, def[1]),
            getInteger(2, def[2]),
            getInteger(3, def[3])
        );
    }

    AccessorIterator& operator++() {
        if (_ptr) {
            _ptr += _stride;
            if (_ptr >= _buffer->data() + _buffer->size()) {
                _ptr = nullptr;
            }
        }

        return *this;
    }

private: 

    template <typename T>
    inline T getAs(size_t index) {
        return *reinterpret_cast<T *>(_ptr + (sizeof(T) * index));
    }

    uint8_t * _ptr;

    size_t _stride;

    size_t _componentCount;
    
    AccessorData * _accessor = nullptr;

    BufferViewData * _bufferView = nullptr;

    std::vector<uint8_t> * _buffer = nullptr;

}; // class AccessorIterator

} // namespace Toon::GLTF2

#endif // TOON_GLTF2_GLTF2_FILE_HPP