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

    explicit GraphicsDriver();

    virtual ~GraphicsDriver();

    virtual void SetWindowTitle(const std::string& title) override;

    virtual std::string GetWindowTitle() override;

    virtual void SetWindowSize(const glm::ivec2& size) override;

    virtual glm::ivec2 GetWindowSize() override;

    virtual void ProcessEvents() override;

    virtual void SwapBuffers() override;

private:
    SDL_Window* _mWindow = nullptr;

    SDL_GLContext _mGLContext;
};

}

#endif // TEMPORALITY_OPENGL_GRAPHICS_DRIVER_HPP