#include "GameScene.hpp"

#include "UI.hpp"

void GameScene::Start()
{
	// Object setup
	printf("\nLoading Models/Materials\n");

	// Scene Objs

	// Initialize Objs

	// Shaders
	SetupShaders();

	// UI
	UI::StartUI();

	// Camera
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);

	Camera::instance().Init(cameraPos, cameraTarget);

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
}

void GameScene::PhysicsUpdate(float dt)
{
}

void GameScene::DeleteShaders()
{
	for (auto& shader : _mShaders)
		shader->Destroy();

	_mShaders.clear();
}

void GameScene::Update(float dt)
{
	Camera::instance().Update(dt);

	for (auto& gobj : _mGameObjects)
	{
		gobj.second->Update(dt);
	}

	PhysicsUpdate(dt);

	UI::UpdateUI();
}

void GameScene::Render()
{
	
	// Render Objects
	for (auto& gameObject : _mGameObjects)
	{
		gameObject.second->Render(_mShaders[1]);
	}

	// Axis Rendering for Objects
	for (auto& gameOject : _mGameObjects)
	{
		if (gameOject.second->IsAxisEnabled())
		{
			gameOject.second->DrawAxis(_mShaders[0]);
		}
	}

	UI::RenderUI();
}
