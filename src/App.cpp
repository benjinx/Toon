#include "App.hpp"

#include <Camera.hpp>
#include <DevUI.hpp>
#include <Log.hpp>
#include <Shader.hpp>

#include <iostream>
#include <chrono>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl.h>
#include <imgui/imgui_impl_opengl3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

App* App::_sInst = nullptr;

const float TARGET_FPS = 60.0f;

App::~App() {
    DeleteShaders();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();

    delete _mWindow;
    _mWindow = nullptr;

    SDL_Quit();
}

void App::Run()
{
    using namespace std::chrono;
    typedef duration<double, std::milli> double_ms;
    typedef duration<float, std::milli> float_ms;

    high_resolution_clock::time_point prevTime = high_resolution_clock::now();
    float frameTime = 0.0f;

    SDL_Window* sdlWindow = _mWindow->GetSDLWindow();
    SDL_Event event;

    unsigned long frames = 0;

    double_ms frameDelay = 1000ms / TARGET_FPS;
    double_ms fpsDelay = 250ms; // Update FPS 4 times per second

    double_ms frameElap = 0ms;
    double_ms fpsElap = 0ms;

    while (_mRunning)
    {
        high_resolution_clock::time_point currTime = high_resolution_clock::now();
        auto elapsed = duration_cast<double_ms>(currTime - prevTime);

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                _mRunning = false;
            }

            switch (event.type)
            {
            case SDL_WINDOWEVENT:
            {
                if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                {
                    _mWindow->OnWindowResize({ event.window.data1, event.window.data2 });
                    _mCurrentCamera->SetAspect({ event.window.data1, event.window.data2 });
                }
            }
            break;
            }

            // Input
            _mInput.ProcessEvent(&event);
        }

        frameTime += (float)elapsed.count();
        float dt = duration_cast<float_ms>(elapsed / frameDelay.count()).count();
        prevTime = currTime;

        // Update
        Update(dt);


        // Render
        frameElap += elapsed;
        if (frameDelay <= frameElap)
        {
            frameElap = 0ms;
            ++frames;

            Render();

        }

        // Display FPS
        fpsElap += elapsed;
        if (fpsDelay <= fpsElap)
        {
            static char buffer[128];
            float fps = (float)(frames / fpsElap.count()) * 1000.f;
            sprintf(buffer, "Temporality ~ BC/DC Games - FPS: %0.2f", fps);
            SDL_SetWindowTitle(_mWindow->GetSDLWindow(), buffer);
        }

    }
}

bool App::Start()
{
    // Welcome
    printf("\nTemporality Engine v%s\n\n", VERSION);

    // Init SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) > 0)
    {
        LogError("Failed to init SDL!");
        return false;
    }

    // Window
    _mWindow = new Window(1024, 768);

    // Display OpenGL info
    OpenGLInfo();

    // Load Engine Shaders
    printf("\nLoading Engine Shaders\n");
    AddShader("axis", new Shader({
        "shaders/axis.vert",
        "shaders/axis.frag" }));

    AddShader("defaultLighting", new Shader({
        "shaders/defaultLighting.vert",
        "shaders/defaultLighting.frag" }));

    AddShader("skybox", new Shader({
        "shaders/skybox.vert",
        "shaders/skybox.frag" }));

    // Clear Window
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    // Depth
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Blend
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Setup Scene
    _mCurrentScene->Start();

    // Register the options function into the UI
    DevUI::RegisterOptionsFunc(&Scene::Options);

    // UI
    DevUI::Start();
    
    return true;
}

void App::Update(float dt)
{
    _mCurrentCamera->HandleMovement(dt);
    _mCurrentScene->Update(dt);
}

void App::Render()
{
    _mWindow->Clear();

    _mCurrentScene->Render();

    DevUI::Render();

    _mWindow->Present();
}

void App::OpenGLInfo()
{
    // OpenGL Basic Info
    LogInfo("OpenGL Vendor: %s", glGetString(GL_VENDOR));
    LogInfo("OpenGL Renderer: %s", glGetString(GL_RENDERER));
    LogInfo("OpenGL Version: %s", glGetString(GL_VERSION));
    LogInfo("GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    // Anti-Aliasing
    int samples;
    glGetIntegerv(GL_SAMPLES, &samples);
    LogInfo("Anti-Aliasing: %dx", samples);

    // Binary Shader Formats
    GLint formats = 0;
    glGetIntegerv(GL_NUM_PROGRAM_BINARY_FORMATS, &formats);
    LogInfo("Binary Shader Formats: %d", formats);

    // Max UBO Size
    int tmp;
    glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &tmp);
    LogInfo("Max UBO Size: %d", tmp);

    // Max Vertex Attributes
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &tmp);
    LogInfo("Max Vertex Attributes: %d\n", tmp);
    
    // Max Vertex UBOs
    glGetIntegerv(GL_MAX_VERTEX_UNIFORM_BLOCKS, &tmp);
    LogInfo("Max Vertex UBOs: %d", tmp);

    // Max Fragment UBOs
    glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_BLOCKS, &tmp);
    LogInfo("Max Fragment UBOs: %d", tmp);

    // Max Geometry UBOs
    glGetIntegerv(GL_MAX_GEOMETRY_UNIFORM_BLOCKS, &tmp);
    LogInfo("Max Geometry UBOs: %d", tmp);

    // Max UBO Bindings
    int maxBindings;
    glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &maxBindings);
    LogInfo("Max UBO Bindings: %d", maxBindings);
}

void App::Screenshot()
{
    std::vector<unsigned int> pixels(3 * GetWindow()->GetWidth() * GetWindow()->GetHeight());

    glReadPixels(0, 0, GetWindow()->GetWidth(), GetWindow()->GetHeight(), GL_RGB, GL_UNSIGNED_BYTE, pixels.data());
    
    stbi_flip_vertically_on_write(true);

    time_t now = time(0);
    tm* localtm = localtime(&now);
    std::string name = "Screenshot " + std::to_string(localtm->tm_mon+1) + std::to_string(localtm->tm_mday) + std::to_string(localtm->tm_year+1900) + "_"
                                                 + std::to_string(localtm->tm_hour) + std::to_string(localtm->tm_min) + std::to_string(localtm->tm_sec) + ".png";

    LogVerbose("Date: %s, %s, %s, Time: %s:%s:%s\n", std::to_string(localtm->tm_mon+1), std::to_string(localtm->tm_mday), std::to_string(localtm->tm_year+1900)
                                                   , std::to_string(localtm->tm_hour), std::to_string(localtm->tm_min), std::to_string(localtm->tm_sec));

    int screenshot = stbi_write_png(name.c_str(), GetWindow()->GetWidth(), GetWindow()->GetHeight(), 3, pixels.data(), 3 * GetWindow()->GetWidth());

    if (screenshot)
        LogInfo("Screenshot successful!");
    else
        LogInfo("Screenshot unsuccessful.");
}

void App::AddShader(std::string name, Shader* shader)
{
    _mShaders[name] = shader;
}

Shader* App::GetShader(std::string name)
{
    if (_mShaders.find(name) == _mShaders.end())
    {
        return nullptr;
    }

    return _mShaders[name];
}

void App::DeleteShaders()
{
    // Destroy the shaders
    for (auto& shader : _mShaders)
        shader.second->Destroy();

    // Clear shader vector
    _mShaders.clear();
}

void App::ReloadShaders()
{
    for (auto s : _mShaders)
        s.second->Reload();
}
