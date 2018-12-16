#include "Application.hpp"
#include "Config.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

Application* Application::_sInst = nullptr;

const float TARGET_FPS = 60.0f;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mode);
void mouse_pos_callback(GLFWwindow* window, double x, double y);

Application::~Application() {
	delete _mWindow;
	_mWindow = nullptr;

	Camera::Delete();
	
    ImGui_ImplGlfwGL3_Shutdown();

	DeleteShaders();
}

void Application::Run()
{
    const float targetElapsed = 1.0f / TARGET_FPS;

    Start();
	float prevTime = 0.0f;
    while (!GetWindow()->ShouldClose()) {
		float currTime = (float)glfwGetTime();
		float elapsed = currTime - prevTime;
        float dt = elapsed / targetElapsed;

		HandleInput(dt);
        Update(dt);
        Render();

		prevTime = currTime;
    }
}

void Application::Start()
{
	// Welcome
	printf("Temporality Engine v%s\n\n", VERSION);

	// Window
    _mWindow = new Window(1024, 768);

	// Display OpenGL info
	OpenGLInfo();

	// Load Engine Shaders
	AddShader("axis", new Shader({
		"shaders/axis.vert",
		"shaders/axis.frag" }));

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

void Application::Update(float dt)
{
    glfwPollEvents();

	_mCurrentScene->Update(dt);
}

void Application::Render()
{
	_mWindow->Clear();

	_mCurrentScene->Render();

	DevUI::Render();

	_mWindow->Present();
}

void Application::OpenGLInfo()
{
	// OpenGL Basic Info
	printf("OpenGL Vendor: %s\n", glGetString(GL_VENDOR));
	printf("OpenGL Renderer: %s\n", glGetString(GL_RENDERER));
	printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
	printf("GLSL Version: %s\n\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	// Anti-Aliasing
	int samples;
	glGetIntegerv(GL_SAMPLES, &samples);
	printf("Anti-Aliasing: %dx\n", samples);

	// Binary Shader Formats
	GLint formats = 0;
	glGetIntegerv(GL_NUM_PROGRAM_BINARY_FORMATS, &formats);
	printf("Binary Shader Formats: %d\n", formats);

	// Max UBO Size
	int tmp;
	glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &tmp);
	printf("Max UBO Size: %d\n", tmp);

	// Max Vertex UBOs
	glGetIntegerv(GL_MAX_VERTEX_UNIFORM_BLOCKS, &tmp);
	printf("Max Vertex UBOs: %d\n", tmp);

	// Max Fragment UBOs
	glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_BLOCKS, &tmp);
	printf("Max Fragment UBOs: %d\n", tmp);

	// Max Geometry UBOs
	glGetIntegerv(GL_MAX_GEOMETRY_UNIFORM_BLOCKS, &tmp);
	printf("Max Geometry UBOs: %d\n", tmp);

	// Max UBO Bindings
	int maxBindings;
	glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &maxBindings);
	printf("Max UBO Bindings: %d\n", maxBindings);
}

void Application::Screenshot()
{
	std::vector<unsigned int> pixels(3 * GetWindow()->GetWidth() * GetWindow()->GetHeight());

	glReadPixels(0, 0, GetWindow()->GetWidth(), GetWindow()->GetHeight(), GL_RGB, GL_UNSIGNED_BYTE, pixels.data());
	
	stbi_flip_vertically_on_write(true);

	stbi_write_png("Screenshot.png", GetWindow()->GetWidth(), GetWindow()->GetHeight(), 3, pixels.data(), 3 * GetWindow()->GetWidth());
}

void Application::AddShader(std::string name, Shader* shader)
{
	_mShaders[name] = shader;
}

Shader* Application::GetShader(std::string name)
{
	if (_mShaders.find(name) == _mShaders.end())
	{
		return nullptr;
	}

	return _mShaders[name];
}

void Application::DeleteShaders()
{
	// Destroy the shaders
	for (auto& shader : _mShaders)
		shader.second->Destroy();

	// Clear shader vector
	_mShaders.clear();
}

void Application::HandleInput(float dt)
{
	if (_mInputMap[GLFW_KEY_W])
		Camera::Inst().HandleMovement(Direction::FORWARD, dt);
	if (_mInputMap[GLFW_KEY_S])
		Camera::Inst().HandleMovement(Direction::BACKWARD, dt);
	if (_mInputMap[GLFW_KEY_A])
		Camera::Inst().HandleMovement(Direction::LEFT, dt);
	if (_mInputMap[GLFW_KEY_D])
		Camera::Inst().HandleMovement(Direction::RIGHT, dt);
	if (_mInputMap[GLFW_KEY_Q])
		Camera::Inst().HandleMovement(Direction::UP, dt);
	if (_mInputMap[GLFW_KEY_E])
		Camera::Inst().HandleMovement(Direction::DOWN, dt);
}

void Application::HandleGLFWKey(GLFWwindow* window, int key, int scancode, int action, int mode)
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
				
				//_mCurrentScene->DeleteShaders();
				//_mCurrentScene->SetupShaders();

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

void Application::HandleGLFWMouseButton(GLFWwindow* window, int button, int action, int mode)
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

void Application::HandleGLFWScroll(GLFWwindow* window, double xoffset, double yoffset)
{
	Camera::Inst().HandleFoV((float)xoffset, (float)yoffset);

    // scroll
    ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
}

void Application::HandleGLFWMousePos(GLFWwindow* window, double x, double y)
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
		Camera::Inst().HandleRotation(xoffset, yoffset);
	}
}

// Not part of the class itself but used for key callback.
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    Application::Inst()->HandleGLFWKey(window, key, scancode, action, mode);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mode)
{
    Application::Inst()->HandleGLFWMouseButton(window, button, action, mode);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    Application::Inst()->HandleGLFWScroll(window, xoffset, yoffset);
}

void mouse_pos_callback(GLFWwindow* window, double x, double y)
{
    Application::Inst()->HandleGLFWMousePos(window, x, y);
}
