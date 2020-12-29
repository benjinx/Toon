#include <Temporality/OpenGL/OpenGLGraphicsDriver.hpp>
#include <Temporality/Log.hpp>

#include <glad/gl.h>

#include <Temporality/Temporality.hpp>
#include <Temporality/OpenGL/OpenGLMesh.hpp>
#include <Temporality/OpenGL/OpenGLShader.hpp>
#include <Temporality/OpenGL/OpenGLTexture.hpp>

namespace Temporality::OpenGL {

TEMPORALITY_OPENGL_API
OpenGLGraphicsDriver::OpenGLGraphicsDriver()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        LogInfo("Unable to initialize SDL");
        return;
    }

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
        "Temporality", 
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        800,
        600,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    if (!_mWindow)
    {
        LogError("Failed to create SDL2 Window.");
        // I need to quit my project here
        return;
    }

    LogLoad("SDL2 Window created successfully.");

    LogInfo("Creating SDL2 GL Context.");

    _mGLContext = SDL_GL_CreateContext(_mWindow);

    if (!_mGLContext)
    {
        LogError("Failed to create SDL2 GL Context.");
        // I need to quit my project here
        return;
    }

    LogLoad("SDL2 GL Context created successfully.\n");

    if (!gladLoadGL((GLADloadfunc) SDL_GL_GetProcAddress)) {
        LogError("Failed to initialize OpenGL context");
        return;
    }

    glViewport(0, 0, 800, 600);

    // Makes an icon
    Uint16 pixels[16 * 16] = { 0xFFFF };
    SDL_Surface * surface = SDL_CreateRGBSurfaceFrom(pixels, 16, 16, 16, 16 * 2,
        0x0f00, 0x00f0, 0x000f, 0xf000);
    SDL_SetWindowIcon(_mWindow, surface);
    SDL_FreeSurface(surface);


    glClearColor(0.0, 0.0, 0.0, 1.0);
}

TEMPORALITY_OPENGL_API
OpenGLGraphicsDriver::~OpenGLGraphicsDriver()
{
    if (_mGLContext) {
        SDL_GL_DeleteContext(_mGLContext);
        _mGLContext = nullptr;
    }
    if (_mWindow) {
        SDL_DestroyWindow(_mWindow);
        _mWindow = nullptr;
    }

    SDL_Quit();
}

TEMPORALITY_OPENGL_API
void OpenGLGraphicsDriver::SetWindowTitle(const std::string& title)
{
    SDL_SetWindowTitle(_mWindow, title.c_str());
}

TEMPORALITY_OPENGL_API
std::string OpenGLGraphicsDriver::GetWindowTitle()
{
    return (std::string)SDL_GetWindowTitle(_mWindow);
}

TEMPORALITY_OPENGL_API
void OpenGLGraphicsDriver::SetWindowSize(const glm::ivec2& size)
{
    SDL_SetWindowSize(_mWindow, size.x, size.y);
    glViewport(0, 0, (GLsizei)size.x, (GLsizei)size.y);
}

TEMPORALITY_OPENGL_API
glm::ivec2 OpenGLGraphicsDriver::GetWindowSize()
{
    glm::ivec2 size;
    SDL_GetWindowSize(_mWindow, &size.x, &size.y);
    return size;
}

TEMPORALITY_OPENGL_API
void OpenGLGraphicsDriver::ProcessEvents()
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

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

TEMPORALITY_OPENGL_API
void OpenGLGraphicsDriver::SwapBuffers()
{
    SDL_GL_SwapWindow(_mWindow);
}

TEMPORALITY_OPENGL_API
std::shared_ptr<Texture> OpenGLGraphicsDriver::CreateTexture()
{
    return std::make_shared<OpenGLTexture>();
}

TEMPORALITY_OPENGL_API
std::shared_ptr<Shader> OpenGLGraphicsDriver::CreateShader()
{
    return std::make_shared<OpenGLShader>();
}

TEMPORALITY_OPENGL_API
std::shared_ptr<Mesh> OpenGLGraphicsDriver::CreateMesh()
{
    return std::make_shared<OpenGLMesh>();
}

} // namespace Temporality::OpenGL