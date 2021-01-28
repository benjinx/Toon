#ifndef TOON_SDL2_GRAPHICS_DRIVER_HPP
#define TOON_SDL2_GRAPHICS_DRIVER_HPP

#include <Toon/SDL2/SDL2Config.hpp>
#include <Toon/GraphicsDriver.hpp>

#include <SDL.h>

namespace Toon::SDL2 {

class TOON_SDL2_API SDL2GraphicsDriver : public GraphicsDriver
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

} // namespace Toon::SDL2

#endif // TOON_SDL2_GRAPHICS_DRIVER_HPP