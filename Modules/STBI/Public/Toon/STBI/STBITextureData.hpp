#ifndef TOON_STBI_TEXTURE_DATA_HPP
#define TOON_STBI_TEXTURE_DATA_HPP

#include <Toon/STBI/STBIConfig.hpp>
#include <Toon/TextureData.hpp>

namespace Toon::STBI {

class TOON_STBI_API STBITextureData : public TextureData
{
public:
    
    uint8_t * Data = nullptr;

    glm::uvec2 Size;

    virtual ~STBITextureData() {
        stbi_image_free(Data);
    }

    uint8_t * GetData() const override {
        return Data;
    }

    glm::uvec2 GetSize() const override {
        return Size;
    }

    TextureDataType GetDataType() const override {
        return TextureDataType::UnsignedByte;
    }
    
}; // class STBITextureData

} // namespace Toon::STBI

#endif // TOON_STBI_TEXTURE_DATA_HPP