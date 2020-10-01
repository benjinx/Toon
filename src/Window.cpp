#include "Window.hpp"

#include <Log.hpp>

#include <iostream>

Window::Window(int width, int height) 
    : _mWidth(width)
    , _mHeight(height)
{
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, GL_TRUE);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    LogInfo("Creating SDL2 Window.");

    _mWindow = SDL_CreateWindow(
        "Temporality ~ BC/DC Games", 
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        _mWidth,
        _mHeight,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    if (!_mWindow)
    {
        LogError("Failed to create SDL2 Window.");
        // I need to quit my project here
    }

    LogLoad("SDL2 Window created successfully.");

    LogInfo("Creating SDL2 GL Context.");

    _mGLContext = SDL_GL_CreateContext(_mWindow);

    if (!_mGLContext)
    {
        LogError("Failed to create SDL2 GL Context.");
        // I need to quit my project here
    }

    LogLoad("SDL2 GL Context created successfully.\n");

    gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
    glViewport(0, 0, width, height);

    // Makes an icon
    Uint16 pixels[16 * 16] = { 0xFFFF };
    SDL_Surface * surface = SDL_CreateRGBSurfaceFrom(pixels, 16, 16, 16, 16 * 2,
        0x0f00, 0x00f0, 0x000f, 0xf000);
    SDL_SetWindowIcon(_mWindow, surface);
    SDL_FreeSurface(surface);
}

Window::~Window() 
{
    SDL_GL_DeleteContext(_mGLContext);
    SDL_DestroyWindow(_mWindow);
    _mWindow = nullptr;
}

void Window::Clear() 
{ 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
}

void Window::Present()
{
    SDL_GL_SwapWindow(_mWindow);
}

void Window::OnWindowResize(glm::vec2 size)
{
    _mWidth = (int)size.x;
    _mHeight = (int)size.y;
    glViewport(0, 0, (GLsizei)_mWidth, (GLsizei)_mHeight);
}
