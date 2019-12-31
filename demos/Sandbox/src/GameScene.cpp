#include "GameScene.hpp"

void GameScene::Start()
{
    Scene::Start();

    LogTest();

    // Shaders
    printf("\nLoading Shaders\n");

    App* app = App::Inst();
    app->AddShader("passThru", new Shader({
        "shaders/passThruColor.vert",
        "shaders/passThruColor.frag" }));

    app->AddShader("normalMapping", new Shader({
        "shaders/normalMapping.vert",
        "shaders/normalMapping.frag" }));

    app->AddShader("lightCasters", new Shader({
        "shaders/lightCasters.vert",
        "shaders/lightCasters.frag" }));

    // Scene Objs
    Load("models/DamagedHelm.glb");
    auto helm = FindGameObject("node_damagedHelmet_-6514");
    helm->SetPosition(glm::vec3(2.0f, 0.0f, 1.0f));
    helm->SetShader(app->GetShader("normalMapping"));

    Load("models/defaultScene2.glb");
    auto cube = FindGameObject("Cube");
    cube->SetPosition(glm::vec3(-1.0f, -1.0f, -1.0f));
    cube->SetShader(app->GetShader("passThru"));

    // Camera
    Camera * camera = (Camera *)AddGameObject("Camera", std::make_unique<Camera>());
    camera->SetPosition(glm::vec3(0.0f, 0.0f, 5.0f));
    App::Inst()->SetCurrentCamera(camera);

    // UI
    DevUI::Start();
}

void GameScene::Update(float dt)
{
    Scene::Update(dt);

    App* app = App::Inst();
    auto norm = app->GetShader("normalMapping");

    norm->Use();
    PointLight* light = (PointLight*)FindGameObject("Light_Orientation");
    norm->SetVec4("lightPos", glm::vec4(light->GetWorldPosition(), 1.0f));

    auto color = app->GetShader("passThru");
    color->Use();
    color->SetVec3("passColor", glm::vec3(0.0f, 0.0f, 1.0f));
}

void GameScene::LogTest()
{
    // Log Test
    printf("\n");
    LogInfo("Info\n");
    LogWarn("Warning\n");
    LogError("Error\n");
    LogPerf("Perf\n");
    LogVerbose("Verb\n");
    LogLoad("Load\n");
}