#ifndef TOON_SDL2_GRAPHICS_DRIVER_HPP
#define TOON_SDL2_GRAPHICS_DRIVER_HPP

#include <Toon/SDL2/SDL2Config.hpp>

#include <Toon/GraphicsDriver.hpp>
#include <Toon/SDL2/SDL2InputDriver.hpp>

namespace Toon::SDL2 {

class TOON_SDL2_API SDL2GraphicsDriver : public GraphicsDriver
{
public:

    bool Initialize() override;

    void Terminate() override;

    virtual bool CreateWindow(unsigned flags);

    void ProcessEvents() override;

    SDL_Window * GetSDL2Window() const {
        return _sdlWindow;
    }

protected:

    void UpdateWindowTitle(const string& title) override;

    void UpdateWindowSize(const glm::ivec2& size) override;

private:

    SDL_Window * _sdlWindow = nullptr;

    SDL2InputDriver * _inputDriver = nullptr;

}; // class SDL2GraphicsDriver

} // namespace Toon::SDL2

#endif // TOON_SDL2_GRAPHICS_DRIVER_HPP