#ifndef TOON_TEXTURE_DATA_HPP
#define TOON_TEXTURE_DATA_HPP

#include <Toon/Config.hpp>
#include <Toon/Math.hpp>
#include <Toon/String.hpp>

namespace Toon {

enum class TextureDataType
{
    UnsignedByte,
    Byte,
    UnsignedShort,
    Short,
    UnsignedInt,
    Int,
}; // enum TextureDataType

class TOON_ENGINE_API TextureData
{
public:

    DISALLOW_COPY_AND_ASSIGN(TextureData)

    TextureData() = default;

    virtual ~TextureData() = default;

    // Pointer to pixel data Buffer
    virtual uint8_t* GetData() const = 0;

    // Width and Height in pixels
    virtual glm::uvec2 GetSize() const = 0;

    // Number of components (R, RG, RGB, RGBA)
    virtual int GetComponents() const = 0;

    // Format of the data in the buffer
    virtual TextureDataType GetDataType() const = 0;
}; // class TextureData

TOON_ENGINE_API
inline string TextureDataTypeToString(TextureDataType textureDataType)
{
    switch (textureDataType) {
        case TextureDataType::UnsignedByte:
            return "UnsignedByte";
        case TextureDataType::Byte:
            return "Byte";
        case TextureDataType::UnsignedShort:
            return "UnsignedShort";
        case TextureDataType::Short:
            return "Short";
        case TextureDataType::UnsignedInt:
            return "UnsignedInt";
        case TextureDataType::Int:
            return "Int";
    }

    return "Unknown";
}

} // namespace Toon

#endif // TOON_TEXTURE_DATA_HPP