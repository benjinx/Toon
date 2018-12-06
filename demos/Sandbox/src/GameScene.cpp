#include "GameScene.hpp"

#include "UI.hpp"

void GameScene::Start()
{
	// Object setup
	printf("\nLoading Materials\n");

	// Scene Objs
	_mGameObjects.emplace("Sun", Utils::LoadObj("models/Primitives/pSphere.obj"));
	_mGameObjects.emplace("Earth", Utils::LoadObj("models/earth.obj"));

	// Initialize Objs
	_mGameObjects["Sun"]->SetPosition(glm::vec3(5.0f, 2.0f, 2.0f));
	_mGameObjects["Sun"]->SetScale(glm::vec3(2.0f, 2.0f, 2.0f));

	_mGameObjects["Earth"]->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	_mGameObjects["Earth"]->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));

	// Shaders
	SetupShaders();

	// UI
	UI::StartUI();

	// Camera
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);

	Camera::instance().Init(cameraPos, cameraTarget);

	// Load lua script
	//_mScriptHost.Load();

	// Set default to lighting
	_mProg = 1;

	// Physics
	PhysicsStart();
}

void GameScene::SetupShaders()
{
	// Shaders
	_mNumShaders = 6;
	std::vector<std::string> vertShaders = {
		"shaders/axis.vert",
		"shaders/passThru.vert",
		"shaders/basicLighting.vert",
		"shaders/advLighting.vert",
		"shaders/normalMapping.vert",
		"shaders/multiLights.vert",
	};

	std::vector<std::string> fragShaders = {
		"shaders/axis.frag",
		"shaders/passThru.frag",
		"shaders/basicLighting.frag",
		"shaders/advLighting.frag",
		"shaders/normalMapping.frag",
		"shaders/multiLights.frag",
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

void GameScene::DeleteShaders()
{
	for (auto& shader : _mShaders)
		shader->Destroy();

	_mShaders.clear();
}

void GameScene::Update(float dt)
{
	// Testing shader value setting
	_mShaders[4]->Use();
	glm::vec4 lightPos = glm::vec4(5.0f, 2.0f, 2.0f, 1.0f);
	_mShaders[4]->SetVec4("lightPos", lightPos);

	///
	_mShaders[5]->Use();
	
	DirectionalLight* dl = new DirectionalLight();
	dl->SetDirection(glm::vec3(-0.2f, -1.0f, -0.3f));
	_mShaders[5]->SetVec3("dirLight.direction", dl->GetDirection());
	_mShaders[5]->SetVec3("dirLight.ambient", glm::vec3(0.1f));
	_mShaders[5]->SetVec3("dirLight.diffuse", glm::vec3(1.0f));
	_mShaders[5]->SetVec3("dirLight.specular", glm::vec3(0.5f));

	PointLight* pl = new PointLight();
	pl->SetPosition(glm::vec3(0.0f, -3.0f, 0.0f));
	pl->SetConstant(1.0f);
	pl->SetLinear(0.09);
	pl->SetQuadratic(0.032);

	_mShaders[5]->SetVec3("pointLights[0].position", pl->GetPosition());
	_mShaders[5]->SetVec3("pointLights[0].ambient", glm::vec3(0.1f));
	_mShaders[5]->SetVec3("pointLights[0].diffuse", glm::vec3(1.0f));
	_mShaders[5]->SetVec3("pointLights[0].specular", glm::vec3(0.5f));
	_mShaders[5]->SetFloat("pointLights[0].constant", pl->GetConstant());
	_mShaders[5]->SetFloat("pointLights[0].linear", pl->GetLinear());
	_mShaders[5]->SetFloat("pointLights[0].quadratic", pl->GetQuadratic());

	// Lights w/ Shader 2
	//_mShaders[2]->Use();

	/// Directional Light
	//DirectionalLight* directionalLight = new DirectionalLight();
	//directionalLight->SetDirection(glm::vec4(-0.2f, -1.0f, -0.3f, 0.0f)); // we define direction FROM the light source so it's pointing down
	//_mShaders[2]->SetVec4("lightVec", directionalLight->GetDirection());

	/// Point Light
	//PointLight* pointLight = new PointLight();
	//pointLight->SetPosition(glm::vec4(5.0f, 2.0f, 2.0f, 1.0f));
	//pointLight->SetConstant(1.0f);
	//pointLight->SetLinear(0.09f);
	//pointLight->SetQuadratic(0.032f);
	//_mShaders[2]->SetVec4("lightVec", pointLight->GetPosition());
	//_mShaders[2]->SetFloat("lightCon", pointLight->GetConstant());
	//_mShaders[2]->SetFloat("lightLin", pointLight->GetLinear());
	//_mShaders[2]->SetFloat("lightQuad", pointLight->GetQuadratic());

	/// Spot Light
	//SpotLight* spotLight = new SpotLight();
	//glm::vec3 camPo = Camera::instance().GetCameraPos();
	//spotLight->SetPosition(glm::vec4(camPo.x, camPo.y, camPo.z, 1.0f));

	//glm::vec3 camFront = Camera::instance().GetCameraForward();
	//spotLight->SetDirection(glm::vec4(camFront.x, camFront.y, camFront.z, 1.0f));

	//spotLight->SetCutOff(glm::cos(glm::radians(12.5f)));

	//_mShaders[2]->SetVec4("lightPos", spotLight->GetPosition());
	//_mShaders[2]->SetVec4("lightDir", spotLight->GetDirection());
	//_mShaders[2]->SetFloat("lightCutOff", spotLight->GetCutOff());

	Camera::instance().Update(dt);

	_mGameObjects["Earth"]->SetRotation(_mGameObjects["Earth"]->GetRotation() + glm::vec3(0.0f, 0.25f * dt, 0.0f));

	for (auto& gobj : _mGameObjects)
	{
		gobj.second->Update(dt);
	}

	PhysicsUpdate(dt);

	UI::UpdateUI();
}

void GameScene::Render()
{
	
	for (auto& gameObject : _mGameObjects)
	{
		if (gameObject.first == "Sun")
			gameObject.second->Render(_mShaders[1]);
		else
			gameObject.second->Render(_mShaders[4]);
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

	// Draw Scene Axis
	//DrawAxis(0, &_mShader);

	UI::RenderUI();
}
