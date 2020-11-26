#include <Temporality/Graphics/TextureImporter.hpp>

namespace Temporality {

static std::unordered_map<std::string, std::unique_ptr<TextureImporter>> _TextureImporters;

static std::vector<TextureImporter *> _TextureImporterList;

void updateTextureImporterList()
{
    _TextureImporterList.clear();
    for (const auto& it : _TextureImporters) {
        _TextureImporterList.push_back(it.second.get());
    }
}

TEMPORALITY_ENGINE_API
void AddTextureImporter(const std::string& id, std::unique_ptr<TextureImporter> importer)
{
    _TextureImporters[id] = std::move(importer);
    updateTextureImporterList();
}

TEMPORALITY_ENGINE_API
void RemoveTextureImporter(const std::string& id)
{
    auto it = _TextureImporters.find(id);
    if (it != _TextureImporters.end()) {
        _TextureImporters.erase(it);
    }
    
    updateTextureImporterList();
}

TEMPORALITY_ENGINE_API
const std::vector<TextureImporter *>& GetAllTextureImporters()
{
    return _TextureImporterList;
}

} // namespace Temporality