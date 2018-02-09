#include "GameScene.h"

void GameScene::Start()
{
	// Shaders
	SetupShaders();

	// Object setup
	_mGameObjects.emplace("Plane", Utils::LoadObj("resources/models/plane.obj"));
	//_mGameObjects.emplace("Sphere", Utils::LoadObj("resources/models/pSphere.obj"));

	_mGameObjects.emplace("Sun", Utils::LoadObj("resources/models/sun.obj"));
	_mGameObjects.emplace("Earth", Utils::LoadObj("resources/models/earth.obj"));
	//_mGameObjects.emplace("Cube", Utils::LoadObj("resources/models/cube.obj"));

	//for (int i = 0; i < 10; i++)
	//{
	//	_mGameObjects.emplace("Earth" + std::to_string(i), Utils::LoadObj("resources/models/earth.obj"));
	//	_mGameObjects["Earth" + std::to_string(i)]->SetPosition(glm::vec3(i*2.0f, 0.0f, 0.0f));
	//}

	_mGameObjects["Plane"]->SetPosition(glm::vec3(0.0f, -0.5f, 0.0f));
	_mGameObjects["Plane"]->SetRotation(glm::vec3(-90.0f, 0.0f, 0.0f));
	_mGameObjects["Plane"]->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));

	_mGameObjects["Sun"]->SetPosition(glm::vec3(5.0f, 2.0f, 2.0f));
	_mGameObjects["Sun"]->SetScale(glm::vec3(2.0f, 2.0f, 2.0f));

	//_mGameObjects["Cube"]->SetPosition(glm::vec3(2.0f, 2.0f, -2.0f));
	//_mGameObjects["Cube"]->SetRotation(glm::vec3(20.0f, 0.0f, 20.0f));

	//_mGameObjects["Sphere"]->SetPosition(glm::vec3(0.0f, 1.0f, 0.0f));
	//_mGameObjects["Sphere"]->SetScale(glm::vec3(0.1f, 0.1f, 0.1f));

	_mGameObjects["Earth"]->SetPosition(glm::vec3(0.0f, 1.0f, 0.0f));
	_mGameObjects["Earth"]->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));
}

void GameScene::SetupShaders()
{
	// Shaders
	_mNumShaders = 2;
	std::vector<std::string> vertShaders = {
		"resources/shaders/nmLighting.vert",
		"resources/shaders/passThru.vert",
	};

	std::vector<std::string> fragShaders = {
		"resources/shaders/nmLighting.frag",
		"resources/shaders/passThru.frag",
	};

	for (int i = 0; i < _mNumShaders; i++)
		_mShader.SetupShaders(vertShaders[i], fragShaders[i]);

	// Depth
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GameScene::DeleteShaders()
{
	//for (int i = 0; i < _mNumShaders; i++)
	_mShader.Destroy();
}

void GameScene::Update(float dt)
{

	_mGameObjects["Earth"]->SetRotation(_mGameObjects["Earth"]->GetRotation() + glm::vec3(0.0f, 0.25f, 0.0f));

	for (auto& gobj : _mGameObjects)
	{
		gobj.second->Update(dt);
	}
}

void GameScene::Render()
{
	for (auto& gameObject : _mGameObjects)
	{
		//if (gameObject.first == "Sun")
		//	gameObject.second->Render(1, &_mShader);
		//else
		//	gameObject.second->Render(0, &_mShader);

		gameObject.second->Render(1, &_mShader);
	}

}