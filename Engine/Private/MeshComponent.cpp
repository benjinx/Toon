#include <Toon/MeshComponent.hpp>
#include <Toon/MeshImporter.hpp>
#include <Toon/GraphicsDriver.hpp>
#include <Toon/Entity.hpp>
#include <Toon/Log.hpp>

namespace Toon {

TOON_ENGINE_API
void MeshComponent::SetMesh(std::shared_ptr<Mesh> mesh)
{
    _mesh = mesh;
}

TOON_ENGINE_API
void MeshComponent::Render(RenderContext * ctx)
{
    auto gfx = GetGraphicsDriver();
    auto transformData = ctx->GetShaderTransform();

    transformData->Model = GetEntity()->GetWorldTransform();
    transformData->UpdateMVP();

    _mesh->Render(ctx);
}

} // namespace Toon