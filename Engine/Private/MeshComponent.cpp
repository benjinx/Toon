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
    auto transformData = ctx->GetTransformData();

    transformData->Model = GetEntity()->GetWorldTransform();
    transformData->UpdateMVP();
    
    Buffer * buffer = GetGraphicsDriver()->GetConstantBuffer(0);
    buffer->WriteTo(0, sizeof(TransformData), reinterpret_cast<uint8_t *>(ctx->GetTransformData()));

    _mesh->Render(ctx);
}

} // namespace Toon