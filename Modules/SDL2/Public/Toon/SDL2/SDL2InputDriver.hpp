#ifndef TOON_SDL2_INPUT_DRIVER_HPP
#define TOON_SDL2_INPUT_DRIVER_HPP

#include <Toon/SDL2/SDL2Config.hpp>

#include <Toon/InputDriver.hpp>

namespace Toon::SDL2 {

class TOON_SDL2_API SDL2InputDriver : public InputDriver
{
public:

    DISALLOW_COPY_AND_ASSIGN(SDL2InputDriver)

    SDL2InputDriver() = default;

    virtual ~SDL2InputDriver() = default;

    void ProcessEvent(SDL_Event * event);

    glm::ivec2 GetMouseCoordinates() const;

private:

    KeyboardKey GetSDLKey(int sdlKey);

    MouseButton GetSDLMouseButton(int button);

    ControllerButton GetSDLControllerButton(int button);
    
}; // class SDL2InputDriver

} // namespace Toon::SDL2

#endif // TOON_SDL2_INPUT_DRIVER_HPP
