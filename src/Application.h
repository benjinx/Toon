#ifndef APPLICATION_H
#define APPLICATION_H

#include "Common.h"

#include "Camera.h"
#include "Window.h"
#include "Mesh.h"
#include "Material.h"
#include "Shader.h"
#include "UI.h"
#include "Utils.h"
#include "ScriptHost.h"
#include "Scene.h"
#include "GameScene.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

#include "Cube.h"

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

private:
    static Application* _sInst;
    void                Start();
    void                Update(float dt);
    void                Render();
    void                Destroy();
	void				HandleInput(float dt);

    Window _mWindow;
	ScriptHost _mScriptHost;
    std::unordered_map<int, bool> keysDown;
	GameScene _mGameScene;
	bool rightButtonDown = false;
	float			   _mDeltaTime;
    int                _mProg;
    std::vector<Mesh*> _mMeshes;
    ImVec4             Red         = ImVec4(255.0f, 0.0f, 0.0f, 255.0f);
    ImVec4             Yellow      = ImVec4(255.0f, 255.0f, 0.0f, 255.0f);
    ImVec4             Green       = ImVec4(0.0f, 255.0f, 0.0f, 255.0f);
    ImVec4             White       = ImVec4(255.0f, 255.0f, 255.0f, 255.0f);
    char               buffer[255] = {};
	bool			   ShowCredits;
	bool firstMouse;
	float lastX = 640, lastY = 360;
};

#endif // APPLICATION_H
