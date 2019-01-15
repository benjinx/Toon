#include "App.hpp"

#include <Camera.hpp>
#include <DevUI.hpp>
#include <Log.hpp>
#include <Shader.hpp>

#include <iostream>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw_gl3.h>

#define TINYGLTF_IMPLEMENTATION
#define TINYGLTF_NO_STB_IMAGE_WRITE
#include <tinygltf/tiny_gltf.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

App* App::_sInst = nullptr;

const float TARGET_FPS = 60.0f;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mode);
void mouse_pos_callback(GLFWwindow* window, double x, double y);

App::~App() {
	DeleteShaders();

    ImGui_ImplGlfwGL3_Shutdown();

	delete _mWindow;
	_mWindow = nullptr;
}

void App::Run()
{
    const float targetElapsed = 1.0f / TARGET_FPS;

    Start();

	float prevTime = 0.0f;
	float frameTime = 0.0f;
    while (!GetWindow()->ShouldClose()) {
		float currTime = (float)glfwGetTime();
		float elapsed = currTime - prevTime;
		frameTime += elapsed;
        float dt = elapsed / targetElapsed;

		HandleInput(dt);
        Update(dt);

		if (frameTime >= targetElapsed)
		{
			frameTime = 0.0f;
			Render();
		}

		prevTime = currTime;
    }
}

void App::Start()
{
	// Welcome
	printf("Temporality Engine v%s\n\n", VERSION);

	// Window
    _mWindow = new Window(1024, 768);

	// Display OpenGL info
	OpenGLInfo();

	// Load Engine Shaders
	printf("\nLoading Engine Shaders\n");
	AddShader("axis", new Shader({
		"shaders/axis.vert",
		"shaders/axis.frag" }));

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

	GLFWwindow * glfwWin = GetWindow()->GetGLFWWindow();

	// Input
	glfwSetKeyCallback(glfwWin, &key_callback);
	glfwSetMouseButtonCallback(glfwWin, &mouse_button_callback);
	glfwSetScrollCallback(glfwWin, &scroll_callback);
	glfwSetCursorPosCallback(glfwWin, &mouse_pos_callback);

	glfwSetInputMode(glfwWin, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// Movement
	_mInputMap[GLFW_KEY_W] = false;
	_mInputMap[GLFW_KEY_A] = false;
	_mInputMap[GLFW_KEY_S] = false;
	_mInputMap[GLFW_KEY_D] = false;
	_mInputMap[GLFW_KEY_Q] = false;
	_mInputMap[GLFW_KEY_E] = false;
	_mInputMap[GLFW_MOUSE_BUTTON_RIGHT] = false;

	// Other Input keys
	_mInputMap[GLFW_KEY_H] = false;

	// Register the options function into the UI
	DevUI::RegisterOptionsFunc(&Scene::Options);
}

void App::Update(float dt)
{
    glfwPollEvents();

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
	LogInfo("OpenGL Vendor: %s\n", glGetString(GL_VENDOR));
	LogInfo("OpenGL Renderer: %s\n", glGetString(GL_RENDERER));
	LogInfo("OpenGL Version: %s\n", glGetString(GL_VERSION));
	LogInfo("GLSL Version: %s\n\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	// Anti-Aliasing
	int samples;
	glGetIntegerv(GL_SAMPLES, &samples);
	LogInfo("Anti-Aliasing: %dx\n", samples);

	// Binary Shader Formats
	GLint formats = 0;
	glGetIntegerv(GL_NUM_PROGRAM_BINARY_FORMATS, &formats);
	LogInfo("Binary Shader Formats: %d\n", formats);

	// Max UBO Size
	int tmp;
	glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &tmp);
	LogInfo("Max UBO Size: %d\n", tmp);

	// Max Vertex UBOs
	glGetIntegerv(GL_MAX_VERTEX_UNIFORM_BLOCKS, &tmp);
	LogInfo("Max Vertex UBOs: %d\n", tmp);

	// Max Fragment UBOs
	glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_BLOCKS, &tmp);
	LogInfo("Max Fragment UBOs: %d\n", tmp);

	// Max Geometry UBOs
	glGetIntegerv(GL_MAX_GEOMETRY_UNIFORM_BLOCKS, &tmp);
	LogInfo("Max Geometry UBOs: %d\n", tmp);

	// Max UBO Bindings
	int maxBindings;
	glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &maxBindings);
	LogInfo("Max UBO Bindings: %d\n", maxBindings);
}

void App::Screenshot()
{
	std::vector<unsigned int> pixels(3 * GetWindow()->GetWidth() * GetWindow()->GetHeight());

	glReadPixels(0, 0, GetWindow()->GetWidth(), GetWindow()->GetHeight(), GL_RGB, GL_UNSIGNED_BYTE, pixels.data());
	
	stbi_flip_vertically_on_write(true);

	stbi_write_png("Screenshot.png", GetWindow()->GetWidth(), GetWindow()->GetHeight(), 3, pixels.data(), 3 * GetWindow()->GetWidth());
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

void App::HandleInput(float dt)
{
	if (_mInputMap[GLFW_KEY_W])
		_mCurrentCamera->HandleMovement(Direction::FORWARD, dt);
	if (_mInputMap[GLFW_KEY_S])
		_mCurrentCamera->HandleMovement(Direction::BACKWARD, dt);
	if (_mInputMap[GLFW_KEY_A])
		_mCurrentCamera->HandleMovement(Direction::LEFT, dt);
	if (_mInputMap[GLFW_KEY_D])
		_mCurrentCamera->HandleMovement(Direction::RIGHT, dt);
	if (_mInputMap[GLFW_KEY_Q])
		_mCurrentCamera->HandleMovement(Direction::UP, dt);
	if (_mInputMap[GLFW_KEY_E])
		_mCurrentCamera->HandleMovement(Direction::DOWN, dt);
}

void App::HandleGLFWKey(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (action == GLFW_RELEASE) {
        if (_mInputMap.find(key) != _mInputMap.end()) {
            _mInputMap[key] = false;
        }
    }
    if (action == GLFW_PRESS)
    {
        if (_mInputMap.find(key) != _mInputMap.end()) {
            _mInputMap[key] = true;
        }

		switch (key) {
			case GLFW_KEY_F5: // Reloads shaders
			{
				std::cout << "\nReloading shaders!\n";
				ReloadShaders();
				break;
			}
				
			case GLFW_KEY_PRINT_SCREEN:
			{
				Screenshot();
				break;
			}
        }
    }

	DevUI::HandleKeyEvent(key, scancode, action, mode);
	ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mode);
}

void App::HandleGLFWMouseButton(GLFWwindow* window, int button, int action, int mode)
{
    if (action == GLFW_RELEASE) {
        if (_mInputMap.find(button) != _mInputMap.end()) {
            _mInputMap[button] = false;
        }
    }
    if (action == GLFW_PRESS)
    {
        if (_mInputMap.find(button) != _mInputMap.end()) {
            _mInputMap[button] = true;
        }
	}

    // Handle mouse button
    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mode);
}

void App::HandleGLFWScroll(GLFWwindow* window, double xoffset, double yoffset)
{
	//Camera::Inst().HandleFoV((float)xoffset, (float)yoffset);

    // scroll
    ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
}

void App::HandleGLFWMousePos(GLFWwindow* window, double x, double y)
{
	if (_mLastMX < 0 && _mLastMY < 0) {
		_mLastMX = (float)x;
		_mLastMY = (float)y;
	}

	float xoffset = (float)x - _mLastMX;
	float yoffset = _mLastMY - (float)y;

	_mLastMX = (float)x;
	_mLastMY = (float)y;

    // handle mouse pos
	if (_mInputMap[GLFW_MOUSE_BUTTON_RIGHT]) {
		_mCurrentCamera->HandleRotation(xoffset, yoffset);
	}
}

// Not part of the class itself but used for key callback.
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    App::Inst()->HandleGLFWKey(window, key, scancode, action, mode);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mode)
{
    App::Inst()->HandleGLFWMouseButton(window, button, action, mode);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    App::Inst()->HandleGLFWScroll(window, xoffset, yoffset);
}

void mouse_pos_callback(GLFWwindow* window, double x, double y)
{
    App::Inst()->HandleGLFWMousePos(window, x, y);
}
