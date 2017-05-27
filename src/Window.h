#ifndef WINDOW_H
#define WINDOW_H

#include "Common.h"

struct GLFWwindow;

class Window
{
public:
    Window(){};
    ~Window(){};

    void Start();
    void Update(){};
    void Destroy();

    GLFWwindow* GetWindow() { return _mpWindow; }

    int GetWidth() { return _mWidth; }
    int GetHeight() { return _mHeight; }

private:
    GLFWwindow* _mpWindow;
    int         _mWidth, _mHeight;
};

#endif // WINDOW_H
