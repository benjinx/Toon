#include <Toon/Mesh.hpp>
#include <Toon/MeshImporter.hpp>
#include <Toon/GraphicsDriver.hpp>
#include <Toon/Log.hpp>

namespace Toon {

TOON_ENGINE_API
bool Mesh::Load(const std::vector<std::unique_ptr<PrimitiveData>>& data)
{
    GraphicsDriver * gfx = GetGraphicsDriver();
    
    for (const auto& primitiveData : data) {
        auto primitive = gfx->CreatePrimitive();
        if (!primitive || !primitive->Load(primitiveData)) {
            return false;
        }
        _primitiveList.push_back(std::move(primitive));
    }

    return true;
}

TOON_ENGINE_API
std::shared_ptr<Mesh> LoadMeshFromFile(const std::string& filename)
{
    GraphicsDriver * gfx = GetGraphicsDriver();

    const auto& importers = GetAllMeshImporters();
    for (const auto& importer : importers) {
        const auto& primitiveList = importer->LoadFromFile(filename);
        if (primitiveList.empty()) {
            continue;
        }

        auto mesh = gfx->CreateMesh();
        if (!mesh->Load(primitiveList)) {
            break;
        }

        return mesh;
    }

    LogError("Failed to load mesh '%s'", filename);
    return nullptr;
}

} // namespace Toon