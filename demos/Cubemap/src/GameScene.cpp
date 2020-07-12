#include "GameScene.hpp"

#include <stb/stb_image.h>

void GameScene::Start()
{
    Scene::Start();

    App* app = App::Inst();

    // Camera
    Camera * camera = (Camera *)AddGameObject("Camera", std::make_unique<Camera>());
    camera->SetPosition(glm::vec3(0.0f, 0.0f, 5.0f));
    app->SetCurrentCamera(camera);

    // Shaders
    printf("\nLoading Shaders\n");

    app->AddShader("reflection", new Shader({
        "shaders/reflection.vert",
        "shaders/reflection.frag" }));

    app->AddShader("refraction", new Shader({
        "shaders/refraction.vert",
        "shaders/refraction.frag" }));

    std::vector<std::string> faces = {
        "images/skyboxes/demo/right.jpg",
        "images/skyboxes/demo/left.jpg",
        "images/skyboxes/demo/top.jpg",
        "images/skyboxes/demo/bottom.jpg",
        "images/skyboxes/demo/front.jpg",
        "images/skyboxes/demo/back.jpg",
    };

    // Call skybox here
    CreateSkybox(faces);

    GameObject* Cube = AddGameObject("Cube");
    MeshComponent* CubeMesh = Cube->AddComponent<MeshComponent>(std::make_unique<MeshComponent>());

    CubeMesh->SetShader(app->GetShader("reflection"));

    if (CubeMesh->Load("models/Primitives/pCube.glb"))
    {
        Cube->SetPosition(glm::vec3(-1.5f, -1.0f, 0.0f));
        //Cube->SetRotation(glm::vec3(20.0f, 0.0f, 20.0f));
        //Cube->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));
    }

    // UI
    DevUI::Start();

    // UI
    DevUI::RegisterOptionsFunc([=]() {
        ImGui::RadioButton("Enable Reflection", (int*)&_mMode, (int)Mode::REFLECTION);
        ImGui::RadioButton("Enable Refraction", (int*)&_mMode, (int)Mode::REFRACTION);

        App* app = App::Inst();

        switch (_mMode)
        {
        case Mode::REFLECTION:
            CubeMesh->SetShader(app->GetShader("reflection"));
            break;
        case Mode::REFRACTION:
            CubeMesh->SetShader(app->GetShader("refraction"));
            break;
        }
    });
}

void GameScene::Update(float dt)
{
    App* app = App::Inst();

    Scene::Update(dt);
}

void GameScene::Render()
{
    App* app = App::Inst();

    glm::vec3 eyePos = app->GetCurrentCamera()->GetPosition();

    auto refection = app->GetShader("reflection");
    refection->Use();
    refection->SetVec3("eyePos", eyePos);

    auto refraction = app->GetShader("refraction");
    refraction->Use();
    refraction->SetVec3("eyePos", eyePos);

    // Use this after using an objects shader 
    // to get the skybox info
    GetSkybox()->PreRender();

    // Render everything else after
    Scene::Render();
}