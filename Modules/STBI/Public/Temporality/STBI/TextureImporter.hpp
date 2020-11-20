#ifndef TEMPORALITY_STBI_TEXTURE_IMPORTER_HPP
#define TEMPORALITY_STBI_TEXTURE_IMPORTER_HPP

#include <Temporality/STBI/Config.hpp>

#include <Temporality/Graphics/Texture.hpp>
#include <Temporality/Graphics/TextureImporter.hpp>

#include <Temporality/Math.hpp>
#include <memory>
#include <string>

namespace Temporality::STBI {

class TEMPORALITY_STBI_API TextureData : public Temporality::TextureData
{
public:

    DISALLOW_COPY_AND_ASSIGN(TextureData)

    TextureData() = default;

    uint8_t* Data = nullptr;

    glm::uvec2 Size = glm::uvec2(0);

    int Components = 0;

    virtual ~TextureData();

    uint8_t* GetData() const override {
        return Data;
    }

    glm::uvec2 GetSize() const override {
        return Size;
    }

    int GetComponents() const override {
        return Components;
    }
};

class TEMPORALITY_STBI_API TextureImporter : public Temporality::TextureImporter
{
public:

    DISALLOW_COPY_AND_ASSIGN(TextureImporter)

    TextureImporter() = default;
    
    virtual std::unique_ptr<Temporality::TextureData> LoadFromFile(const std::string& filename);

    virtual std::unique_ptr<Temporality::TextureData> LoadFromMemory(uint8_t * buffer, size_t length);
};

} // namespace Temporality::STBI

#endif // TEMPORALITY_STBI_TEXTURE_IMPORTER_HPP