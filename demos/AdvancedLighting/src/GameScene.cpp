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
    auto Light = AddGameObject("Light");
    Light->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));

    // Shaders
    printf("\nLoading Shaders\n");

    App* app = App::Inst();
    app->AddShader("passThru", new Shader({
        "shaders/passThruColor.vert",
        "shaders/passThruColor.frag" }));

    app->AddShader("advLighting", new Shader({
        "shaders/advLighting.vert",
        "shaders/advLighting.frag" }));

    // Initialize Objects
    auto Plane = AddGameObject("Plane");
    auto PlaneMesh = (MeshComponent*)Plane->AddComponent(std::make_unique<MeshComponent>());
    PlaneMesh->SetShader(app->GetShader("advLighting"));

    auto Sphere = AddGameObject("Sphere");
    auto SphereMesh = (MeshComponent*)Sphere->AddComponent(std::make_unique<MeshComponent>());
    SphereMesh->SetShader(app->GetShader("advLighting"));

    auto Cube = AddGameObject("Cube");
    auto CubeMesh = (MeshComponent*)Cube->AddComponent(std::make_unique<MeshComponent>());
    CubeMesh->SetShader(app->GetShader("advLighting"));

    auto Torus = AddGameObject("Torus");
    auto TorusMesh = (MeshComponent*)Torus->AddComponent(std::make_unique<MeshComponent>());
    TorusMesh->SetShader(app->GetShader("advLighting"));

    // Scene Objs
    if (PlaneMesh->Load("models/Primitives/pPlane.glb"))
    {
        Plane->SetPosition(glm::vec3(0.0f, -2.5f, 0.0f));
        Plane->SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
        Plane->SetScale(glm::vec3(5.0f, 5.0f, 5.0f));
    }

    if (SphereMesh->Load("models/Primitives/pSphere.glb"))
    {
        Sphere->SetPosition(glm::vec3(1.5f, 0.0f, 2.0f));
        Sphere->SetRotation(glm::vec3(90.0f, 0.0f, 0.0f));
        Sphere->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
    }

    if (CubeMesh->Load("models/Primitives/pCube.glb"))
    {
        Cube->SetPosition(glm::vec3(-1.5f, -1.0f, 0.0f));
        Cube->SetRotation(glm::vec3(20.0f, 0.0f, 20.0f));
        Cube->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));
    }

    if (TorusMesh->Load("models/Primitives/pTorus.glb"))
    {
        Torus->SetPosition(glm::vec3(1.0f, 1.0f, -1.0f));
        Torus->SetRotation(glm::vec3(-90.0f, -90.0f, 0.0f));
        Torus->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
    }
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
    glm::vec4 lightPos = glm::vec4(FindGameObject("Light")->GetPosition(), 1.0f);
    advLighting->SetVec4("lightVec", lightPos);

    glm::vec3 camPos = App::Inst()->GetCurrentCamera()->GetPosition();
    glm::vec4 eyePos = glm::vec4(camPos.x, camPos.y, camPos.z, 1.0f);
    advLighting->SetVec4("eyePos", eyePos);

    
    // Rotate objects
    FindGameObject("Sphere")->SetRotation(FindGameObject("Sphere")->GetWorldRotation()
        * glm::angleAxis(glm::radians(-0.25f) * dt, glm::vec3(0.0f, 0.0f, 1.0f)));
    FindGameObject("Cube")->SetRotation(FindGameObject("Cube")->GetWorldRotation()
        * glm::angleAxis(glm::radians(-0.25f) * dt, glm::vec3(0.0f, 1.0f, 0.0f)));
    FindGameObject("Torus")->SetRotation(FindGameObject("Torus")->GetWorldRotation()
        * glm::angleAxis(glm::radians(-0.25f) * dt, glm::vec3(0.0f, 1.0f, 0.0f)));
}
