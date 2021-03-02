#include "Toon/AxisComponent.hpp"

#include <Toon/GraphicsDriver.hpp>
#include <Toon/Entity.hpp>

namespace Toon {

// TOON_ENGINE_API
// AxisPrimitiveData AxisComponent::_PrimitiveData;

TOON_ENGINE_API
AxisComponent::AxisComponent()
{
    auto gfx = GetGraphicsDriver();

    _shader = gfx->CreateShader();
    _shader->LoadFromFiles({
        "Toon/axis.vert",
        "Toon/axis.frag",
    });

    _mesh = gfx->CreateMesh();
    // _mesh->Load(&_PrimitiveData);
    // TODO: Make GL_LINES
}

TOON_ENGINE_API
void AxisComponent::Render(RenderContext * ctx)
{
    auto gfx = GetGraphicsDriver();
    auto transformData = ctx->GetShaderTransform();

    transformData->Model = GetEntity()->GetWorldTransform();
    transformData->UpdateMVP();
    
    // gfx->SetShaderData("ToonTransformData", sizeof(TransformData), transformData);

    // _shader->Bind();
    // _mesh->Render();
}

} // namespace Toon