#ifndef TEMPORALITY_TINYOBJ_MESH_IMPORTER_HPP
#define TEMPORALITY_TINYOBJ_MESH_IMPORTER_HPP

#include <Temporality/TinyOBJ/TinyOBJConfig.hpp>
#include <Temporality/MeshImporter.hpp>

#include <vector>

namespace Temporality::TinyOBJ {

class TEMPORALITY_TINYOBJ_API TinyOBJMeshImporter : public MeshImporter
{
public:

    DISALLOW_COPY_AND_ASSIGN(TinyOBJMeshImporter)

    TinyOBJMeshImporter() = default;

    std::vector<std::unique_ptr<PrimitiveData>> LoadFromFile(const std::string& filename) override;
    
}; // class TinyOBJMeshImporter

} // namespace Temporality::TinyOBJ

#endif // TEMPORALITY_TINYOBJ_MESH_IMPORTER_HPP