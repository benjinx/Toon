#include <Toon/Mesh.hpp>
#include <Toon/MeshImporter.hpp>
#include <Toon/GraphicsDriver.hpp>
#include <Toon/Log.hpp>
#include <Toon/Benchmark.hpp>

namespace Toon {

TOON_ENGINE_API
bool Mesh::Initialize()
{
    bool result = true;

    GraphicsDriver* gfx = GetGraphicsDriver();

    _pipeline = gfx->GetDefaultPipeline();

    _shaderTransformBuffer = gfx->CreateBuffer();
    result = _shaderTransformBuffer->Initialize(
        sizeof(ShaderTransform),
        nullptr,
        BufferUsage::Constant,
        MemoryUsage::UploadOften
    );

    return result;
}

TOON_ENGINE_API
void Mesh::Terminate()
{
    if (_shaderTransformBuffer) {
        _shaderTransformBuffer->Terminate();
        _shaderTransformBuffer.reset();
    }
}

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
std::shared_ptr<Mesh> LoadMeshFromFile(const string& filename)
{
    ToonBenchmarkStart();
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

    ToonLogError("Failed to load mesh '%s'", filename);
    ToonBenchmarkEnd();
    return nullptr;
}

TOON_ENGINE_API
void Mesh::Render(RenderContext * ctx)
{
    if (_shaderTransformBuffer) {
        uint8_t * data = reinterpret_cast<uint8_t *>(ctx->GetShaderTransform());
        _shaderTransformBuffer->WriteTo(0, sizeof(ShaderTransform), data);
    }
}

} // namespace Toon