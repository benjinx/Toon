#ifndef TOON_MESH_IMPORTER_HPP
#define TOON_MESH_IMPORTER_HPP

#include <Toon/Config.hpp>
#include <Toon/Mesh.hpp>

#include <Toon/String.hpp>
#include <memory>
#include <vector>

namespace Toon {

class TOON_ENGINE_API MeshImporter
{
public:
    DISALLOW_COPY_AND_ASSIGN(MeshImporter)

    MeshImporter() = default;

    virtual ~MeshImporter() = default;

    virtual std::vector<std::unique_ptr<PrimitiveData>> LoadFromFile(const string& filename, bool useAssetPath = true) = 0;
    
}; // class MeshImporter

TOON_ENGINE_API
void AddMeshImporter(const string& id, std::unique_ptr<MeshImporter> importer);

TOON_ENGINE_API
void RemoveMeshImporter(const string& id);

TOON_ENGINE_API
const std::vector<MeshImporter*>& GetAllMeshImporters();

} // namespace Toon

#endif // TOON_MESH_IMPORTER_HPP