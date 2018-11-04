#include "GameScene.hpp"

#include "UI.hpp"

void GameScene::Start()
{
	// Object setup
	printf("\nLoading Materials\n");

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
	_mGameObjects["Sphere"]->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));

	_mGameObjects["Cube"]->SetPosition(glm::vec3(-1.5f, -1.0f, 0.0f));
	_mGameObjects["Cube"]->SetRotation(glm::vec3(20.0f, 0.0f, 20.0f));
	_mGameObjects["Cube"]->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));

	_mGameObjects["Torus"]->SetPosition(glm::vec3(1.0f, 1.0f, -1.0f));
	_mGameObjects["Torus"]->SetRotation(glm::vec3(-90.0f, -90.0f, 0.0f));
	_mGameObjects["Torus"]->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));

	// Shaders
	SetupShaders();

	// UI
	UI::StartUI();

	// Camera
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);

	Camera::instance().Init(cameraPos, cameraTarget);
}

void GameScene::SetupShaders()
{
	// Shaders
	_mNumShaders = 3;
	std::vector<std::string> vertShaders = {
		"shaders/axis.vert",
		"shaders/passThru.vert",
		"shaders/advLighting.vert",
	};

	std::vector<std::string> fragShaders = {
		"shaders/axis.frag",
		"shaders/passThru.frag",
		"shaders/advLighting.frag",
	};

	printf("\nLoading Shaders\n");

	for (int i = 0; i < _mNumShaders; i++)
	{
		Shader* shader = new Shader();
		shader->SetupShaders(vertShaders[i], fragShaders[i]);
		_mShaders.push_back(shader);
	}

	// Clear Window
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	// Depth
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GameScene::DeleteShaders()
{
	// Destroy the shaders
	for (auto& shader : _mShaders)
		shader->Destroy();

	// Clear shader vector
	_mShaders.clear();
}

void GameScene::Update(float dt)
{
	// Set Shader values

	// Set Light Color
	_mShaders[1]->Use();
	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	_mShaders[1]->SetVec3("passColor", lightColor);

	// Set Light Position
	_mShaders[2]->Use();
	//_mShaders[2]->SetVec3("lightColor", lightColor);
	//glm::vec4 lightPos = glm::vec4(_mGameObjects["Light"]->GetPosition(), 1.0f);
	//_mShaders[2]->SetVec4("lightVec", lightPos);

	// Directional Light
	DirectionalLight* directionalLight = new DirectionalLight();
	directionalLight->SetDirection(glm::vec3(-0.2f, -1.0f, -0.3f)); // direction is light direction FROM the light source
	_mShaders[2]->SetVec3("dirLight.direction", directionalLight->GetDirection());
	_mShaders[2]->SetVec3("dirLight.ambient", glm::vec3(0.1f));
	_mShaders[2]->SetVec3("dirLight.diffuse", glm::vec3(1.0f));
	_mShaders[2]->SetVec3("dirLight.specular", glm::vec3(0.5f));

	// Point Light
	PointLight* pointLight = new PointLight();
	pointLight->SetPosition(glm::vec3(0.0f, -3.0f, 0.0f));
	pointLight->SetConstant(1.0f);
	pointLight->SetLinear(0.09);
	pointLight->SetQuadratic(0.032);

	_mShaders[2]->SetVec3("pointLight.position", pointLight->GetPosition());
	_mShaders[2]->SetVec3("pointLight.ambient", glm::vec3(0.1f));
	_mShaders[2]->SetVec3("pointLight.diffuse", glm::vec3(1.0f));
	_mShaders[2]->SetVec3("pointLight.specular", glm::vec3(0.5f));
	_mShaders[2]->SetFloat("pointLight.constant", pointLight->GetConstant());
	_mShaders[2]->SetFloat("pointLight.linear", pointLight->GetLinear());
	_mShaders[2]->SetFloat("pointLight.quadratic", pointLight->GetQuadratic());

	// Spot Light
	SpotLight* spotLight = new SpotLight();
	spotLight->SetPosition(Camera::instance().GetCameraPos());
	spotLight->SetDirection(Camera::instance().GetCameraForward());
	spotLight->SetCutOff(glm::cos(glm::radians(12.5f)));

	_mShaders[2]->SetVec3("spotLight.position", spotLight->GetPosition());
	_mShaders[2]->SetVec3("spotLight.direction", spotLight->GetDirection());
	_mShaders[2]->SetFloat("spotLight.cutoff", spotLight->GetCutOff());

	// Update Camera
	Camera::instance().Update(dt);

	// Rotate objects
	_mGameObjects["Sphere"]->SetRotation(_mGameObjects["Sphere"]->GetRotation() + glm::vec3(0.0f, 0.25f * dt, 0.0f));
	_mGameObjects["Cube"]->SetRotation(_mGameObjects["Cube"]->GetRotation() + glm::vec3(0.0f, 0.25f * dt, 0.0f));
	_mGameObjects["Torus"]->SetRotation(_mGameObjects["Torus"]->GetRotation() + glm::vec3(0.0f, 0.0f, 0.25f * dt));

	// Update objects
	for (auto& gobj : _mGameObjects)
	{
		gobj.second->Update(dt);
	}

	// Update UI
	UI::UpdateUI();
}

void GameScene::Render()
{
	// Render objects in scene
	for (auto& gameObject : _mGameObjects)
	{
		if (gameObject.first == "Light")
			gameObject.second->Render(_mShaders[1]);
		else
			gameObject.second->Render(_mShaders[2]);
	}

	// Render object axis
	if (UI::showAxis)
	{
		for (auto& gameOject : _mGameObjects)
		{
			if (gameOject.second->IsAxisEnabled())
			{
				gameOject.second->DrawAxis(_mShaders[0]);
			}
		}
	}

	// Render UI
	UI::RenderUI();
}
