#include <Toon/GraphicsDriver.hpp>

namespace Toon {

static std::unique_ptr<GraphicsDriver> _GraphicsDriver(nullptr);

TOON_ENGINE_API
void GraphicsDriver::InitializeUpdateContext()
{
    if (!_updateContext) {
        _updateContext.reset(new UpdateContext());
    }

    _updateContext->SetTargetFPS(60.0f);
    _updateContext->ResetTime();
}

TOON_ENGINE_API
void GraphicsDriver::InitializeRenderContext()
{
    if (!_renderContext) {
        _renderContext.reset(new RenderContext());
    }

    auto globals = _renderContext->GetShaderGlobals();
    globals->Resolution = GetWindowSize();
    globals->Mouse = {0, 0};
    globals->FrameCount = 0;
    globals->TotalTime = 0;
    globals->FrameSpeedRatio = 0.0f;

    auto transform = _renderContext->GetShaderTransform();
    transform->Model = glm::mat4(1.0f);
    transform->View = glm::mat4(1.0f);
    transform->Projection = glm::mat4(1.0f);
    transform->MVP = glm::mat4(1.0f);
}

TOON_ENGINE_API
bool GraphicsDriver::InitializeConstantBuffers()
{
    bool result;

    _shaderGlobalsBuffer = CreateBuffer();
    result = _shaderGlobalsBuffer->Initialize(
        sizeof(ShaderGlobals),
        nullptr,
        BufferUsage::Constant,
        MemoryUsage::UploadOften
    );

    return result;
}

TOON_ENGINE_API
void GraphicsDriver::Render()
{
    UpdateContext * updateCtx = GetUpdateContext();

    // Update FPS
    _fpsUpdateElaspedTime += updateCtx->GetPreviousFrameDuration();
    if (_fpsUpdateFrameCount >= updateCtx->GetTargetFPS()) {
        float currentFPS = (float)_fpsUpdateFrameCount / (float)_fpsUpdateElaspedTime.count();
        currentFPS *= 1000000.0f; // Microseconds -> Seconds

        char buffer[32];
        snprintf(buffer, sizeof(buffer), "%.2f FPS", currentFPS);
        UpdateWindowTitle(_windowTitle + " - " + buffer); // FIX

        _fpsUpdateElaspedTime = 0ms;
        _fpsUpdateFrameCount = 0;
    }

    ++_fpsUpdateFrameCount;

    auto input = GetInputDriver();

    auto globals = _renderContext->GetShaderGlobals();
    globals->Resolution = GetWindowSize();
    globals->Mouse = input->GetMouseCoordinates();
    ++globals->FrameCount;
    globals->TotalTime = (float)_updateContext->GetTotalDuration().count() / 100.0f;
    globals->FrameSpeedRatio = _updateContext->GetFrameSpeedRatio();

    _shaderGlobalsBuffer->WriteTo(0, sizeof(ShaderGlobals), reinterpret_cast<uint8_t *>(globals));
}

TOON_ENGINE_API
UpdateContext * GraphicsDriver::GetUpdateContext()
{
    return _updateContext.get();
}

TOON_ENGINE_API
RenderContext * GraphicsDriver::GetRenderContext()
{
    return _renderContext.get();
}

// PyObject * WindowResizedEventData::GetPyObject() const
// {
//     PyObject * dict = PyDict_New();

//     PyObject * size = PyTuple_New(2);
//     PyTuple_SET_ITEM(size, 0, PyLong_FromLong(Size.x));
//     PyTuple_SET_ITEM(size, 1, PyLong_FromLong(Size.y));
//     PyDict_SetItemString(dict, "Size", size);

//     return dict;
// }

TOON_ENGINE_API 
void SetGraphicsDriver(std::unique_ptr<GraphicsDriver> && driver)
{
    if (_GraphicsDriver) {
        _GraphicsDriver->Terminate();
    }
    _GraphicsDriver = std::move(driver);
}

TOON_ENGINE_API 
GraphicsDriver * GetGraphicsDriver()
{
    return _GraphicsDriver.get();
}

} // namespace Toon