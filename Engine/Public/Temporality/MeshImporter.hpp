#ifndef TEMPORALITY_MESH_IMPORTER_HPP
#define TEMPORALITY_MESH_IMPORTER_HPP

#include <Temporality/Config.hpp>
#include <Temporality/Mesh.hpp>
#include <Temporality/PrimitiveData.hpp>

#include <memory>
#include <vector>

namespace Temporality {

class TEMPORALITY_ENGINE_API MeshImporter
{
public:
    DISALLOW_COPY_AND_ASSIGN(MeshImporter)

    MeshImporter() = default;

    virtual ~MeshImporter() = default;

    virtual std::vector<std::unique_ptr<PrimitiveData>> LoadFromFile(const std::string& filename) = 0;
}; // class MeshImporter

TEMPORALITY_ENGINE_API
void AddMeshImporter(const std::string& id, std::unique_ptr<MeshImporter> importer);

TEMPORALITY_ENGINE_API
void RemoveMeshImporter(const std::string& id);

TEMPORALITY_ENGINE_API
const std::vector<MeshImporter*>& GetAllMeshImporters();

} // namespace Temporality

#endif // TEMPORALITY_MESH_IMPORTER_HPP