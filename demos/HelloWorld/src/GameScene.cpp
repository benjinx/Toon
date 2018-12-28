#include "GameScene.hpp"

void GameScene::Start()
{
	// Object setup
	printf("\nLoading Models/Materials\n");

	// Scene Objs
	_mGameObjects.emplace("Logo", new GameObject("models/logo/logo.obj"));

	// Initialize Objs
	_mGameObjects["Logo"]->SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));

	// Shaders
	printf("\nLoading Shaders\n");

	App* app = App::Inst();
	app->AddShader("passThru", new Shader({
		"shaders/passThru.vert",
		"shaders/passThru.frag"
		}));

	_mGameObjects["Logo"]->SetShader(app->GetShader("passThru"));

	// UI
	DevUI::Start();

	// Camera
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);

	Camera::Inst().Init(cameraPos, cameraTarget);
}

void GameScene::Update(float dt)
{
	Scene::Update(dt);

	Camera::Inst().Update(dt);
}
