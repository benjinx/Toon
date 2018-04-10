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

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

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
    ImVec4             Red         = ImVec4(255.0f, 0.0f, 0.0f, 255.0f);
    ImVec4             Yellow      = ImVec4(255.0f, 255.0f, 0.0f, 255.0f);
    ImVec4             Green       = ImVec4(0.0f, 255.0f, 0.0f, 255.0f);
    ImVec4             White       = ImVec4(255.0f, 255.0f, 255.0f, 255.0f);
    char               buffer[255] = {};
	bool firstMouse;
	float lastX = 640, lastY = 360;

	std::string _mVersionNum = "0.01";
};

#endif // APPLICATION_H
