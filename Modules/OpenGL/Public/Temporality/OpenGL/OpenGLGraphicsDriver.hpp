#ifndef TEMPORALITY_OPENGL_GRAPHICS_DRIVER_HPP
#define TEMPORALITY_OPENGL_GRAPHICS_DRIVER_HPP

#include <Temporality/OpenGL/OpenGLConfig.hpp>
#include <Temporality/SDL2/SDL2GraphicsDriver.hpp>

#include <SDL.h>

namespace Temporality::OpenGL {

class TEMPORALITY_OPENGL_API OpenGLGraphicsDriver : public SDL2::SDL2GraphicsDriver
{
public:

    DISALLOW_COPY_AND_ASSIGN(OpenGLGraphicsDriver)

    OpenGLGraphicsDriver() = default;

    virtual ~OpenGLGraphicsDriver() = default;

    bool Initialize() override;

    void Terminate() override;

    void SwapBuffers() override;

    std::shared_ptr<Texture> CreateTexture() override;

    std::shared_ptr<Shader> CreateShader() override;

    std::shared_ptr<Mesh> CreateMesh() override;

private:
    void InitDebugMessageCallback();

    SDL_GLContext _mGLContext;
}; // class OpenGLGraphicsDriver

} // namespace Temporality::OpenGL

#endif // TEMPORALITY_OPENGL_GRAPHICS_DRIVER_HPP