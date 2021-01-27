#ifndef TEMPORALITY_GRAPHICS_DRIVER_HPP
#define TEMPORALITY_GRAPHICS_DRIVER_HPP

#include <Temporality/Config.hpp>
#include <Temporality/Math.hpp>
#include <Temporality/UpdateContext.hpp>
#include <Temporality/RenderContext.hpp>
#include <Temporality/Pipeline.hpp>
#include <Temporality/Texture.hpp>
#include <Temporality/Shader.hpp>
#include <Temporality/Mesh.hpp>
#include <Temporality/Event.hpp>
#include <Temporality/Buffer.hpp>
#include <Temporality/Scene.hpp>

#include <string>
#include <vector>
#include <memory>

namespace Temporality {

// class TEMPORALITY_ENGINE_API WindowResizedEventData : public EventData
// {
// public:

//     glm::ivec2 Size;

//     //PyObject * GetPyObject() const override;

// }; // class WindowResizedEventData

class TEMPORALITY_ENGINE_API GraphicsDriver
{
public:

    DISALLOW_COPY_AND_ASSIGN(GraphicsDriver)

    GraphicsDriver() = default;

    virtual ~GraphicsDriver() = default;

    virtual bool Initialize();

    virtual void Terminate() = 0;

    virtual void SetWindowTitle(const std::string& title) = 0;

    virtual std::string GetWindowTitle() = 0;

    virtual void SetWindowSize(const glm::ivec2& size) = 0;

    virtual glm::ivec2 GetWindowSize() = 0;

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

    virtual bool AddConstantBuffer(std::shared_ptr<Buffer> buffer, unsigned binding);

    virtual bool RemoveConstantBuffer(unsigned binding);

    virtual Buffer * GetConstantBuffer(unsigned binding);

    virtual void ProcessEvents() = 0;
    
    virtual void Render() = 0;

    virtual std::shared_ptr<Pipeline> CreatePipeline(std::shared_ptr<Shader> shader) = 0;

    virtual std::shared_ptr<Texture> CreateTexture() = 0;

    virtual std::shared_ptr<Shader> CreateShader() = 0;

    virtual std::shared_ptr<Mesh> CreateMesh() = 0;

    virtual std::unique_ptr<Primitive> CreatePrimitive() = 0;

    virtual UpdateContext * GetUpdateContext();

    virtual RenderContext * GetRenderContext();

    //Event<Temporality::WindowResizedEventData> WindowResizedEvent;

    inline void SetCurrentScene(Scene * currentScene) {
        _currentScene = currentScene;
    }

protected:

    virtual void TermConstantBuffers();

    // Push Constants?

    glm::vec4 _clearColor = glm::vec4(0.392f, 0.584f, 0.929f, 1.0f);

    std::unique_ptr<UpdateContext> _updateContext;

    std::unique_ptr<RenderContext> _renderContext;

    unsigned _backbufferCount = 2;

    std::unordered_map<unsigned, std::shared_ptr<Buffer>> _constantBufferBindings;

    Scene * _currentScene = nullptr;

}; // class GraphicsDriver

TEMPORALITY_ENGINE_API void SetGraphicsDriver(std::unique_ptr<GraphicsDriver> && driver);

TEMPORALITY_ENGINE_API GraphicsDriver * GetGraphicsDriver();

} // namespace Temporality

#endif // TEMPORALITY_GRAPHICS_DRIVER_HPP