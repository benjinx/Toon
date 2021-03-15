#include <Toon/Material.hpp>
#include <Toon/GraphicsDriver.hpp>

namespace Toon {

void Material::Initialize()
{
    auto gfx = GetGraphicsDriver();

    _shaderMaterialBuffer = gfx->CreateBuffer();
    bool result = _shaderMaterialBuffer->Initialize(
        sizeof(ShaderMaterial),
        nullptr,
        BufferUsage::Constant,
        MemoryUsage::UploadOften
    );

    // TODO:
    (void)(result);

    UpdateBuffer();
}

void Material::Terminate()
{
    if (_shaderMaterialBuffer) {
        _shaderMaterialBuffer->Terminate();
        _shaderMaterialBuffer.reset();
    }
}

void Material::UpdateBuffer()
{
    if (_shaderMaterialBuffer) {
        uint8_t * data = reinterpret_cast<uint8_t *>(&_shaderMaterial);
        _shaderMaterialBuffer->WriteTo(0, sizeof(ShaderMaterial), data);
    }
}

} // namespace Toon