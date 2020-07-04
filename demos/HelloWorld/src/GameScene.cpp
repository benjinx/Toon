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
    auto logo = AddGameObject("Logo");
    auto comp = (MeshComponent*)logo->AddComponent(std::make_unique<MeshComponent>());
    comp->SetShader(app->GetShader("passThru"));

    if (comp->Load("models/logo/logo.glb"))
    {
        // Initialize Objs
        logo->SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));
        logo->SetRotation(glm::angleAxis(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    }
}

void GameScene::Update(float dt)
{
    Scene::Update(dt);
}
