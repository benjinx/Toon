#include "Window.hpp"

#include <iostream>

Window::Window(int width, int height) 
    : _mWidth(width)
    , _mHeight(height)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
    glfwWindowHint(GLFW_DECORATED, true);
    glfwWindowHint(GLFW_RESIZABLE, false);
	glfwWindowHint(GLFW_SAMPLES, 8);

    _mWindow = glfwCreateWindow(_mWidth, _mHeight, "Temporality ~ BC/DC Games", nullptr, nullptr);

    if (_mWindow == nullptr)
    {
        std::cout << "Failed to create GLFW Window: " << std::endl;
        glfwTerminate();
    }

    glfwMakeContextCurrent(_mWindow);

    glfwSwapInterval(0);

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    glfwGetFramebufferSize(_mWindow, &width, &height);
    glViewport(0, 0, width, height);
}

Window::~Window() 
{
    glfwDestroyWindow(_mWindow);
    _mWindow = nullptr;

    glfwTerminate(); 
}

void Window::Clear() 
{ 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
}

void Window::Present()
{
    glfwSwapBuffers(_mWindow); 
}
