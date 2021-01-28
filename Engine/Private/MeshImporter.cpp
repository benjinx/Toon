#include <Toon/MeshImporter.hpp>

#include <unordered_map>

namespace Toon {

static std::unordered_map<std::string, std::unique_ptr<MeshImporter>> _MeshImporters;

static std::vector<MeshImporter*> _MeshImporterList;

void updateMeshImporterList()
{
    _MeshImporterList.clear();
    for (const auto& it : _MeshImporters)
    {
        _MeshImporterList.push_back(it.second.get());
    }
}

TOON_ENGINE_API
void AddMeshImporter(const std::string& id, std::unique_ptr<MeshImporter> importer)
{
    _MeshImporters[id] = std::move(importer);
    updateMeshImporterList();
}

TOON_ENGINE_API
void RemoveMeshImporter(const std::string& id)
{
    auto it = _MeshImporters.find(id);
    if (it != _MeshImporters.end())
    {
        _MeshImporters.erase(it);
    }

    updateMeshImporterList();
}

TOON_ENGINE_API
const std::vector<MeshImporter*>& GetAllMeshImporters()
{
    return _MeshImporterList;
}

} // namespace Toon