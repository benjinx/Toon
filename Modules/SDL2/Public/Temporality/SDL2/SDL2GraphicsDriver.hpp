#ifndef TEMPORALITY_SDL2_GRAPHICS_DRIVER_HPP
#define TEMPORALITY_SDL2_GRAPHICS_DRIVER_HPP

#include <Temporality/SDL2/SDL2Config.hpp>
#include <Temporality/GraphicsDriver.hpp>

#include <SDL.h>

namespace Temporality::SDL2 {

class TEMPORALITY_SDL2_API SDL2GraphicsDriver : public GraphicsDriver
{
public:

    bool Initialize() override;

    void Terminate() override;

    virtual bool CreateWindow(unsigned flags);
    
    void SetWindowTitle(const std::string& title) override;

    std::string GetWindowTitle() override;

    void SetWindowSize(const glm::ivec2& size) override;

    glm::ivec2 GetWindowSize() override;

    void ProcessEvents() override;

    SDL_Window * GetSDL2Window() const {
        return _mWindow;
    }

private:
    SDL_Window* _mWindow = nullptr;
}; // class SDL2GraphicsDriver

} // namespace Temporality::SDL2

#endif // TEMPORALITY_SDL2_GRAPHICS_DRIVER_HPP