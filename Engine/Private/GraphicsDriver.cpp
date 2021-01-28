#include <Toon/GraphicsDriver.hpp>

namespace Toon {

static std::unique_ptr<GraphicsDriver> _GraphicsDriver(nullptr);

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

TOON_ENGINE_API
bool GraphicsDriver::Initialize()
{
    _updateContext.reset(new UpdateContext());
    _renderContext.reset(new RenderContext());

    return true;
}

TOON_ENGINE_API
bool GraphicsDriver::AddConstantBuffer(std::shared_ptr<Buffer> buffer, unsigned binding)
{
    auto it = _constantBufferBindings.find(binding);
    if (it != _constantBufferBindings.end()) {
        return false;
    }

    _constantBufferBindings[binding] = buffer;
    return true;
}

TOON_ENGINE_API
bool GraphicsDriver::RemoveConstantBuffer(unsigned binding)
{
    auto it = _constantBufferBindings.find(binding);
    if (it == _constantBufferBindings.end()) {
        return false;
    }

    _constantBufferBindings.erase(it);
    return true;
}

TOON_ENGINE_API
Buffer * GraphicsDriver::GetConstantBuffer(unsigned binding)
{
    auto it = _constantBufferBindings.find(binding);
    if (it == _constantBufferBindings.end()) {
        return nullptr;
    }

    return it->second.get();
}

TOON_ENGINE_API
void GraphicsDriver::TermConstantBuffers()
{
    _constantBufferBindings.clear();
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

} // namespace Toon