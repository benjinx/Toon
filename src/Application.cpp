#include "Application.h"

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
		float currTime = glfwGetTime();
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
	// Window
    _mWindow.Start();

	// Setup Scene
	_mpCurrentScene = new GameScene();
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
			// Dans Engine - Calls deleteShaders then setupShaders
			std::cout << "Reloading shaders!\n";


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
	Camera::instance().HandleFoV(xoffset, yoffset);

    // scroll
    ImGui_ImplGlfwGL3_ScrollCallback(window, xoffset, yoffset);
}

void Application::HandleGLFWMousePos(GLFWwindow* window, double x, double y)
{
	if (firstMouse)
	{
		lastX = x;
		lastY = y;
		firstMouse = false;
	}

	float xoffset = x - lastX;
	float yoffset = lastY - y;

	lastX = x;
	lastY = y;

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
