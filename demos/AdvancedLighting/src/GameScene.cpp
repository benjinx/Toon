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

	// Light Source
	_mGameObjects.emplace("Light", new GameObject("/models/Primitives/pCube.glb"));

	_mGameObjects["Light"]->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	_mGameObjects["Light"]->SetScale(glm::vec3(0.3f, 0.3f, 0.3f));

	// Scene Objs
	_mGameObjects.emplace("Plane", new GameObject("/models/Primitives/pPlane.glb"));
	_mGameObjects.emplace("Sphere", new GameObject("/models/Primitives/pSphere.glb"));
	_mGameObjects.emplace("Cube", new GameObject("/models/Primitives/pCube.glb"));
	_mGameObjects.emplace("Torus", new GameObject("/models/Primitives/pTorus.glb"));

	// Initialize Objs

	_mGameObjects["Plane"]->SetPosition(glm::vec3(0.0f, -2.5f, 0.0f));
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

	App* app = App::Inst();
	app->AddShader("passThru", new Shader({
		"shaders/passThruColor.vert",
		"shaders/passThruColor.frag" }));

	app->AddShader("advLighting", new Shader({
		"shaders/advLighting.vert",
		"shaders/advLighting.frag" }));

	_mGameObjects["Light"]->SetShader(app->GetShader("passThru"));
	_mGameObjects["Plane"]->SetShader(app->GetShader("advLighting"));
	_mGameObjects["Sphere"]->SetShader(app->GetShader("advLighting"));
	_mGameObjects["Cube"]->SetShader(app->GetShader("advLighting"));
	_mGameObjects["Torus"]->SetShader(app->GetShader("advLighting"));

	// UI
	DevUI::Start();

	// Camera
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
}

void GameScene::Update(float dt)
{
	Scene::Update(dt);

	// Set Shader values

	// Get the application for ease.
	App* app = App::Inst();

	// Get reference to each shader
	Shader* passThru = app->GetShader("passThru");
	Shader* advLighting = app->GetShader("advLighting");

	// Set Light Color
	passThru->Use();
	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	passThru->SetVec3("passColor", lightColor);

	// Set Light Position
	advLighting->Use();

	advLighting->SetVec3("lightColor", lightColor);
	glm::vec4 lightPos = glm::vec4(_mGameObjects["Light"]->GetPosition(), 1.0f);
	advLighting->SetVec4("lightVec", lightPos);

	glm::vec3 camPos = App::Inst()->GetCurrentCamera()->GetPosition();
	glm::vec4 eyePos = glm::vec4(camPos.x, camPos.y, camPos.z, 1.0f);
	advLighting->SetVec4("eyePos", eyePos);

	
	// Rotate objects
	_mGameObjects["Sphere"]->SetRotation(_mGameObjects["Sphere"]->GetRotation() *
										 (dt * 
										 glm::angleAxis(glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f))
										 ));
	_mGameObjects["Cube"]->SetRotation(_mGameObjects["Cube"]->GetRotation() *
									   (dt *
									   glm::angleAxis(glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f))
									   ));
	_mGameObjects["Torus"]->SetRotation(_mGameObjects["Torus"]->GetRotation() *
										(dt *
										glm::angleAxis(glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f))
										));
	//LogInfo("dt: %f \n", dt);
}
