#include "GameScene.h"

#include "UI.h"

void GameScene::Start()
{
	// Shaders
	SetupShaders();

	// UI
	UI::StartUI();

	// Camera
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);

	Camera::instance().Init(cameraPos, cameraTarget);

	// Object setup
	//_mGameObjects.emplace("Plane", Utils::LoadObj("resources/models/Primitives/pPlane.obj"));
	//_mGameObjects.emplace("Sphere", Utils::LoadObj("resources/models/Primitives/pSphere.obj"));

	_mGameObjects.emplace("Sun", Utils::LoadObj("resources/models/sun.obj"));
	//_mGameObjects.emplace("Earth", Utils::LoadObj("resources/models/earth.obj"));
	_mGameObjects.emplace("Cube", Utils::LoadObj("resources/models/Primitives/pCube.obj"));

	/*_mGameObjects["Plane"]->SetPosition(glm::vec3(0.0f, -0.5f, 0.0f));
	_mGameObjects["Plane"]->SetRotation(glm::vec3(-90.0f, 0.0f, 0.0f));
	_mGameObjects["Plane"]->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));*/

	_mGameObjects["Sun"]->SetPosition(glm::vec3(5.0f, 2.0f, 2.0f));
	_mGameObjects["Sun"]->SetScale(glm::vec3(2.0f, 2.0f, 2.0f));

	_mGameObjects["Cube"]->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	_mGameObjects["Cube"]->SetRotation(glm::vec3(20.0f, 0.0f, 20.0f));
	_mGameObjects["Cube"]->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));

	//_mGameObjects["Sphere"]->SetPosition(glm::vec3(0.0f, 0.0f, 1.0f));
	//_mGameObjects["Sphere"]->SetScale(glm::vec3(0.1f, 0.1f, 0.1f));

	//_mGameObjects["Earth"]->SetPosition(glm::vec3(0.0f, 1.0f, 0.0f));
	//_mGameObjects["Earth"]->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));

	// Load lua script
	_mScriptHost.Load();

	// Set default to lighting
	_mProg = 1;

	// Physics
	PhysicsStart();
}

void GameScene::SetupShaders()
{
	// Shaders
	_mNumShaders = 4;
	std::vector<std::string> vertShaders = {
		"resources/shaders/passThru.vert",
		"resources/shaders/basicLighting.vert",
		"resources/shaders/bpLighting.vert",
		"resources/shaders/nmLighting.vert",
	};

	std::vector<std::string> fragShaders = {
		"resources/shaders/passThru.frag",
		"resources/shaders/basicLighting.frag",
		"resources/shaders/bpLighting.frag",
		"resources/shaders/nmLighting.frag",
	};

	for (int i = 0; i < _mNumShaders; i++)
		_mShader.SetupShaders(vertShaders[i], fragShaders[i]);

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
	//for (int i = 0; i < _mNumShaders; i++)
	_mShader.Destroy();
}

void GameScene::Update(float dt)
{
	Camera::instance().Update(dt);

	//_mGameObjects["Sun"]->SetRotation(_mGameObjects["Sun"]->GetRotation() + glm::vec3(0.0f, 0.1f, 0.0f));
	//_mGameObjects["Earth"]->SetRotation(_mGameObjects["Earth"]->GetRotation() + glm::vec3(0.0f, 0.25f, 0.0f));
	_mGameObjects["Cube"]->SetRotation(_mGameObjects["Cube"]->GetRotation() + glm::vec3(0.0f, 0.25f, 0.0f));

	for (auto& gobj : _mGameObjects)
	{
		gobj.second->Update(dt);
	}

	PhysicsUpdate(dt);

	UI::UpdateUI();
}

void GameScene::Render()
{
	for (auto& gameObject : _mGameObjects)
	{
		if (gameObject.first == "Sun")
			gameObject.second->Render(0, &_mShader);
		else
			gameObject.second->Render(2, &_mShader);
	}

	UI::RenderUI();
}