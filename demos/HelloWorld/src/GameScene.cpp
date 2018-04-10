#include "GameScene.hpp"

#include "UI.hpp"

void GameScene::Start()
{
	// Shaders
	SetupShaders();

	// UI
	UI::StartUI();

	// Camera
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);

	Camera::instance().Init(cameraPos, cameraTarget);

	// Init Axis For Scene
	//InitAxis();

	// Object setup
	printf("\nLoading Materials\n");
	// Primitive Objs
	_mGameObjects.emplace("Plane", Utils::LoadObj("models/Primitives/pPlane.obj"));
	_mGameObjects.emplace("Sphere", Utils::LoadObj("models/Primitives/pSphere.obj"));
	//_mGameObjects.emplace("Torus", Utils::LoadObj("resources/models/Primitives/pTorus.obj"));

	// Scene Objs
	_mGameObjects.emplace("Sun", Utils::LoadObj("models/sun.obj"));
	_mGameObjects.emplace("Earth", Utils::LoadObj("models/earth.obj"));
	_mGameObjects.emplace("Cube", Utils::LoadObj("models/Primitives/pCube.obj"));


	// Initialize Objs
	//_mGameObjects["Torus"]->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	//_mGameObjects["Torus"]->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));
	//_mGameObjects["Torus"]->InitAxis();

	_mGameObjects["Cube"]->SetPosition(glm::vec3(-1.5f, -1.0f, 1.0f));
	_mGameObjects["Cube"]->SetRotation(glm::vec3(20.0f, 0.0f, 20.0f));
	_mGameObjects["Cube"]->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));
	_mGameObjects["Cube"]->InitAxis();

	_mGameObjects["Sphere"]->SetPosition(glm::vec3(1.5f, 0.0f, 2.0f));
	_mGameObjects["Sphere"]->SetScale(glm::vec3(0.1f, 0.1f, 0.1f));
	_mGameObjects["Sphere"]->InitAxis();


	_mGameObjects["Plane"]->SetPosition(glm::vec3(0.0f, -1.0f, 0.0f));
	_mGameObjects["Plane"]->SetRotation(glm::vec3(-90.0f, 0.0f, 0.0f));
	_mGameObjects["Plane"]->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));
	_mGameObjects["Plane"]->InitAxis();

	_mGameObjects["Sun"]->SetPosition(glm::vec3(5.0f, 2.0f, 2.0f));
	_mGameObjects["Sun"]->SetScale(glm::vec3(2.0f, 2.0f, 2.0f));
	_mGameObjects["Sun"]->InitAxis();

	_mGameObjects["Earth"]->SetPosition(glm::vec3(0.0f, 1.0f, 0.0f));
	_mGameObjects["Earth"]->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));
	_mGameObjects["Earth"]->InitAxis();

	// Load lua script
	_mScriptHost.Load();

	// Set default to lighting
	_mProg = 1;

	// Physics
	PhysicsStart();

}

void GameScene::SetupShaders()
{
	// Shaders
	_mNumShaders = 5;
	std::vector<std::string> vertShaders = {
		"shaders/axis.vert",
		"shaders/passThru.vert",
		"shaders/basicLighting.vert",
		"shaders/bpLighting.vert",
		"shaders/nmLighting.vert",
	};

	std::vector<std::string> fragShaders = {
		"shaders/axis.frag",
		"shaders/passThru.frag",
		"shaders/basicLighting.frag",
		"shaders/bpLighting.frag",
		"shaders/nmLighting.frag",
	};

	printf("Loading Shaders\n");

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

	// Testing shader value setting
	_mShaders[4]->Use();
	glm::vec4 lightPos = glm::vec4(5.0f, 2.0f, 2.0f, 1.0f);
	_mShaders[4]->SetVec4("lightPos", lightPos);

	// Lights w/ Shader 2
	_mShaders[2]->Use();

	/// Directional Light
	DirectionalLight* directionalLight = new DirectionalLight();
	directionalLight->SetDirection(glm::vec4(-0.2f, -1.0f, -0.3f, 0.0f)); // we define direction FROM the light source so it's pointing down
	_mShaders[2]->SetVec4("lightVec", directionalLight->GetDirection());

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
	Camera::instance().Update(dt);

	//_mGameObjects["Sun"]->SetRotation(_mGameObjects["Sun"]->GetRotation() + glm::vec3(0.0f, 0.1f, 0.0f));
	_mGameObjects["Earth"]->SetRotation(_mGameObjects["Earth"]->GetRotation() + glm::vec3(0.0f, 0.25f * dt, 0.0f));
	//_mGameObjects["Cube"]->SetRotation(_mGameObjects["Cube"]->GetRotation() + glm::vec3(0.0f, 0.25f * dt, 0.0f));
	//_mGameObjects["Torus"]->SetRotation(_mGameObjects["Torus"]->GetRotation() + glm::vec3(0.0f, 0.25f * dt, 0.0f));

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

	// Axis Rendering for Objects
	for (auto& gameOject : _mGameObjects)
	{
		if (gameOject.second->IsAxisEnabled())
		{
			gameOject.second->DrawAxis(_mShaders[0]);
		}
	}


	// Draw Scene Axis
	//DrawAxis(0, &_mShader);

	UI::RenderUI();
}
