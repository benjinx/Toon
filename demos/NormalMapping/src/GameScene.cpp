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
	printf("\nLoading Shaders\n");

	Application* app = Application::Inst();
	app->AddShader("passThru", new Shader({
		"shaders/passThru.vert",
		"shaders/passThru.frag" }));

	app->AddShader("normalMapping", new Shader({
		"shaders/normalMapping.vert",
		"shaders/normalMapping.frag" }));

	_mGameObjects["Light"]->SetShader(app->GetShader("passThru"));
	_mGameObjects["Earth"]->SetShader(app->GetShader("normalMapping"));
	_mGameObjects["Moon"]->SetShader(app->GetShader("normalMapping"));

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
	// Get the application for ease.
	Application* app = Application::Inst();

	// Get reference to each shader
	Shader* passThru = app->GetShader("passThru");
	Shader* normalMapping = app->GetShader("normalMapping");

	// Set Shader values

	// Set Light Color
	passThru->Use();
	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	passThru->SetVec3("passColor", lightColor);

	// Set Light Position
	normalMapping->Use();

	normalMapping->SetVec3("lightColor", lightColor);
	glm::vec4 lightPos = glm::vec4(_mGameObjects["Light"]->GetPosition(), 1.0f);
	normalMapping->SetVec4("lightPos", lightPos);

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
