#ifndef TOON_TINYOBJ_MESH_IMPORTER_HPP
#define TOON_TINYOBJ_MESH_IMPORTER_HPP

#include <Toon/TinyOBJ/TinyOBJConfig.hpp>
#include <Toon/MeshImporter.hpp>

#include <vector>

namespace Toon::TinyOBJ {

class TOON_TINYOBJ_API TinyOBJMeshImporter : public MeshImporter
{
public:

    DISALLOW_COPY_AND_ASSIGN(TinyOBJMeshImporter)

    TinyOBJMeshImporter() = default;

    std::vector<std::unique_ptr<PrimitiveData>> LoadFromFile(const std::string& filename) override;
    
}; // class TinyOBJMeshImporter

} // namespace Toon::TinyOBJ

#endif // TOON_TINYOBJ_MESH_IMPORTER_HPP