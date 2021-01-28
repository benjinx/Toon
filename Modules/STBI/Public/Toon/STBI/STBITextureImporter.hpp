#ifndef TOON_STBI_TEXTURE_IMPORTER_HPP
#define TOON_STBI_TEXTURE_IMPORTER_HPP

#include <Toon/STBI/STBIConfig.hpp>

#include <Toon/Texture.hpp>
#include <Toon/TextureImporter.hpp>

#include <Toon/Math.hpp>
#include <memory>
#include <string>

namespace Toon::STBI {

class TOON_STBI_API STBITextureData : public TextureData
{
public:

    DISALLOW_COPY_AND_ASSIGN(STBITextureData)

    STBITextureData() = default;

    uint8_t* Data = nullptr;

    glm::uvec2 Size = glm::uvec2(0);

    int Components = 0;

    virtual ~STBITextureData();

    uint8_t* GetData() const override {
        return Data;
    }

    glm::uvec2 GetSize() const override {
        return Size;
    }

    int GetComponents() const override {
        return Components;
    }
}; // class STBITextureData

class TOON_STBI_API STBITextureImporter : public TextureImporter
{
public:

    DISALLOW_COPY_AND_ASSIGN(STBITextureImporter)

    STBITextureImporter() = default;
    
    virtual std::unique_ptr<Toon::TextureData> LoadFromFile(const std::string& filename);

    virtual std::unique_ptr<Toon::TextureData> LoadFromMemory(const uint8_t * buffer, size_t length);
}; // class STBITextureImporter

} // namespace Toon::STBI

#endif // TOON_STBI_TEXTURE_IMPORTER_HPP