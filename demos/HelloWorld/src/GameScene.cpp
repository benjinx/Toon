#include "GameScene.hpp"

void GameScene::Start()
{
    Scene::Start();

    // Object setup
    printf("\nLoading Models/Materials\n");

    // Camera
    auto camera = new Camera();
    AddGameObject("Camera", camera);
    GetGameObject("Camera")->SetPosition(glm::vec3(0.0f, 0.0f, 5.0f));
    App::Inst()->SetCurrentCamera(camera);

    // Scene Objs
    Load("models/logo/logo.glb");

    // Initialize Objs
    auto logo = GetGameObject("Logo");
    logo->SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));
    logo->SetRotation(glm::angleAxis(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));

    // Shaders
    printf("\nLoading Shaders\n");

    App* app = App::Inst();
    app->AddShader("passThru", new Shader({
        "shaders/passThruTexture.vert",
        "shaders/passThruTexture.frag"
        }));

    logo->SetShader(app->GetShader("passThru"));

    // UI
    DevUI::Start();
}

void GameScene::Update(float dt)
{
    Scene::Update(dt);
}
