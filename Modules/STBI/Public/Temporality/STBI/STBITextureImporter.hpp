#ifndef TEMPORALITY_STBI_TEXTURE_IMPORTER_HPP
#define TEMPORALITY_STBI_TEXTURE_IMPORTER_HPP

#include <Temporality/STBI/STBIConfig.hpp>

#include <Temporality/Texture.hpp>
#include <Temporality/TextureImporter.hpp>

#include <Temporality/Math.hpp>
#include <memory>
#include <string>

namespace Temporality::STBI {

class TEMPORALITY_STBI_API STBITextureData : public TextureData
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

class TEMPORALITY_STBI_API STBITextureImporter : public TextureImporter
{
public:

    DISALLOW_COPY_AND_ASSIGN(STBITextureImporter)

    STBITextureImporter() = default;
    
    virtual std::unique_ptr<Temporality::TextureData> LoadFromFile(const std::string& filename);

    virtual std::unique_ptr<Temporality::TextureData> LoadFromMemory(const uint8_t * buffer, size_t length);
}; // class STBITextureImporter

} // namespace Temporality::STBI

#endif // TEMPORALITY_STBI_TEXTURE_IMPORTER_HPP