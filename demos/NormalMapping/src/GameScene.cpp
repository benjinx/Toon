#include "GameScene.hpp"

#include "UI.hpp"

void GameScene::Start()
{
	// Object setup
	printf("\nLoading Models/Materials\n");

	// Light Source
	_mGameObjects.emplace("Light", Utils::LoadObj("models/Primitives/pSphere.obj"));

	_mGameObjects["Light"]->SetPosition(glm::vec3(-2.0f, -0.5f, 1.0f));
	_mGameObjects["Light"]->SetScale(glm::vec3(0.3f, 0.3f, 0.3f));
	_mGameObjects["Light"]->SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));

	// Initialize Objs
	_mGameObjects.emplace("Earth", Utils::LoadObj("models/earth.obj"));
	_mGameObjects.emplace("Moon", Utils::LoadObj("models/moon.obj"));

	_mGameObjects["Earth"]->SetPosition(glm::vec3(1.0f, 0.0f, 0.0f));
	_mGameObjects["Earth"]->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));
	_mGameObjects["Earth"]->SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));

	_mGameObjects["Moon"]->SetPosition(glm::vec3(-1.0f, 0.0f, 0.0f));
	_mGameObjects["Moon"]->SetScale(glm::vec3(0.2f, 0.2f, 0.2f));
	_mGameObjects["Moon"]->SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));

	// Shaders
	SetupShaders();

	// UI
	DevUI::Start();

	// Camera
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);

	Camera::Inst().Init(cameraPos, cameraTarget);
}

void GameScene::SetupShaders()
{
	// Shaders
	_mNumShaders = 3;
	std::vector<std::string> vertShaders = {
		"shaders/axis.vert",
		"shaders/passThru.vert",
		"shaders/normalMapping.vert",
	};

	std::vector<std::string> fragShaders = {
		"shaders/axis.frag",
		"shaders/passThru.frag",
		"shaders/normalMapping.frag",
	};

	printf("\nLoading Shaders\n");

	for (int i = 0; i < _mNumShaders; i++)
	{
		Shader* shader = new Shader();
		shader->Load(vertShaders[i], fragShaders[i]);
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

	_mShaders[2]->SetVec3("lightColor", lightColor);
	glm::vec4 lightPos = glm::vec4(_mGameObjects["Light"]->GetPosition(), 1.0f);
	_mShaders[2]->SetVec4("lightPos", lightPos);

	// Update Camera
	Camera::Inst().Update(dt);

	// Rotate objects
	_mGameObjects["Earth"]->SetRotation(_mGameObjects["Earth"]->GetRotation() + glm::vec3(0.0f, 0.25f * dt, 0.0f));
	_mGameObjects["Moon"]->SetRotation(_mGameObjects["Moon"]->GetRotation() + glm::vec3(0.0f, 0.5f * dt, 0.0f));

	const auto& earthPos = _mGameObjects["Earth"]->GetPosition();
	const auto& moonPos = _mGameObjects["Moon"]->GetPosition();

	_mAngle += 0.5f * dt;

	if (_mAngle > 360.0f)
		_mAngle = 0.0f;

	float radian = glm::radians(_mAngle);

	float radius = glm::distance(moonPos, earthPos);

	float x = earthPos.x + (radius * cosf(radian));
	float z = earthPos.z + (radius * sinf(radian));

	glm::vec3 newPos = glm::vec3(x, 0.0f, -z);
	
	_mGameObjects["Moon"]->SetPosition(newPos);


	// Update objects
	for (auto& gobj : _mGameObjects)
	{
		gobj.second->Update(dt);
	}
}
