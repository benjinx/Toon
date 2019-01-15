#include "GameScene.hpp"

void GameScene::Start()
{
	Scene::Start();

	// Object setup
	printf("\nLoading Models/Materials\n");

	// Camera
	auto camera = new Camera();
	_mGameObjects.emplace("Camera", camera);
	_mGameObjects["Camera"]->SetPosition(glm::vec3(0.0f, 0.0f, 5.0f));
	App::Inst()->SetCurrentCamera(camera);

	// Scene Objs
	_mGameObjects.emplace("Logo", new GameObject("/models/logo/logo.glb"));

	// Initialize Objs
	_mGameObjects["Logo"]->SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));
	_mGameObjects["Logo"]->SetRotation(glm::angleAxis(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));

	// Shaders
	printf("\nLoading Shaders\n");

	App* app = App::Inst();
	app->AddShader("passThru", new Shader({
		"shaders/passThruTexture.vert",
		"shaders/passThruTexture.frag"
		}));

	_mGameObjects["Logo"]->SetShader(app->GetShader("passThru"));

	// UI
	DevUI::Start();

	// Camera
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
}

void GameScene::Update(float dt)
{
	Scene::Update(dt);
}
