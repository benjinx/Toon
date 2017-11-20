#include "Application.h"

Application* Application::_sInst = nullptr;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mode);
void mouse_pos_callback(GLFWwindow* window, double x, double y);

void Application::Run()
{
    Start();
	float prevTime = 0.0f;
    while (!glfwWindowShouldClose(_mWindow.GetWindow()))
    {
		float currTime = glfwGetTime();
		float dt = currTime - prevTime;
		HandleInput(dt);
        Update(dt);
        Render();
		prevTime = currTime;
    }
    Destroy();
}

void Application::Start()
{
    _mWindow.Start();
    _mShader.Start(&_mWindow);
	UI::StartUI(&_mWindow);
    glfwSetKeyCallback(_mWindow.GetWindow(), &key_callback);
    glfwSetMouseButtonCallback(_mWindow.GetWindow(), &mouse_button_callback);
    glfwSetScrollCallback(_mWindow.GetWindow(), &scroll_callback);
    glfwSetCursorPosCallback(_mWindow.GetWindow(), &mouse_pos_callback);

	glfwSetInputMode(_mWindow.GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	firstMouse = true;

    keysDown.emplace(GLFW_KEY_W, false);
    keysDown.emplace(GLFW_KEY_A, false);
    keysDown.emplace(GLFW_KEY_S, false);
    keysDown.emplace(GLFW_KEY_D, false);
    keysDown.emplace(GLFW_KEY_Q, false);
    keysDown.emplace(GLFW_KEY_E, false);

	keysDown.emplace(GLFW_KEY_H, false);

    // Camera
    glm::vec3 cameraPos    = glm::vec3(0.0f, 0.0f, 2.0f);
    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 up           = glm::vec3(0.0f, 1.0f, 0.0f);

    Camera::instance().Init(&_mWindow, cameraPos, cameraTarget, up);

    // Shaders
	int numShaders = 2;
    std::vector<std::string> vertShaders = {
		"resources/shaders/nmLighting.vert",
        "resources/shaders/passThru.vert",
    };

    std::vector<std::string> fragShaders = {
		"resources/shaders/nmLighting.frag",
        "resources/shaders/passThru.frag",
    };

    for (int i = 0; i < numShaders; i++)
        _mShader.SetupShaders(vertShaders[i], fragShaders[i]);

    // Depth
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Load Obj
	//_mGameObjects.emplace("Plane", Utils::LoadObj("resources/models/plane.obj"));
	_mGameObjects.emplace("Sun", Utils::LoadObj("resources/models/sun.obj"));
	_mGameObjects.emplace("Earth", Utils::LoadObj("resources/models/earth.obj"));

	//for (int i = 0; i < 10; i++)
	//{
	//	_mGameObjects.emplace("Earth" + std::to_string(i), Utils::LoadObj("resources/models/earth.obj"));
	//	_mGameObjects["Earth" + std::to_string(i)]->SetPosition(glm::vec3(i*2.0f, 0.0f, 0.0f));
	//}

	//_mGameObjects["Plane"]->SetPosition(glm::vec3(3.0f, 0.0f, 0.0f));

	_mGameObjects["Sun"]->SetPosition(glm::vec3(5.0f, 2.0f, 2.0f));
	_mGameObjects["Sun"]->SetScale(glm::vec3(2.0f, 2.0f, 2.0f));


	// Set default to lighting
    _mProg = 1;

	//
	ShowCredits = false;

	// Load lua script
	_mScriptHost.Load();

	// Physics
	//PhysicsStart();
}

void Application::PhysicsStart()
{
	// Testing Physics System Currently
	glm::vec3 pos(2.0f, 2.0f, 2.0f);
	Cube cube;
	cube.SetPosition(pos);
	cube.Update();
}

void Application::Update(float dt)
{
	_mDeltaTime = dt;

    glfwPollEvents();

	UI::UpdateUI(&_mWindow);

	//printf("%f\n", dt);
	Camera::instance().Update(dt);

	_mGameObjects["Earth"]->SetRotation(glm::vec3(0.0f, 0.25f, 0.0f));
	_mGameObjects["Sun"]->SetRotation(glm::vec3(0.0f, 0.1f, 0.0f));

	//PhysicsUpdate(dt);
}

void Application::PhysicsUpdate(float dt)
{
	//
	glm::vec3 pos(1.0f, 1.0f, 1.0f), rot(0.0f), scale(2.0f, 2.0f, 2.0f);
	GameObject gobj;
	gobj.SetTransform(pos, rot, scale);
	gobj.Update(_mDeltaTime);
}

void Application::Render()
{
    _mShader.Clear();

	for (auto& gameObject : _mGameObjects)
	{ 
		if (gameObject.first == "Sun")
			gameObject.second->Render(1, &_mShader);
		else
			gameObject.second->Render(0, &_mShader);
	}

	UI::RenderUI();

    _mShader.Present();
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
    _mShader.Destroy();
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
