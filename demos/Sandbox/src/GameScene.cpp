#include "GameScene.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

void GameScene::Start()
{
	// Object setup
	printf("\nLoading Materials\n");

	// Scene Objs
	_mGameObjects.emplace("Light", Utils::LoadObj("models/Primitives/pCube.obj"));
	_mGameObjects.emplace("Cube", Utils::LoadObj("models/Primitives/pCube.obj"));

	// Initialize Objs
	_mGameObjects["Light"]->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	_mGameObjects["Light"]->SetScale(glm::vec3(0.3f, 0.3f, 0.3f));

	_mGameObjects["Cube"]->SetPosition(glm::vec3(-1.5f, -1.0f, 0.0f));
	_mGameObjects["Cube"]->SetRotation(glm::vec3(20.0f, 0.0f, 20.0f));
	_mGameObjects["Cube"]->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));

	// Shaders
	printf("\nLoading Shaders\n");

	Application* app = Application::Inst();
	app->AddShader("passThru", new Shader({
		"shaders/passThru.vert",
		"shaders/passThru.frag" }));

	app->AddShader("advLighting", new Shader({
		"shaders/advLighting.vert",
		"shaders/advLighting.frag" }));

	_mGameObjects["Light"]->SetShader(app->GetShader("passThru"));
	_mGameObjects["Cube"]->SetShader(app->GetShader("advLighting"));

	// Clear Window
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	// Depth
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// UI
	DevUI::Start();
	DevUI::RegisterOptionsFunc([this]() {
		ImGui::Checkbox("Enable Directional Light", &_mDirLight);
		ImGui::Checkbox("Enable Point Light", &_mPointLight);
		ImGui::Checkbox("Enable Spot Light", &_mSpotLight);
	});

	// Camera
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);

	Camera::Inst().Init(cameraPos, cameraTarget);

	// Load lua script
	//_mScriptHost.Load();

	// Physics
	PhysicsStart();
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

void GameScene::Update(float dt)
{
	// Get the application for ease.
	Application* app = Application::Inst();

	// Get reference to each shader
	Shader* passThru = app->GetShader("passThru");
	Shader* advLighting = app->GetShader("advLighting");

	passThru->Use();
	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	passThru->SetVec3("passColor", lightColor);

	advLighting->Use();

	advLighting->SetVec3("lightColor", lightColor);

	glm::vec4 lightPos = glm::vec4(_mGameObjects["Light"]->GetPosition(), 1.0f);
	advLighting->SetVec3("lightVec", lightPos);


	Camera::Inst().Update(dt);

	for (auto& gobj : _mGameObjects)
	{
		gobj.second->Update(dt);
	}

	PhysicsUpdate(dt);
}
