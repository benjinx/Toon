#ifndef TOON_STBI_TEXTURE_IMPORTER_HPP
#define TOON_STBI_TEXTURE_IMPORTER_HPP

#include <Toon/STBI/STBIConfig.hpp>

#include <Toon/TextureImporter.hpp>

namespace Toon::STBI {

class TOON_STBI_API STBITextureImporter : public TextureImporter
{
public:

    DISALLOW_COPY_AND_ASSIGN(STBITextureImporter)

    STBITextureImporter() = default;
    
    virtual std::unique_ptr<Toon::TextureData> LoadFromFile(const std::string& filename, bool useAssetPath = true) override;

    virtual std::unique_ptr<Toon::TextureData> LoadFromMemory(const uint8_t * buffer, size_t length) override;
}; // class STBITextureImporter

} // namespace Toon::STBI

#endif // TOON_STBI_TEXTURE_IMPORTER_HPP