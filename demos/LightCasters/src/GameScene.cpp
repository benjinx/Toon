#include "GameScene.hpp"

#include <Light.hpp>

#include <imgui/imgui.h>

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
	_mGameObjects.emplace("Torus2", new GameObject("/models/Primitives/pTorus.glb"));
	_mGameObjects.emplace("Torus3", new GameObject("/models/Primitives/pTorus.glb"));

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

	_mGameObjects["Torus2"]->SetPosition(glm::vec3(4.0f, 1.0f, -1.0f));
	_mGameObjects["Torus2"]->SetRotation(glm::vec3(-90.0f, 0.0f, 90.0f));
	_mGameObjects["Torus2"]->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));

	_mGameObjects["Torus3"]->SetPosition(glm::vec3(8.0f, 1.0f, -1.0f));
	_mGameObjects["Torus3"]->SetRotation(glm::vec3(-90.0f, 0.0f, 90.0f));
	_mGameObjects["Torus3"]->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));

	// Shaders
	printf("\nLoading Shaders\n");

	App* app = App::Inst();
	app->AddShader("passThru", new Shader({
		"shaders/passThruColor.vert",
		"shaders/passThruColor.frag" }));

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
}

void GameScene::Update(float dt)
{
	Scene::Update(dt);

	// Get the application for ease.
	App* app = App::Inst();

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
	DirectionalLight dirLight(glm::vec3(-0.2f, -1.0f, -0.3f));
	if (_mDirLight)
	{
		lightCasters->SetBool("lightCheck.Directional", true);
		lightCasters->SetVec4("dirLight.direction", glm::vec4(dirLight.GetDirection(), 0.0f));
	}
	else
		lightCasters->SetBool("lightCheck.Directional", false);

	// Point Lighting
	// Set attenuation values
	PointLight pointLight(_mGameObjects["Light"]->GetPosition(),
						1.0f,
						0.09f,
						0.032f);
	if (_mPointLight)
	{
		lightCasters->SetBool("lightCheck.Point", true);
		lightCasters->SetFloat("pointLight.constant", pointLight.GetConstant());
		lightCasters->SetFloat("pointLight.linear", pointLight.GetLinear());
		lightCasters->SetFloat("pointLight.quadratic", pointLight.GetQuadratic());

		lightCasters->SetVec4("pointLight.position", glm::vec4(pointLight.GetPosition(), 1.0f));

		//lightDir = glm::vec4(-0.2f, -1.0f, -0.3f, 1.0f);
		//lightCasters->SetVec4("pointLight.direction", lightDir);
	}
	else
		lightCasters->SetBool("lightCheck.Point", false);

	// Spotlight Lighting
	SpotLight spotLight(App::Inst()->GetCurrentCamera()->GetPosition(),
						App::Inst()->GetCurrentCamera()->GetForward(), 
						glm::cos(glm::radians(12.5f)),
						glm::cos(glm::radians(17.5f)));
	if (_mSpotLight)
	{
		lightCasters->SetBool("lightCheck.Spot", true);
		lightCasters->SetVec3("spotlight.position", spotLight.GetPosition());

		// Change 0.0f to 1.0f to just enable spotlight
		glm::vec4 camFront = glm::vec4(spotLight.GetDirection(), 1.0f);
		lightCasters->SetVec4("spotlight.direction", camFront);

		lightCasters->SetFloat("spotlight.cutoff", spotLight.GetCutOff());
		lightCasters->SetFloat("spotlight.outerCutoff", spotLight.GetOuterCutOff());
	}
	else
		lightCasters->SetBool("lightCheck.Spot", false);

	// Rotate objects
	//_mGameObjects["Sphere"]->SetRotation(_mGameObjects["Sphere"]->GetRotation() + glm::vec3(0.0f, 0.25f * dt, 0.0f));
	//_mGameObjects["Cube"]->SetRotation(_mGameObjects["Cube"]->GetRotation() + glm::vec3(0.0f, 0.25f * dt, 0.0f));
	//_mGameObjects["Torus"]->SetRotation(_mGameObjects["Torus"]->GetRotation() + glm::vec3(0.0f, 0.0f, 0.25f * dt));
}
