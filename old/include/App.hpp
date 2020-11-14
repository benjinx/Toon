#ifndef APPLICATION_H
#define APPLICATION_H

#include <Config.hpp>
#include <Camera.hpp>
#include <Scene.hpp>
#include <Input.hpp>
#include <Window.hpp>

class Shader;

class App
{
public:

    static App* Inst() { 
        return _sInst; 
    }

    App() { _sInst = this; }
    ~App();

    bool Start();
    void Run();
    void Quit() { _mRunning = false; }

    Window * GetWindow() { return _mWindow; }

    Scene * GetCurrentScene() { return _mCurrentScene; }
    void SetCurrentScene(Scene* scene) { _mCurrentScene = scene; }

    void Screenshot();

    void AddShader(std::string name, Shader* shader);
    Shader* GetShader(std::string name);
    void DeleteShaders();
    void ReloadShaders();

    void SetCurrentCamera(Camera* camera) { _mCurrentCamera = camera; }
    
    Camera* GetCurrentCamera() { return _mCurrentCamera; }

    Input* GetInput() { return &_mInput; }

private:

    static App* _sInst;

    void Update(float dt);
    void Render();
    void OpenGLInfo();

    // Should change window to not be a pointer and have an init
    Window* _mWindow = nullptr;
    Scene* _mCurrentScene = nullptr;
    Input _mInput;
    
    std::unordered_map<int, bool> _mInputMap;

    // List of Shaders
    std::unordered_map<std::string, Shader*> _mShaders;

    Camera* _mCurrentCamera = nullptr;

    bool _mRunning = true;
};

#endif // APPLICATION_H
