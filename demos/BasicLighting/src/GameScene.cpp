#include "GameScene.hpp"

void GameScene::Start()
{
	// Object setup
	printf("\nLoading Models/Materials\n");

	// Light Source
	_mGameObjects.emplace("Light", Utils::LoadObj("models/Primitives/pCube.obj"));

	_mGameObjects["Light"]->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	_mGameObjects["Light"]->SetScale(glm::vec3(0.3f, 0.3f, 0.3f));

	// Scene Objs
	_mGameObjects.emplace("Plane", Utils::LoadObj("models/Primitives/pPlane.obj"));
	_mGameObjects.emplace("Sphere", Utils::LoadObj("models/Primitives/pSphere.obj"));
	_mGameObjects.emplace("Cube", Utils::LoadObj("models/Primitives/pCube.obj"));
	_mGameObjects.emplace("Torus", Utils::LoadObj("models/Primitives/pTorus.obj"));

	// Initialize Objs

	_mGameObjects["Plane"]->SetPosition(glm::vec3(0.0f, -2.5f, 0.0f));
	_mGameObjects["Plane"]->SetRotation(glm::vec3(-90.0f, 0.0f, 0.0f));
	_mGameObjects["Plane"]->SetScale(glm::vec3(5.0f, 5.0f, 5.0f));

	_mGameObjects["Sphere"]->SetPosition(glm::vec3(1.5f, 0.0f, 2.0f));
	_mGameObjects["Sphere"]->SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
	_mGameObjects["Sphere"]->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));

	_mGameObjects["Cube"]->SetPosition(glm::vec3(-1.5f, -1.0f, 0.0f));
	_mGameObjects["Cube"]->SetRotation(glm::vec3(20.0f, 0.0f, 20.0f));
	_mGameObjects["Cube"]->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));

	_mGameObjects["Torus"]->SetPosition(glm::vec3(1.0f, 1.0f, -1.0f));
	_mGameObjects["Torus"]->SetRotation(glm::vec3(-90.0f, -90.0f, 0.0f));
	_mGameObjects["Torus"]->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));

	// Shaders
	printf("\nLoading Shaders\n");

	Application* app = Application::Inst();
	app->AddShader("passThru", new Shader({
		"shaders/passThru.vert",
		"shaders/passThru.frag" }));

	app->AddShader("basicLighting", new Shader({
		"shaders/basicLighting.vert",
		"shaders/basicLighting.frag" }));

	_mGameObjects["Light"]->SetShader(app->GetShader("passThru"));
	_mGameObjects["Plane"]->SetShader(app->GetShader("basicLighting"));
	_mGameObjects["Sphere"]->SetShader(app->GetShader("basicLighting"));
	_mGameObjects["Cube"]->SetShader(app->GetShader("basicLighting"));
	_mGameObjects["Torus"]->SetShader(app->GetShader("basicLighting"));

	// Clear Window
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	// Depth
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// UI
	DevUI::Start();

	// Camera
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);

	Camera::Inst().Init(cameraPos, cameraTarget);
}

void GameScene::Update(float dt)
{
	// Set Shader values
	
	// Get the application for ease.
	Application* app = Application::Inst();

	// Get reference to each shader
	Shader* passThru = app->GetShader("passThru");
	Shader* basicLighting = app->GetShader("basicLighting");


	// Set Light Color
	passThru->Use();
	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	passThru->SetVec3("passColor", lightColor);

	// Set Light Position
	basicLighting->Use();

	basicLighting->SetVec3("lightColor", lightColor);

	glm::vec4 lightPos = glm::vec4(_mGameObjects["Light"]->GetPosition(), 1.0f);
	basicLighting->SetVec4("lightVec", lightPos);

	glm::vec3 objColor = glm::vec3(UI::objectColor[0], UI::objectColor[1], UI::objectColor[2]);
	basicLighting->SetVec3("objectColor", objColor);

	// Update Camera
	Camera::Inst().Update(dt);

	// Rotate objects
	_mGameObjects["Sphere"]->SetRotation(_mGameObjects["Sphere"]->GetRotation() + glm::vec3(0.25f * dt, 0.0f, 0.0f));
	_mGameObjects["Cube"]->SetRotation(_mGameObjects["Cube"]->GetRotation() + glm::vec3(0.0f, 0.25f * dt, 0.0f));
	_mGameObjects["Torus"]->SetRotation(_mGameObjects["Torus"]->GetRotation() + glm::vec3(0.0f, 0.0f, 0.25f * dt));

	// Update objects
	for (auto& gobj : _mGameObjects)
	{
		gobj.second->Update(dt);
	}
}