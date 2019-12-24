#include "GameScene.hpp"

void GameScene::Start()
{
    Scene::Start();

    // Camera
    Camera * camera = (Camera *)AddGameObject("Camera", std::make_unique<Camera>());
    camera->SetPosition(glm::vec3(0.0f, 0.0f, 5.0f));
    App::Inst()->SetCurrentCamera(camera);

    // Shaders
    printf("\nLoading Shaders\n");

    App* app = App::Inst();
    app->AddShader("passThru", new Shader({
        "shaders/passThruTexture.vert",
        "shaders/passThruTexture.frag"
        }));

    // Object setup
    printf("\nLoading Models/Materials\n");

    // Scene Objs
    if (Load("models/logo/logo.glb"))
    {
        // Initialize Objs
        auto logo = FindGameObject("Logo");
        logo->SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));
        logo->SetRotation(glm::angleAxis(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
        logo->SetShader(app->GetShader("passThru"));
    }

    // UI
    DevUI::Start();
}

void GameScene::Update(float dt)
{
    Scene::Update(dt);
}
