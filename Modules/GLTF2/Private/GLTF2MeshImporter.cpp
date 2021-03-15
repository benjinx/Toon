#include <Toon/GLTF2/GLTF2MeshImporter.hpp>

#include <Toon/Log.hpp>
#include <Toon/Benchmark.hpp>
#include <Toon/Util.hpp>
#include <Toon/Path.hpp>
#include <Toon/GLTF2/GLTF2PrimitiveData.hpp>

#include "glTF2File.hpp"

namespace Toon::GLTF2 {

TOON_GLTF2_API
std::vector<std::unique_ptr<PrimitiveData>> GLTF2MeshImporter::LoadFromFile(const string& filename, bool useAssetPath /*= true*/)
{
    ToonBenchmarkStart();

    glTF2File file;
    bool result = false;

    if (useAssetPath) {
        const auto& getAssetPathList = GetAssetPathList();

        for (const auto& path : getAssetPathList) {
            Path fullPath = path / "Models" / filename;
            ToonLogVerbose("Checking '%s'", fullPath);

            result = file.LoadFromFile(fullPath);

            if (result) {
                break;
            }
        }
    }
    else {
        result = file.LoadFromFile(filename);
    }

    if (!result) {
        return { };
    }

    auto primitiveList = file.LoadMesh();

    ToonBenchmarkEnd();

    return primitiveList;
}

} // namespace Toon::GLTF2