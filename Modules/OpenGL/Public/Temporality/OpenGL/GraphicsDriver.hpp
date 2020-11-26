#ifndef TEMPORALITY_OPENGL_GRAPHICS_DRIVER_HPP
#define TEMPORALITY_OPENGL_GRAPHICS_DRIVER_HPP

#include <Temporality/OpenGL/Config.hpp>
#include <Temporality/Graphics/GraphicsDriver.hpp>

#include <SDL.h>

namespace Temporality::OpenGL {

class TEMPORALITY_OPENGL_API GraphicsDriver : public Temporality::GraphicsDriver
{
public:

    DISALLOW_COPY_AND_ASSIGN(GraphicsDriver)

    GraphicsDriver();

    virtual ~GraphicsDriver();

    void SetWindowTitle(const std::string& title) override;

    std::string GetWindowTitle() override;

    void SetWindowSize(const glm::ivec2& size) override;

    glm::ivec2 GetWindowSize() override;

    void ProcessEvents() override;

    void SwapBuffers() override;

    std::shared_ptr<Texture> CreateTexture() override;

    std::shared_ptr<Shader> CreateShader() override;

    std::shared_ptr<Mesh> CreateMesh() override;

private:
    SDL_Window* _mWindow = nullptr;

    SDL_GLContext _mGLContext;
};

}

#endif // TEMPORALITY_OPENGL_GRAPHICS_DRIVER_HPP