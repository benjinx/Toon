#include "GameScene.hpp"

#include "UI.hpp"
#include "DevUI.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

void GameScene::Start()
{
	// Object setup
	printf("\nLoading Materials\n");

	// Scene Objs

	// Initialize Objs

	// Shaders
	SetupShaders();

	// UI
	//UI::StartUI();
	DevUI::Start();
	DevUI::RegisterOptionsFunc([this]() {
		ImGui::Checkbox("Enable Directional Light", &_mDirLight);
		ImGui::Checkbox("Enable Point Light", &_mPointLight);
		ImGui::Checkbox("Enable Spot Light", &_mSpotLight);
	});

	// Camera
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);

	Camera::instance().Init(cameraPos, cameraTarget);

	// Load lua script
	//_mScriptHost.Load();

	// Set default to lighting
	_mProg = 1;

	// Physics
	PhysicsStart();
}

void GameScene::SetupShaders()
{
	// Shaders
	_mNumShaders = 2;
	std::vector<std::string> vertShaders = {
		"shaders/axis.vert",
		"shaders/passThru.vert",
	};

	std::vector<std::string> fragShaders = {
		"shaders/axis.frag",
		"shaders/passThru.frag",
	};

	printf("\nLoading Shaders\n");

	for (int i = 0; i < _mNumShaders; i++)
	{
		Shader* shader = new Shader();
		shader->SetupShaders(vertShaders[i], fragShaders[i]);
		_mShaders.push_back(shader);
	}

	// Clear Window
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	// Depth
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GameScene::PhysicsStart()
{
	// Testing Physics System Currently
	//glm::vec3 pos(2.0f, 2.0f, 2.0f);
	//Cube cube;
	//cube.SetPosition(pos);
	//cube.Update();
}

void GameScene::PhysicsUpdate(float dt)
{
	//
	//_mScene.GetGameObjects()["Sphere"]->Update(dt);
}

void GameScene::DeleteShaders()
{
	for (auto& shader : _mShaders)
		shader->Destroy();

	_mShaders.clear();
}

void GameScene::Update(float dt)
{
	// Testing shader value setting
	//_mShaders[4]->Use();
	//glm::vec4 lightPos = glm::vec4(5.0f, 2.0f, 2.0f, 1.0f);
	//_mShaders[4]->SetVec4("lightPos", lightPos);

	Camera::instance().Update(dt);

	for (auto& gobj : _mGameObjects)
	{
		gobj.second->Update(dt);
	}

	PhysicsUpdate(dt);

	// Old update UI
	//UI::UpdateUI();
}

void GameScene::Render()
{
	
	for (auto& gameObject : _mGameObjects)
	{
		if (gameObject.first == "Sun")
			gameObject.second->Render(_mShaders[1]);
		else
			gameObject.second->Render(_mShaders[4]);
	}

	// Render object axis
	if (UI::showAxis)
	{
		for (auto& gameOject : _mGameObjects)
		{
			if (gameOject.second->IsAxisEnabled())
			{
				gameOject.second->DrawAxis(_mShaders[0]);
			}
		}
	}

	// Draw Scene Axis
	//DrawAxis(0, &_mShader);

	//UI::RenderUI();
	DevUI::Render();
}
