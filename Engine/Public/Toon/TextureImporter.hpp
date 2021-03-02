#ifndef TOON_TEXTURE_IMPORTER_HPP
#define TOON_TEXTURE_IMPORTER_HPP

#include <Toon/Config.hpp>
#include <Toon/Math.hpp>
#include <Toon/TextureData.hpp>
#include <Toon/String.hpp>

#include <memory>
#include <unordered_map>
#include <vector>

namespace Toon {

class TOON_ENGINE_API TextureImporter
{
public:

    DISALLOW_COPY_AND_ASSIGN(TextureImporter)

    TextureImporter() = default;

    virtual ~TextureImporter() = default;

    virtual std::unique_ptr<TextureData> LoadFromFile(const string& filename, bool useAssetPath = true) = 0;

    virtual std::unique_ptr<TextureData> LoadFromMemory(const uint8_t * buffer, size_t length) = 0;

}; // class TextureImporter

TOON_ENGINE_API
void AddTextureImporter(const string& id, std::unique_ptr<TextureImporter> importer);

TOON_ENGINE_API
void RemoveTextureImporter(const string& id);

TOON_ENGINE_API
const std::vector<TextureImporter *>& GetAllTextureImporters();

} // namespace Toon

#endif // TOON_TEXTURE_IMPORTER_HPP