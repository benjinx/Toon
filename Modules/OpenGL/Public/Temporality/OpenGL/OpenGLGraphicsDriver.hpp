#ifndef TEMPORALITY_OPENGL_GRAPHICS_DRIVER_HPP
#define TEMPORALITY_OPENGL_GRAPHICS_DRIVER_HPP

#include <Temporality/OpenGL/OpenGLConfig.hpp>

#include <Temporality/SDL2/SDL2GraphicsDriver.hpp>
#include <Temporality/OpenGL/OpenGLPipeline.hpp>
#include <Temporality/OpenGL/OpenGLTexture.hpp>
#include <Temporality/OpenGL/OpenGLShader.hpp>
#include <Temporality/OpenGL/OpenGLMesh.hpp>
#include <Temporality/OpenGL/OpenGLPrimitive.hpp>
#include <Temporality/OpenGL/OpenGLBuffer.hpp>

#include <SDL.h>

namespace Temporality::OpenGL {

struct ShaderDataBindingInfo
{
    GLuint glID;
    size_t size;
};

class TEMPORALITY_OPENGL_API OpenGLGraphicsDriver : public SDL2::SDL2GraphicsDriver
{
public:

    DISALLOW_COPY_AND_ASSIGN(OpenGLGraphicsDriver)

    OpenGLGraphicsDriver() = default;

    virtual ~OpenGLGraphicsDriver() = default;

    bool Initialize() override;

    void Terminate() override;

    void Render() override;

    std::shared_ptr<Pipeline> CreatePipeline(std::shared_ptr<Shader> shader) override;

    std::shared_ptr<Texture> CreateTexture() override;

    std::shared_ptr<Shader> CreateShader() override;

    std::shared_ptr<Mesh> CreateMesh() override;

    std::unique_ptr<Primitive> CreatePrimitive() override;

private:

    void BindUniformBufferObjects();

    void InitDebugMessageCallback();

    SDL_GLContext _glContext = nullptr;

    std::unordered_map<std::string, GLuint> _shaderDataBindings;

    std::vector<std::weak_ptr<Shader>> _shaders;
}; // class OpenGLGraphicsDriver

} // namespace Temporality::OpenGL

#endif // TEMPORALITY_OPENGL_GRAPHICS_DRIVER_HPP