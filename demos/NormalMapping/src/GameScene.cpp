#include "GameScene.hpp"

#include "UI.hpp"

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
	_mGameObjects.emplace("Light", new GameObject("/models/Primitives/pSphere.glb"));

	_mGameObjects["Light"]->SetPosition(glm::vec3(-2.0f, -0.5f, 1.0f));
	_mGameObjects["Light"]->SetScale(glm::vec3(0.3f, 0.3f, 0.3f));
	_mGameObjects["Light"]->SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));

	// Initialize Objs
	_mGameObjects.emplace("Earth", new GameObject("/models/earth.glb"));
	_mGameObjects.emplace("Moon", new GameObject("/models/moon.glb"));
	_mGameObjects.emplace("Mars", new GameObject("/models/mars.glb"));

	_mGameObjects["Earth"]->SetPosition(glm::vec3(1.0f, 0.0f, 0.0f));
	_mGameObjects["Earth"]->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));

	_mGameObjects["Moon"]->SetPosition(glm::vec3(-1.0f, 0.0f, 0.0f));
	_mGameObjects["Moon"]->SetScale(glm::vec3(0.2f, 0.2f, 0.2f));

	_mGameObjects["Mars"]->SetPosition(glm::vec3(10.0f, 0.0f, 0.0f));
	_mGameObjects["Mars"]->SetScale(glm::vec3(1.2f, 1.2f, 1.2f));

	// Shaders
	printf("\nLoading Shaders\n");

	App* app = App::Inst();
	app->AddShader("passThru", new Shader({
		"shaders/passThruColor.vert",
		"shaders/passThruColor.frag" }));

	app->AddShader("normalMapping", new Shader({
		"shaders/normalMapping.vert",
		"shaders/normalMapping.frag" }));

	_mGameObjects["Light"]->SetShader(app->GetShader("passThru"));
	_mGameObjects["Earth"]->SetShader(app->GetShader("normalMapping"));
	_mGameObjects["Moon"]->SetShader(app->GetShader("normalMapping"));
	_mGameObjects["Mars"]->SetShader(app->GetShader("normalMapping"));

	// UI
	DevUI::Start();

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

	glm::vec3 camPos = App::Inst()->GetCurrentCamera()->GetPosition();
	glm::vec4 eyePos = glm::vec4(camPos.x, camPos.y, camPos.z, 1.0f);
	normalMapping->SetVec4("eyePos", eyePos);

	// Rotate objects
	_mGameObjects["Earth"]->SetRotation(_mGameObjects["Earth"]->GetRotation()
		* glm::angleAxis(glm::radians(-0.25f) * dt, glm::vec3(0.0f, 1.0f, 0.0f)));
	
	_mGameObjects["Moon"]->SetRotation(_mGameObjects["Moon"]->GetRotation() 
		* glm::angleAxis(glm::radians(-0.5f) * dt, glm::vec3(0.0f, 1.0f, 0.0f)));

	const auto& earthPos = _mGameObjects["Earth"]->GetPosition();
	const auto& moonPos = _mGameObjects["Moon"]->GetPosition();
	const auto& marsPos = _mGameObjects["Mars"]->GetPosition();

	_mAngle += 0.5f * dt;

	if (_mAngle > 360.0f)
		_mAngle = 0.0f;

	float radian = glm::radians(_mAngle);

	float radius = glm::distance(moonPos, earthPos);

	float x = earthPos.x + (radius * cosf(radian));
	float z = earthPos.z + (radius * sinf(radian));

	glm::vec3 newPos = glm::vec3(x, 0.0f, -z);
	
	_mGameObjects["Moon"]->SetPosition(newPos);


	_mMarsAngle += 0.5f * dt;

	if (_mMarsAngle > 360.0f)
		_mMarsAngle = 0.0f;

	float marsRadian = glm::radians(_mMarsAngle);

	float marsRadius = glm::distance(marsPos, glm::vec3(lightPos.x, lightPos.y, lightPos.z));

	float marsX = lightPos.x + (marsRadius * cosf(marsRadian));
	float marsZ = lightPos.z + (marsRadius * sinf(marsRadian));

	glm::vec3 newMarsPos = glm::vec3(marsX, 0.0f, -marsZ);

	//_mGameObjects["Mars"]->SetPosition(newMarsPos);
}
