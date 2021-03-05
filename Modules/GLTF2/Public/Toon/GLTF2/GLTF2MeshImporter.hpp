#ifndef TOON_GLTF2_MESH_IMPORTER_HPP
#define TOON_GLTF2_MESH_IMPORTER_HPP

#include <Toon/GLTF2/GLTF2Config.hpp>

#include <Toon/MeshImporter.hpp>
#include <Toon/GLTF2/GLTF2PrimitiveData.hpp>

#include <vector>

namespace Toon::GLTF2 {

class TOON_GLTF2_API GLTF2MeshImporter : public MeshImporter
{
public:

    DISALLOW_COPY_AND_ASSIGN(GLTF2MeshImporter)

    GLTF2MeshImporter() = default;

    std::vector<std::unique_ptr<PrimitiveData>> LoadFromFile(const string& filename, bool useAssetPath = true) override;

}; // class GLTF2MeshImporter

} // namespace Toon::GLTF2

#endif // TOON_GLTF2_MESH_IMPORTER_HPP