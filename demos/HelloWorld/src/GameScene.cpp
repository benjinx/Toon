#include "GameScene.hpp"

void GameScene::Start()
{
	// Object setup
	printf("\nLoading Models/Materials\n");

	// Scene Objs

	// Initialize Objs

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
