#include <Temporality/SDL2/SDL2GraphicsDriver.hpp>
#include <Temporality/Log.hpp>

#include <Temporality/Temporality.hpp>

namespace Temporality::SDL2 {

TEMPORALITY_SDL2_API
bool SDL2GraphicsDriver::Initialize()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        LogError("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }

    LogInfo("Creating SDL2 Window.");

    return true;
}

TEMPORALITY_SDL2_API
void SDL2GraphicsDriver::Terminate()
{
    if (_mWindow) {
        SDL_DestroyWindow(_mWindow);
        _mWindow = nullptr;
    }

    SDL_Quit();
}

TEMPORALITY_SDL2_API
bool SDL2GraphicsDriver::CreateWindow(unsigned flags)
{
    if (_mWindow) {
        SDL_DestroyWindow(_mWindow);
        _mWindow = nullptr;
    }

    _mWindow = SDL_CreateWindow(
        "Temporality", 
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800,
        600,
        flags);

    if (!_mWindow)
    {
        LogError("Failed to create SDL2 Window. %s", SDL_GetError());
        return false;
    }

    // Makes an icon
    Uint16 pixels[16 * 16] = { 0xFFFF };
    SDL_Surface * surface = SDL_CreateRGBSurfaceFrom(pixels, 16, 16, 16, 16 * 2,
        0x0f00, 0x00f0, 0x000f, 0xf000);
    SDL_SetWindowIcon(_mWindow, surface);
    SDL_FreeSurface(surface);

    return true;
}

TEMPORALITY_SDL2_API
void SDL2GraphicsDriver::SetWindowTitle(const std::string& title)
{
    SDL_SetWindowTitle(_mWindow, title.c_str());
}

TEMPORALITY_SDL2_API
std::string SDL2GraphicsDriver::GetWindowTitle()
{
    return (std::string)SDL_GetWindowTitle(_mWindow);
}

TEMPORALITY_SDL2_API
void SDL2GraphicsDriver::SetWindowSize(const glm::ivec2& size)
{
    SDL_SetWindowSize(_mWindow, size.x, size.y);
}

TEMPORALITY_SDL2_API
glm::ivec2 SDL2GraphicsDriver::GetWindowSize()
{
    glm::ivec2 size;
    SDL_GetWindowSize(_mWindow, &size.x, &size.y);
    return size;
}

TEMPORALITY_SDL2_API
void SDL2GraphicsDriver::ProcessEvents()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            SetRunning(false);
            break;
        case SDL_WINDOWEVENT:
            if (event.window.event == SDL_WINDOWEVENT_RESIZED)
            {
                //_mWindow->OnWindowResize({ event.window.data1, event.window.data2 });
                //_mCurrentCamera->SetAspect({ event.window.data1, event.window.data2 });
            }
            break;
        }

        // Input
        //_mInput.ProcessEvent(&event);
    }
}

} // namespace Temporality::SDL2