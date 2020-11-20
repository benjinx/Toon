#include <Temporality/Graphics/TextureImporter.hpp>

namespace Temporality {

std::unordered_map<std::string, std::unique_ptr<TextureImporter>> TextureImporter::_msTextureImporters;

std::vector<TextureImporter *> TextureImporter::_msTextureImporterList;

void TextureImporter::UpdateList()
{
    _msTextureImporterList.clear();
    for (const auto& it : _msTextureImporters) {
        _msTextureImporterList.push_back(it.second.get());
    }
}

void TextureImporter::Register(const std::string& id, std::unique_ptr<TextureImporter> importer)
{
    _msTextureImporters[id] = std::move(importer);
    UpdateList();
}

void TextureImporter::Unregister(const std::string& id)
{
    auto it = _msTextureImporters.find(id);
    if (it != _msTextureImporters.end()) {
        _msTextureImporters.erase(it);
    }

    UpdateList();
}

const std::vector<TextureImporter*>& TextureImporter::GetAll()
{
    return _msTextureImporterList;
}






}