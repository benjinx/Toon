#ifndef TOON_TEXTURE_HPP
#define TOON_TEXTURE_HPP

#include <Toon/Config.hpp>
#include <Toon/TextureImporter.hpp>

namespace Toon {

enum class TextureWrapType
{
    Repeat,
    MirroredRepeat,
    ClampToEdge,
    ClampToBorder,

}; // enum TextureWrapType

enum class TextureFilterType
{
    Nearest,
    NearestMipmapNearest,
    NearestMipmapLinear,
    Linear,
    LinearMipmapNearest,
    LinearMipmapLinear,

}; // enum TextureFilterType

class TOON_ENGINE_API Texture
{
public:

    struct Options
    {
        Options() { }

        TextureWrapType WrapS = TextureWrapType::Repeat;
        TextureWrapType WrapT = TextureWrapType::Repeat;

        TextureFilterType MagFilter = TextureFilterType::Nearest;
        TextureFilterType MinFilter = TextureFilterType::Nearest;

        bool GenerateMipmaps = false;

    }; // struct Options

    DISALLOW_COPY_AND_ASSIGN(Texture)
    
    Texture() = default;

    virtual ~Texture() = default;

    virtual bool Load(const std::unique_ptr<TextureData>& data, Options opts = Options()) = 0;
    
}; // class Texture

TOON_ENGINE_API
std::shared_ptr<Texture> LoadTextureFromFile(const string& filename, bool useAssetPath = true, Texture::Options opts = Texture::Options());

TOON_ENGINE_API
std::shared_ptr<Texture> LoadFromMemory(const uint8_t* buffer, size_t length, Texture::Options opts = Texture::Options());

inline string TextureWrapTypeToString(TextureWrapType textureWrapType)
{
    switch(textureWrapType) {
        case TextureWrapType::Repeat:
            return "Repeat";
        case TextureWrapType::MirroredRepeat:
            return "MirroredRepeat";
        case TextureWrapType::ClampToEdge:
            return "ClampToEdge";
        case TextureWrapType::ClampToBorder:
            return "ClampToBorder";
    }

    return "Unknown";
}

inline string TextureWrapTypeToString(TextureFilterType textureFilterType)
{
    switch (textureFilterType) {
        case TextureFilterType::Nearest:
            return "Nearest";
        case TextureFilterType::NearestMipmapNearest:
            return "NearestMipmapNearest";
        case TextureFilterType::NearestMipmapLinear:
            return "NearestMipmapLinear";
        case TextureFilterType::Linear:
            return "Linear";
        case TextureFilterType::LinearMipmapNearest:
            return "LinearMipmapNearest";
        case TextureFilterType::LinearMipmapLinear:
            return "LinearMipmapLinear";
    }

    return "Unknown";
}

} // namespace Toon

#endif // TOON_TEXTURE_HPP