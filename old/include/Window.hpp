#ifndef WINDOW_H
#define WINDOW_H

#include <Config.hpp>
#include <OpenGL.hpp>

#include <glm/glm.hpp>

class Window
{
public:

    Window(int width, int height);
    virtual ~Window();

    void Clear();
    void Present();
    void OnWindowResize(glm::vec2 size);

    SDL_Window* GetSDLWindow() { return _mWindow; }
    SDL_GLContext GetGLContext() { return _mGLContext; }

    int GetWidth() { return _mWidth; }
    int GetHeight() { return _mHeight; }

private:

    SDL_Window* _mWindow;
    SDL_GLContext _mGLContext;
    int _mWidth, _mHeight;

};

#endif // WINDOW_H
