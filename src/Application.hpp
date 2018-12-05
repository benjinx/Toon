#ifndef APPLICATION_H
#define APPLICATION_H

#include "Common.hpp"

#include "Camera.hpp"
#include "Window.hpp"
#include "Mesh.hpp"
#include "Material.hpp"
#include "Shader.hpp"
#include "UI.hpp"
#include "Utils.hpp"
#include "Scene.hpp"

#include "Cube.hpp"

class Application
{
public:
    static Application* Inst() { return _sInst; }

    Application() { _sInst = this; }
    ~Application() {}

    void Run();

    void HandleGLFWKey(GLFWwindow* window, int key, int scancode, int action, int mode);
    void HandleGLFWMouseButton(GLFWwindow* window, int button, int action, int mode);
    void HandleGLFWScroll(GLFWwindow* window, double xoffset, double yoffset);
    void HandleGLFWMousePos(GLFWwindow* window, double x, double y);

	int GetWindowWidth() { return _mWindow.GetWidth(); }
	int GetWindowHeight() { return _mWindow.GetHeight(); }
	GLFWwindow* GetWindow() { return _mWindow.GetWindow(); }

	void SetCurrentScene(Scene* scene) { _mpCurrentScene = scene; }

	void SetDemoName(std::string demoName) { _mDemoName = demoName; }
	std::string GetDemoName() { return _mDemoName; }

	void Screenshot();

private:
    static Application* _sInst;
    void                Start();
    void                Update(float dt);
    void                Render();
    void                Destroy();
	void				HandleInput(float dt);
	void				OpenGLInfo();

    Window _mWindow;
    std::unordered_map<int, bool> keysDown;
	Scene* _mpCurrentScene;
	bool rightButtonDown = false;
	float			   _mDeltaTime;
    std::vector<Mesh*> _mMeshes;
	bool firstMouse;
	float lastX = 640, lastY = 360;

	std::string _mVersionNum = "0.01";
	std::string _mDemoName;
};

#endif // APPLICATION_H
