#include "GameScene.hpp"

#include "imgui/imgui.h"

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
	_mGameObjects.emplace("Torus2", Utils::LoadObj("models/Primitives/pTorus.obj"));
	_mGameObjects.emplace("Torus3", Utils::LoadObj("models/Primitives/pTorus.obj"));

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

	_mGameObjects["Torus2"]->SetPosition(glm::vec3(4.0f, 1.0f, -1.0f));
	_mGameObjects["Torus2"]->SetRotation(glm::vec3(-90.0f, 0.0f, 90.0f));
	_mGameObjects["Torus2"]->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));

	_mGameObjects["Torus3"]->SetPosition(glm::vec3(8.0f, 1.0f, -1.0f));
	_mGameObjects["Torus3"]->SetRotation(glm::vec3(-90.0f, 0.0f, 90.0f));
	_mGameObjects["Torus3"]->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));

	// Shaders
	printf("\nLoading Shaders\n");

	Application* app = Application::Inst();
	app->AddShader("passThru", new Shader({
		"shaders/passThru.vert",
		"shaders/passThru.frag" }));

	app->AddShader("lightCasters", new Shader({
		"shaders/lightCasters.vert",
		"shaders/lightCasters.frag" }));

	_mGameObjects["Light"]->SetShader(app->GetShader("passThru"));
	_mGameObjects["Plane"]->SetShader(app->GetShader("lightCasters"));
	_mGameObjects["Sphere"]->SetShader(app->GetShader("lightCasters"));
	_mGameObjects["Cube"]->SetShader(app->GetShader("lightCasters"));
	_mGameObjects["Torus"]->SetShader(app->GetShader("lightCasters"));
	_mGameObjects["Torus2"]->SetShader(app->GetShader("lightCasters"));
	_mGameObjects["Torus3"]->SetShader(app->GetShader("lightCasters"));

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
}

void GameScene::Update(float dt)
{
	// Get the application for ease.
	Application* app = Application::Inst();

	// Get reference to each shader
	Shader* passThru = app->GetShader("passThru");
	Shader* lightCasters = app->GetShader("lightCasters");

	// Set Shader values

	// Set Light Color
	passThru->Use();
	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	passThru->SetVec3("passColor", lightColor);

	// Set Light Position
	lightCasters->Use();
	lightCasters->SetVec3("lightColor", lightColor);

	// Directional Lighting
	// Set Directional Light Position
	glm::vec4 lightDir = glm::vec4(-0.2f, -1.0f, -0.3f, 0.0f);
	if (_mDirLight)
	{
		lightCasters->SetBool("lightCheck.Directional", true);
		lightCasters->SetVec4("dirLight.direction", lightDir);
	}
	else
		lightCasters->SetBool("lightCheck.Directional", false);

	// Point Lighting
	// Set attenuation values
	float	constant = 1.0f,
			linear = 0.09f,
			quadratic = 0.032f;

	if (_mPointLight)
	{
		lightCasters->SetBool("lightCheck.Point", true);
		lightCasters->SetFloat("pointLight.constant", constant);
		lightCasters->SetFloat("pointLight.linear", linear);
		lightCasters->SetFloat("pointLight.quadratic", quadratic);

		glm::vec4 lightPos = glm::vec4(_mGameObjects["Light"]->GetPosition(), 1.0f);
		lightCasters->SetVec4("pointLight.position", lightPos);

		lightDir = glm::vec4(-0.2f, -1.0f, -0.3f, 1.0f);
		lightCasters->SetVec4("pointLight.direction", lightDir);
	}
	else
		lightCasters->SetBool("lightCheck.Point", false);

	// Spotlight Lighting
	if (_mSpotLight)
	{
		lightCasters->SetBool("lightCheck.Spot", true);
		lightCasters->SetVec3("spotlight.position", Camera::Inst().GetCameraPos());

		// Change 0.0f to 1.0f to just enable spotlight
		glm::vec4 camFront = glm::vec4(Camera::Inst().GetCameraForward(), 1.0f);
		lightCasters->SetVec4("spotlight.direction", camFront);

		lightCasters->SetFloat("spotlight.cutoff", glm::cos(glm::radians(12.5f)));
		lightCasters->SetFloat("spotlight.outerCutoff", glm::cos(glm::radians(17.5f)));
	}
	else
		lightCasters->SetBool("lightCheck.Spot", false);

	// Update Camera
	Camera::Inst().Update(dt);

	// Rotate objects
	_mGameObjects["Sphere"]->SetRotation(_mGameObjects["Sphere"]->GetRotation() + glm::vec3(0.0f, 0.25f * dt, 0.0f));
	_mGameObjects["Cube"]->SetRotation(_mGameObjects["Cube"]->GetRotation() + glm::vec3(0.0f, 0.25f * dt, 0.0f));
	_mGameObjects["Torus"]->SetRotation(_mGameObjects["Torus"]->GetRotation() + glm::vec3(0.0f, 0.0f, 0.25f * dt));

	// Update objects
	for (auto& gobj : _mGameObjects)
	{
		gobj.second->Update(dt);
	}
}
