#ifndef TOON_GRAPHICS_DRIVER_HPP
#define TOON_GRAPHICS_DRIVER_HPP

#include <Toon/Config.hpp>
#include <Toon/Buffer.hpp>
#include <Toon/Event.hpp>
#include <Toon/Pipeline.hpp>
#include <Toon/InputDriver.hpp>
#include <Toon/Math.hpp>
#include <Toon/Mesh.hpp>
#include <Toon/RenderContext.hpp>
#include <Toon/Shader.hpp>
#include <Toon/String.hpp>
#include <Toon/Texture.hpp>
#include <Toon/Time.hpp>
#include <Toon/UpdateContext.hpp>

#include <vector>
#include <memory>

namespace Toon {

// class TOON_ENGINE_API WindowResizedEventData : public EventData
// {
// public:

//     glm::ivec2 Size;

//     //PyObject * GetPyObject() const override;

// }; // class WindowResizedEventData

class TOON_ENGINE_API GraphicsDriver
{
public:

    DISALLOW_COPY_AND_ASSIGN(GraphicsDriver)

    GraphicsDriver() = default;

    virtual ~GraphicsDriver() = default;

    virtual bool Initialize() = 0;

    virtual void Terminate() = 0;

    virtual void InitializeUpdateContext();

    virtual void InitializeRenderContext();

    virtual bool InitializeConstantBuffers();

    virtual void SetWindowTitle(const string& title) {
        _windowTitle = title;
        UpdateWindowTitle(title);
    }

    virtual string GetWindowTitle() {
        return _windowTitle;
    }

    virtual void SetWindowSize(const glm::ivec2& size) {
        _windowSize = size;
        UpdateWindowSize(size);
    }

    virtual glm::ivec2 GetWindowSize() {
        return _windowSize;
    }

    virtual void SetBackbufferCount(unsigned backbufferCount) {
        _backbufferCount = backbufferCount;
    }

    virtual unsigned GetBackbufferCount() const {
        return _backbufferCount;
    }

    virtual void SetClearColor(const glm::vec4& color) {
        _clearColor = color;
    }

    virtual glm::vec4 GetClearColor() const {
        return _clearColor;
    }

    virtual void ProcessEvents() = 0;
    
    virtual void Render() = 0;

    virtual std::shared_ptr<Buffer> CreateBuffer() = 0;

    virtual std::shared_ptr<Pipeline> CreatePipeline(std::shared_ptr<Shader> shader) = 0;

    virtual std::shared_ptr<Texture> CreateTexture() = 0;

    virtual std::shared_ptr<Shader> CreateShader() = 0;

    virtual std::shared_ptr<Mesh> CreateMesh() = 0;

    virtual std::shared_ptr<Primitive> CreatePrimitive() = 0;

    virtual UpdateContext * GetUpdateContext();

    virtual RenderContext * GetRenderContext();

    //Event<Toon::WindowResizedEventData> WindowResizedEvent;

    std::shared_ptr<Buffer> GetShaderGlobalsBuffer() {
        return _shaderGlobalsBuffer;
    }

protected:

    virtual void UpdateWindowTitle(const string& title) = 0;

    virtual void UpdateWindowSize(const glm::ivec2& size) = 0;

    string _windowTitle = "Toon";

    glm::ivec2 _windowSize = { 640, 480 };

    microseconds _fpsUpdateElapsedTime = 0us;

    uintmax_t _fpsUpdateFrameCount = 0;
    
    // Push Constants?

    glm::vec4 _clearColor = glm::vec4(0.392f, 0.584f, 0.929f, 1.0f);

    std::unique_ptr<UpdateContext> _updateContext;

    std::unique_ptr<RenderContext> _renderContext;

    unsigned _backbufferCount = 2;

    std::unordered_map<unsigned, std::shared_ptr<Buffer>> _constantBufferBindings;

    std::shared_ptr<Buffer> _shaderGlobalsBuffer;

}; // class GraphicsDriver

TOON_ENGINE_API
void SetGraphicsDriver(std::unique_ptr<GraphicsDriver> && driver);

TOON_ENGINE_API
GraphicsDriver * GetGraphicsDriver();

} // namespace Toon

#endif // TOON_GRAPHICS_DRIVER_HPP