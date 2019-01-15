#ifndef APPLICATION_H
#define APPLICATION_H

#include <Config.hpp>
#include <Camera.hpp>
#include <Scene.hpp>
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

    void Run();

    void HandleGLFWKey(GLFWwindow* window, int key, int scancode, int action, int mode);
    void HandleGLFWMouseButton(GLFWwindow* window, int button, int action, int mode);
    void HandleGLFWScroll(GLFWwindow* window, double xoffset, double yoffset);
    void HandleGLFWMousePos(GLFWwindow* window, double x, double y);

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

private:

    static App* _sInst;

    void Start();
    void Update(float dt);
    void Render();
	void HandleInput(float dt);
	void OpenGLInfo();

    Window* _mWindow = nullptr;
	Scene* _mCurrentScene = nullptr;
    
    std::unordered_map<int, bool> _mInputMap;

	// List of Shaders
	std::unordered_map<std::string, Shader*> _mShaders;
    
	float _mLastMX = -1, 
          _mLastMY = -1;

	Camera* _mCurrentCamera = nullptr;
};

#endif // APPLICATION_H
