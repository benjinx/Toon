#ifndef WINDOW_H
#define WINDOW_H

#include <Config.hpp>
#include <OpenGL.hpp>

struct GLFWwindow;

class Window
{
public:

    Window(int width, int height);
    virtual ~Window();

	void Clear();
	void Present();

    GLFWwindow* GetGLFWWindow() { return _mWindow; }

    int GetWidth() { return _mWidth; }
    int GetHeight() { return _mHeight; }

    bool ShouldClose() { return glfwWindowShouldClose(_mWindow); }

private:

    GLFWwindow* _mWindow;
    int         _mWidth, _mHeight;

};

#endif // WINDOW_H
