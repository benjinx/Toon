#include "Application.hpp"

Application* Application::_sInst = nullptr;

const float TARGET_FPS = 60.0f;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mode);
void mouse_pos_callback(GLFWwindow* window, double x, double y);

void Application::Run()
{
    const float targetElapsed = 1.0f / TARGET_FPS;

    Start();
	float prevTime = 0.0f;
    while (!glfwWindowShouldClose(_mWindow.GetWindow()))
    {
		float currTime = (float)glfwGetTime();
		float elapsed = currTime - prevTime;
        float dt = elapsed / targetElapsed;

		HandleInput(dt);
        Update(dt);
        Render();

		prevTime = currTime;
    }
    Destroy();
}

void Application::Start()
{
	// Welcome
	printf("Temporality Engine v%s\n\n", _mVersionNum.c_str());

	// Window
    _mWindow.Start();

	// Display OpenGL info
	OpenGLInfo();

	// Setup Scene
	_mpCurrentScene->Start();

	// Input
	glfwSetKeyCallback(_mWindow.GetWindow(), &key_callback);
	glfwSetMouseButtonCallback(_mWindow.GetWindow(), &mouse_button_callback);
	glfwSetScrollCallback(_mWindow.GetWindow(), &scroll_callback);
	glfwSetCursorPosCallback(_mWindow.GetWindow(), &mouse_pos_callback);

	glfwSetInputMode(_mWindow.GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	firstMouse = true;

	// Movement
	keysDown.emplace(GLFW_KEY_W, false);
	keysDown.emplace(GLFW_KEY_A, false);
	keysDown.emplace(GLFW_KEY_S, false);
	keysDown.emplace(GLFW_KEY_D, false);
	keysDown.emplace(GLFW_KEY_Q, false);
	keysDown.emplace(GLFW_KEY_E, false);

		// Other Input keys
	keysDown.emplace(GLFW_KEY_H, false);
}

void Application::Update(float dt)
{
	_mDeltaTime = dt;

    glfwPollEvents();

	_mpCurrentScene->Update(dt);
}

void Application::Render()
{
	_mWindow.Clear();

	_mpCurrentScene->Render();

	_mWindow.Present();
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

void Application::HandleInput(float dt)
{
	if (keysDown[GLFW_KEY_W])
		Camera::instance().HandleMovement(Direction::FORWARD, dt);
	if (keysDown[GLFW_KEY_S])
		Camera::instance().HandleMovement(Direction::BACKWARD, dt);
	if (keysDown[GLFW_KEY_A])
		Camera::instance().HandleMovement(Direction::LEFT, dt);
	if (keysDown[GLFW_KEY_D])
		Camera::instance().HandleMovement(Direction::RIGHT, dt);
	if (keysDown[GLFW_KEY_Q])
		Camera::instance().HandleMovement(Direction::UP, dt);
	if (keysDown[GLFW_KEY_E])
		Camera::instance().HandleMovement(Direction::DOWN, dt);

}

void Application::HandleGLFWKey(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (action == GLFW_RELEASE)
    {
        if (keysDown.find(key) != keysDown.end())
        {
            keysDown[key] = false;
        }
    }
    if (action == GLFW_PRESS)
    {
        if (keysDown.find(key) != keysDown.end())
        {
            keysDown[key] = true;
        }
        switch (key)
        {
        case GLFW_KEY_ESCAPE:
        {
			UI::settingsSelected = !UI::settingsSelected;
			break;
        }

        case GLFW_KEY_GRAVE_ACCENT:
        {
            UI::showMainMenuBar = !UI::showMainMenuBar;
            break;
        }

        case GLFW_KEY_F1:
        {
            UI::consoleSelected = !UI::consoleSelected;
            break;
        }

        case GLFW_KEY_F2:
        {
            UI::optionsSelected = !UI::optionsSelected;
            break;
        }

		case GLFW_KEY_F5: // Reloads shaders
		{
			std::cout << "\nReloading shaders!\n";
			
			_mpCurrentScene->DeleteShaders();
			_mpCurrentScene->SetupShaders();

			break;
		}

        case GLFW_KEY_F11:
        {
            UI::showTestWindow = !UI::showTestWindow;
            break;
        }
        }
    }

    ImGui_ImplGlfwGL3_KeyCallback(window, key, scancode, action, mode);
}

void Application::HandleGLFWMouseButton(GLFWwindow* window, int button, int action, int mode)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
		rightButtonDown = true;
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
		rightButtonDown = false;
		// tell camera that it can do handleRotation

    // Handle mouse button
    ImGui_ImplGlfwGL3_MouseButtonCallback(window, button, action, mode);
}

void Application::HandleGLFWScroll(GLFWwindow* window, double xoffset, double yoffset)
{
	Camera::instance().HandleFoV((float)xoffset, (float)yoffset);

    // scroll
    ImGui_ImplGlfwGL3_ScrollCallback(window, xoffset, yoffset);
}

void Application::HandleGLFWMousePos(GLFWwindow* window, double x, double y)
{
	if (firstMouse)
	{
		lastX = (float)x;
		lastY = (float)y;
		firstMouse = false;
	}

	float xoffset = (float)x - lastX;
	float yoffset = lastY - (float)y;

	lastX = (float)x;
	lastY = (float)y;

    // handle mouse pos
	if (rightButtonDown)
		Camera::instance().HandleRotation(xoffset, yoffset);
}

void Application::Destroy()
{
    ImGui_ImplGlfwGL3_Shutdown();
    _mWindow.Destroy();
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
