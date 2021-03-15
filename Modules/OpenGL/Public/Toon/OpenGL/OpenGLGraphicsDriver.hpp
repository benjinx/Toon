#ifndef TOON_OPENGL_GRAPHICS_DRIVER_HPP
#define TOON_OPENGL_GRAPHICS_DRIVER_HPP

#include <Toon/OpenGL/OpenGLConfig.hpp>

#include <Toon/SDL2/SDL2GraphicsDriver.hpp>
#include <Toon/OpenGL/OpenGLPipeline.hpp>
#include <Toon/OpenGL/OpenGLTexture.hpp>
#include <Toon/OpenGL/OpenGLShader.hpp>
#include <Toon/OpenGL/OpenGLMesh.hpp>
#include <Toon/OpenGL/OpenGLMaterial.hpp>
#include <Toon/OpenGL/OpenGLPrimitive.hpp>
#include <Toon/OpenGL/OpenGLBuffer.hpp>

#include <SDL.h>

namespace Toon::OpenGL {

#define TOON_OPENGL_GRAPHICS_DRIVER(x) (dynamic_cast<Toon::OpenGL::OpenGLGraphicsDriver *>(x))

class TOON_OPENGL_API OpenGLGraphicsDriver : public SDL2::SDL2GraphicsDriver
{
public:

    DISALLOW_COPY_AND_ASSIGN(OpenGLGraphicsDriver)

    OpenGLGraphicsDriver() = default;

    virtual ~OpenGLGraphicsDriver() = default;

    bool Initialize() override;

    void Terminate() override;

    void Render() override;

    std::shared_ptr<Buffer> CreateBuffer() override;

    std::shared_ptr<Pipeline> CreatePipeline(std::shared_ptr<Shader> shader) override;

    std::shared_ptr<Texture> CreateTexture() override;

    std::shared_ptr<Shader> CreateShader() override;

    std::shared_ptr<Mesh> CreateMesh() override;

    std::shared_ptr<Material> CreateMaterial() override;

    std::shared_ptr<Primitive> CreatePrimitive() override;

private:

    void BindUniformBufferObjects();

    void InitDebugMessageCallback();

    SDL_GLContext _glContext = nullptr;

    std::vector<std::weak_ptr<Shader>> _shaderList;
}; // class OpenGLGraphicsDriver

} // namespace Toon::OpenGL

#endif // TOON_OPENGL_GRAPHICS_DRIVER_HPP