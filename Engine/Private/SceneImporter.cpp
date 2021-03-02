#include <Toon/SceneImporter.hpp>

#include <unordered_map>

namespace Toon {

static std::unordered_map<string, std::unique_ptr<SceneImporter>> _SceneImporters;

static std::vector<SceneImporter *> _SceneImporterList;

void updateSceneImporterList()
{
    _SceneImporterList.clear();
    for (const auto& it : _SceneImporters) {
        _SceneImporterList.push_back(it.second.get());
    }
}

TOON_ENGINE_API
void AddSceneImporter(const string& id, std::unique_ptr<SceneImporter> importer)
{
    _SceneImporters[id] = std::move(importer);
    updateSceneImporterList();
}

TOON_ENGINE_API
void RemoveSceneImporter(const string& id)
{
    auto it = _SceneImporters.find(id);
    if (it != _SceneImporters.end()) {
        _SceneImporters.erase(it);
    }

    updateSceneImporterList();
}

TOON_ENGINE_API
const std::vector<SceneImporter *>& GetAllSceneImporters()
{
    return _SceneImporterList;
}

} // namespace Toon