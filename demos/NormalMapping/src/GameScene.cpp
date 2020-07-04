#include "GameScene.hpp"

void GameScene::Start()
{
    Scene::Start();

    // Object setup
    printf("\nLoading Models/Materials\n");

    // Camera
    Camera * camera = (Camera *)AddGameObject("Camera", std::make_unique<Camera>());
    camera->SetPosition(glm::vec3(0.0f, 0.0f, 5.0f));
    App::Inst()->SetCurrentCamera(camera);

    // Light Source
    auto Light = AddGameObject();
    Light->SetName("Light");
    Light->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));

    // Shaders
    printf("\nLoading Shaders\n");

    App* app = App::Inst();
    app->AddShader("passThru", new Shader({
        "shaders/passThruColor.vert",
        "shaders/passThruColor.frag" }));

    app->AddShader("normalMapping", new Shader({
        "shaders/normalMapping.vert",
        "shaders/normalMapping.frag" }));

    // Initialize Objs
    auto Earth = AddGameObject("Earth");
    auto EarthMesh = (MeshComponent*)Earth->AddComponent(std::make_unique<MeshComponent>());
    EarthMesh->SetShader(app->GetShader("normalMapping"));

    auto Moon = AddGameObject("Moon");
    auto MoonMesh = (MeshComponent*)Moon->AddComponent(std::make_unique<MeshComponent>());
    MoonMesh->SetShader(app->GetShader("normalMapping"));

    auto Mars = AddGameObject("Mars");
    auto MarsMesh = (MeshComponent*)Mars->AddComponent(std::make_unique<MeshComponent>());
    MarsMesh->SetShader(app->GetShader("normalMapping"));

    // Scene Objs
    if (EarthMesh->Load("models/earth.glb"))
    {
        Earth->SetPosition(glm::vec3(2.0f, 0.0f, 0.0f));
        Earth->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));
        Earth->SetRotation(glm::vec3(0.0f, 0.0f, 9.5f));
    }

    if (MoonMesh->Load("models/moon.glb"))
    {
        Moon->SetPosition(glm::vec3(-1.0f, 0.0f, 0.0f));
        Moon->SetScale(glm::vec3(0.2f, 0.2f, 0.2f));
    }

    if (MarsMesh->Load("models/mars.glb"))
    {
        Mars->SetPosition(glm::vec3(10.0f, 0.0f, 0.0f));
        Mars->SetScale(glm::vec3(1.2f, 1.2f, 1.2f));
    }
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
    glm::vec4 lightPos = glm::vec4(FindGameObject("Light")->GetPosition(), 1.0f);
    normalMapping->SetVec4("lightPos", lightPos);

    glm::vec3 camPos = App::Inst()->GetCurrentCamera()->GetPosition();
    glm::vec4 eyePos = glm::vec4(camPos.x, camPos.y, camPos.z, 1.0f);
    normalMapping->SetVec4("eyePos", eyePos);

    // Rotate objects
    FindGameObject("Earth")->SetRotation(FindGameObject("Earth")->GetWorldRotation()
        * glm::angleAxis(glm::radians(-0.25f) * dt, glm::vec3(0.0f, 1.0f, 0.0f)));
    
    FindGameObject("Moon")->SetRotation(FindGameObject("Moon")->GetWorldRotation()
        * glm::angleAxis(glm::radians(-0.5f) * dt, glm::vec3(0.0f, 1.0f, 0.0f)));

    const auto& earthPos = FindGameObject("Earth")->GetPosition();
    const auto& moonPos = FindGameObject("Moon")->GetPosition();
    const auto& marsPos = FindGameObject("Mars")->GetPosition();

    _mAngle += 0.5f * dt;

    if (_mAngle > 360.0f)
        _mAngle = 0.0f;

    float radian = glm::radians(_mAngle);

    float radius = glm::distance(moonPos, earthPos);

    float x = earthPos.x + (radius * cosf(radian));
    float z = earthPos.z + (radius * sinf(radian));

    glm::vec3 newPos = glm::vec3(x, 0.0f, -z);
    
    FindGameObject("Moon")->SetPosition(newPos);
    
    _mMarsAngle += 0.5f * dt;

    if (_mMarsAngle > 360.0f)
        _mMarsAngle = 0.0f;

    float marsRadian = glm::radians(_mMarsAngle);

    float marsRadius = glm::distance(marsPos, glm::vec3(lightPos.x, lightPos.y, lightPos.z));

    float marsX = lightPos.x + (marsRadius * cosf(marsRadian));
    float marsZ = lightPos.z + (marsRadius * sinf(marsRadian));

    glm::vec3 newMarsPos = glm::vec3(marsX, 0.0f, -marsZ);

    //GetGameObject("Mars")->SetPosition(newMarsPos);
}
