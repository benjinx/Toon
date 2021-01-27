#include <Temporality/Scene/MeshComponent.hpp>
#include <Temporality/MeshImporter.hpp>
#include <Temporality/GraphicsDriver.hpp>
#include <Temporality/Scene/Entity.hpp>
#include <Temporality/Log.hpp>

namespace Temporality {

TEMPORALITY_ENGINE_API
void MeshComponent::SetMesh(std::shared_ptr<Mesh> mesh)
{
    _mesh = mesh;
}

TEMPORALITY_ENGINE_API
void MeshComponent::Render(RenderContext * ctx)
{
    auto gfx = GetGraphicsDriver();
    auto transformData = ctx->GetTransformData();

    transformData->Model = GetEntity()->GetWorldTransform();
    transformData->UpdateMVP();
    
    Buffer * buffer = GetGraphicsDriver()->GetConstantBuffer(0);
    buffer->WriteTo(0, sizeof(TransformData), reinterpret_cast<uint8_t *>(ctx->GetTransformData()));

    _mesh->Render(ctx);
}

} // namespace Temporality