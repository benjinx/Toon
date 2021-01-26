#include <Temporality/Scene/MeshComponent.hpp>
#include <Temporality/MeshImporter.hpp>
#include <Temporality/GraphicsDriver.hpp>
#include <Temporality/Scene/Entity.hpp>
#include <Temporality/Log.hpp>

namespace Temporality {

bool MeshComponent::LoadFromFile(const std::string& filename)
{
    auto gfx = GetGraphicsDriver();
    if (!gfx) {
        LogError("Unable to load mesh '%s', no graphics driver found", filename);
        return false;
    }

    const auto& importers = GetAllMeshImporters();
    for (const auto& importer : importers) {
        auto meshes = importer->LoadFromFile(filename);
        if (!meshes.empty()) {
            for (auto& meshData : meshes) {
                auto mesh = gfx->CreateMesh();
                /*if (mesh->Load(meshData.get())) {
                    _mMeshes.push_back(std::move(mesh));
                }*/
            }
            return true;
        }
    }

    LogError("Unable to load mesh '%s'", filename);
    return false;
}

void MeshComponent::AddMesh(std::shared_ptr<Mesh> && mesh)
{
    _mMeshes.push_back(std::move(mesh));
}

void MeshComponent::Render(RenderContext * ctx)
{
    auto gfx = GetGraphicsDriver();
    auto transformData = ctx->GetTransformData();

    transformData->Model = GetEntity()->GetWorldTransform();
    transformData->UpdateMVP();

    //gfx->SetShaderData("TemporalityTransformData", sizeof(TransformData), transformData);

    for (auto& mesh : _mMeshes) {
        mesh->Render(ctx);
    }
}

} // namespace Temporality