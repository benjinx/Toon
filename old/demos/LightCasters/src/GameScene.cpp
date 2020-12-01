#include "GameScene.hpp"

#include <Light.hpp>

#include <imgui/imgui.h>

void GameScene::Start()
{
    Scene::Start();

    // Object setup
    printf("\nLoading Models/Materials\n");

    // Camera
    Camera * camera = (Camera *)AddEntity("Camera", std::make_unique<Camera>());
    camera->SetPosition(glm::vec3(0.0f, 0.0f, 5.0f));
    App::Inst()->SetCurrentCamera(camera);

    // Shaders
    printf("\nLoading Shaders\n");

    App* app = App::Inst();
    app->AddShader("passThru", new Shader({
        "shaders/passThruColor.vert",
        "shaders/passThruColor.frag" }));

    app->AddShader("lightCasters", new Shader({
        "shaders/lightCasters.vert",
        "shaders/lightCasters.frag" }));

    // Initialize Objs
    auto Plane = AddEntity("Plane");
    StaticMeshComponent* PlaneMesh = Plane->AddComponent<StaticMeshComponent>(std::make_unique<StaticMeshComponent>());
    PlaneMesh->SetShader(app->GetShader("lightCasters"));

    auto Sphere = AddEntity("Sphere");
    StaticMeshComponent* SphereMesh = Sphere->AddComponent<StaticMeshComponent>(std::make_unique<StaticMeshComponent>());
    SphereMesh->SetShader(app->GetShader("lightCasters"));

    auto Cube = AddEntity("Cube");
    StaticMeshComponent* CubeMesh = Cube->AddComponent<StaticMeshComponent>(std::make_unique<StaticMeshComponent>());
    CubeMesh->SetShader(app->GetShader("lightCasters"));

    auto Torus = AddEntity("Torus");
    StaticMeshComponent* TorusMesh = Torus->AddComponent<StaticMeshComponent>(std::make_unique<StaticMeshComponent>());
    TorusMesh->SetShader(app->GetShader("lightCasters"));

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
    
    // Lights

    auto dirLight = AddEntity("dLight", std::make_unique<DirectionalLight>());
    dirLight->SetPosition(glm::vec3(0.0f));
    dirLight->SetRotation(glm::vec3(-0.2f, -1.0f, -0.3f));

    auto pLight = AddEntity("pLight",
        std::make_unique<PointLight>(
            glm::vec3(0.0f, 0.0f, 0.0f),
            1.0f,
            0.09f,
            0.032f
        ));
    
    auto sLight = AddEntity("sLight",
        std::make_unique<SpotLight>(
            App::Inst()->GetCurrentCamera()->GetPosition(),
            glm::cos(glm::radians(12.5f)),
            glm::cos(glm::radians(17.5f))
        ));

    // UI
    DevUI::RegisterOptionsFunc([this]() {
        ImGui::Checkbox("Enable Directional Light", &_mDirLight);
        ImGui::Checkbox("Enable Point Light", &_mPointLight);
        ImGui::Checkbox("Enable Spot Light", &_mSpotLight);
    });
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
    if (_mDirLight)
    {
        lightCasters->SetBool("lightCheck.Directional", true);
                
        glm::vec3 dir = FindEntity("dLight")->GetWorldRotation() * Utils::GetWorldForward();
        lightCasters->SetVec4("dirLight.direction", glm::vec4(dir, 0.0f));
    }
    else
        lightCasters->SetBool("lightCheck.Directional", false);

    // Point Lighting
    if (_mPointLight)
    {
        auto pLight = (PointLight*)FindEntity("pLight");

        lightCasters->SetBool("lightCheck.Point", true);
        lightCasters->SetFloat("pointLight.constant", pLight->GetConstant());
        lightCasters->SetFloat("pointLight.linear", pLight->GetLinear());
        lightCasters->SetFloat("pointLight.quadratic", pLight->GetQuadratic());

        lightCasters->SetVec4("pointLight.position", glm::vec4(pLight->GetPosition(), 1.0f));
    }
    else
        lightCasters->SetBool("lightCheck.Point", false);

    // Spotlight Lighting
    if (_mSpotLight)
    {
        auto sLight = (SpotLight*)FindEntity("sLight");

        lightCasters->SetBool("lightCheck.Spot", true);
        lightCasters->SetVec3("spotlight.position", sLight->GetPosition());

        // Change 0.0f to 1.0f to just enable spotlight
        glm::vec4 camFront = glm::vec4(App::Inst()->GetCurrentCamera()->GetForward(), 1.0f);
        lightCasters->SetVec4("spotlight.direction", camFront);

        lightCasters->SetFloat("spotlight.cutoff", sLight->GetCutOff());
        lightCasters->SetFloat("spotlight.outerCutoff", sLight->GetOuterCutOff());
    }
    else
        lightCasters->SetBool("lightCheck.Spot", false);

    // Rotate objects
    FindEntity("Sphere")->SetRotation(FindEntity("Sphere")->GetWorldRotation()
        * glm::angleAxis(glm::radians(-0.25f) * dt, glm::vec3(1.0f, 0.0f, 0.0f)));
    FindEntity("Cube")->SetRotation(FindEntity("Cube")->GetWorldRotation()
        * glm::angleAxis(glm::radians(-0.25f) * dt, glm::vec3(0.0f, 1.0f, 0.0f)));
    FindEntity("Torus")->SetRotation(FindEntity("Torus")->GetWorldRotation()
        * glm::angleAxis(glm::radians(-0.25f) * dt, glm::vec3(0.0f, 1.0f, 1.0f)));
}
