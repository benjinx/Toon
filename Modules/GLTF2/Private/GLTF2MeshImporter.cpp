#include <Toon/GLTF2/GLTF2MeshImporter.hpp>

#include <Toon/Log.hpp>
#include <Toon/Benchmark.hpp>
#include <Toon/Util.hpp>

namespace Toon::GLTF2 {

TOON_GLTF2_API
std::vector<std::unique_ptr<PrimitiveData>> GLTF2MeshImporter::LoadFromFile(const string& filename, bool useAssetPath /*= true*/)
{
    ToonBenchmarkStart();

    const string& dir = GetDirname(filename);
    std::vector<std::unique_ptr<PrimitiveData>> meshes;

    ToonLogInfo("%s %s", dir, filename);

    ToonBenchmarkEnd();
    return { };
}

} // namespace Toon::GLTF2