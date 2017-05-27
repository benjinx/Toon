#include "Application.h"

Application* Application::_sInst = nullptr;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mode);
void mouse_pos_callback(GLFWwindow* window, double x, double y);

void Application::Run()
{
    Start();
	float prevTime = glfwGetTime();
    while (!glfwWindowShouldClose(_mWindow.GetWindow()))
    {
		float currTime = glfwGetTime();
		float dt = currTime - prevTime;
        Update(dt);
        Render();
		prevTime = glfwGetTime();
    }
    Destroy();
}

void Application::Start()
{
    _mWindow.Start();
    _mShader.Start(&_mWindow);
    ImGui_ImplGlfwGL3_Init(_mWindow.GetWindow(), true);
    glfwSetKeyCallback(_mWindow.GetWindow(), &key_callback);
    glfwSetMouseButtonCallback(_mWindow.GetWindow(), &mouse_button_callback);
    glfwSetScrollCallback(_mWindow.GetWindow(), &scroll_callback);
    glfwSetCursorPosCallback(_mWindow.GetWindow(), &mouse_pos_callback);

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
		"resources/shaders/passThru.vert",
        "resources/shaders/normalMapping.vert",
    };

    std::vector<std::string> fragShaders = {
		"resources/shaders/passThru.frag",
        "resources/shaders/normalMapping.frag",
    };

    for (int i = 0; i < numShaders; i++)
        _mShader.SetupShaders(vertShaders[i], fragShaders[i]);

    // Depth
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Load Obj
	_mModels = Utils::LoadObj("resources/models/pcube.obj");

	// Set default to lighting
    _mProg = 1;

	//
	ShowCredits = false;
}

void Application::Update(float dt)
{
	ImGui_ImplGlfwGL3_NewFrame();
    // DeltaTime
    glfwPollEvents();

	_mDeltaTime = dt;
	
	// Update cam
		

		// forward, direction X velocity
		// back, -forward
		// right, direction X velocity
		// left, -right
		// up, direction X velocity
		// down, -up
	//

	//printf("%f\n", dt);
	Camera::instance().Update(dt);

    // 1. Show a simple window
    // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
    {
        if (UI::showMainMenuBar)
        {
            if (ImGui::BeginMainMenuBar())
            {
                if (ImGui::BeginMenu("Tools"))
                {
					ImGui::MenuItem("Settings", "ESC", &UI::settingsSelected, true);
                    ImGui::MenuItem("Console", "F1", &UI::consoleSelected, true);
                    ImGui::MenuItem("Options", "F2", &UI::optionsSelected, true);
                    ImGui::MenuItem("Test Window", "F11", &UI::showTestWindow, true);
                    ImGui::EndMenu();
                }

                ImGui::SameLine(_mWindow.GetWidth() - 150, 0.0f);
                ImGui::Text("%.2f FPS (%.2f ms)", ImGui::GetIO().Framerate, 1000.0f / ImGui::GetIO().Framerate);
                ImGui::EndMainMenuBar();
            }

            if (UI::consoleSelected)
            {
                ImGui::SetNextWindowSize(ImVec2(400, 200));
                ImGui::Begin("Console", &UI::consoleSelected);
				ImGui::BeginChild(1, ImVec2(390, 135));
                ImGui::TextColored(Red, "> Error: Test\n");
                ImGui::TextColored(Yellow, "> Warning: Test\n");
                ImGui::TextColored(Green, "> Loaded: Test\n");
                ImGui::TextColored(White, "> Normal Text: Test\n");
				ImGui::EndChild();
                ImGui::Text(">");
                ImGui::SameLine();
                ImGui::InputText("", buffer, 255);
                ImGui::End();
            }

            if (UI::optionsSelected)
            {
                ImGui::Begin("Options", &UI::optionsSelected);
				for (int i = 0; i < _mModels.size(); i++)
					ImGui::SliderFloat("Rotation Speed", _mModels[i]->GetMesh(0)->GetRotSpeed(), 0.0f, 5.0f);
				if (_mProg == 0)
				{
					if (ImGui::Button("NormalMapping"))
						_mProg = 1;
					ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
					for (unsigned int i = 0; i < _mModels.size(); i++)
					{
						for (unsigned int j = 0; j < _mModels[i]->GetNumMeshes(); j++)
						{
							if (ImGui::Button("Ambient"))
								_mModels[i]->GetMesh(j)->SetSel(0.0f);
							if (ImGui::Button("Diffuse"))
								_mModels[i]->GetMesh(j)->SetSel(1.0f);
							if (ImGui::Button("Specular"))
								_mModels[i]->GetMesh(j)->SetSel(2.0f);
							if (ImGui::Button("Bump"))
								_mModels[i]->GetMesh(j)->SetSel(3.0f);
							if (ImGui::Button("Color"))
								_mModels[i]->GetMesh(j)->SetSel(4.0f);
						}
					}
				}
				else
				{
					if (ImGui::Button("PassThru"))
						_mProg = 0;
				}

                ImGui::End();
            }

            if (UI::showTestWindow)
            {
                ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
                ImGui::ShowTestWindow(&UI::showTestWindow);
            }

			if (UI::settingsSelected)
			{
				ImGui::SetNextWindowSize(ImVec2(200, 400));
				ImGui::SetNextWindowPosCenter();
				ImGui::Begin("Settings", NULL, ImGuiWindowFlags_NoCollapse + ImGuiWindowFlags_AlwaysAutoResize);
				
				ImGui::TextColored(Green, "Created by BC/DC Games:\n");
				ImGui::TextColored(White, "- Benji Campbell\n");
				ImGui::TextColored(White, "- Daniel Covert\n");
				ImGui::TextColored(White, "- Stephen Lane-Walsh\n");
				ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

				if (ImGui::Button("Quit"))
					glfwSetWindowShouldClose(_mWindow.GetWindow(), GL_TRUE);
				ImGui::End();
			}
        }
    }
}

void Application::Render()
{
    _mShader.Clear();

    for (Model* model : _mModels)
    {
        model->Render(_mProg, &_mShader);
    }

    ImGui::Render();

    _mShader.Present();
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

	if (keysDown[GLFW_KEY_W])
		Camera::instance().Translate(glm::vec3(0, 0, -0.1));
    if (keysDown[GLFW_KEY_S])
        Camera::instance().Translate(glm::vec3(0, 0, 0.1));
    if (keysDown[GLFW_KEY_A])
        Camera::instance().Translate(glm::vec3(-0.1, 0, 0));
    if (keysDown[GLFW_KEY_D])
        Camera::instance().Translate(glm::vec3(0.1, 0, 0));
    if (keysDown[GLFW_KEY_Q])
        Camera::instance().Translate(glm::vec3(0, -0.1, 0));
    if (keysDown[GLFW_KEY_E])
        Camera::instance().Translate(glm::vec3(0, 0.1, 0));


	float camSpeed = 100000.0f * 0.1;
	glm::vec3 camFront = glm::vec3(0.0f, 0.0f, -1.0f);
	if (keysDown[GLFW_KEY_H])
		Camera::instance().SetCameraPos(Camera::instance().GetCameraPos() + camSpeed * camFront);

    ImGui_ImplGlfwGL3_KeyCallback(window, key, scancode, action, mode);
}

void Application::HandleGLFWMouseButton(GLFWwindow* window, int button, int action, int mode)
{
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
    // handle mouse pos
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
