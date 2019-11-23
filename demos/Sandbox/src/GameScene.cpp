#include "GameScene.hpp"

void GameScene::Start()
{
    Scene::Start();

    // Log Test
    printf("\n");
    LogInfo("Info\n");
    LogWarn("Warning\n");
    LogError("Error\n");
    LogPerf("Perf\n");
    LogVerbose("Verb\n");
    LogLoad("Load\n");

    // Camera
    auto camera = new Camera();
    AddGameObject("Camera", camera);
    GetGameObject("Camera")->SetPosition(glm::vec3(0.0f, 0.0f, 5.0f));
    App::Inst()->SetCurrentCamera(camera);

    // Scene Objs
    Load("models/TestScene.glb");
    Load("models/DamagedHelm.glb");

    // Initialize Objs
    auto helmet = GetGameObject("node_damagedHelmet_-6514");
    helmet->SetPosition(glm::vec3(3.0f, 0.0f, 0.0f));
    helmet->SetRotation(glm::angleAxis(glm::radians(60.0f), glm::vec3(1.0f, 1.0f, 0.0f)));

    // Shaders
    printf("\nLoading Shaders\n");

    App* app = App::Inst();
    app->AddShader("passThru", new Shader({
        "shaders/passThruColor.vert",
        "shaders/passThruColor.frag" }));

    app->AddShader("advLighting", new Shader({
        "shaders/advLighting.vert",
        "shaders/advLighting.frag" }));

    app->AddShader("normalMapping", new Shader({
        "shaders/normalMapping.vert",
        "shaders/normalMapping.frag" }));
    
    // Setup Shaders
    helmet->SetShader(app->GetShader("normalMapping"));

    // UI
    DevUI::Start();
    DevUI::RegisterOptionsFunc([this]() {
        ImGui::Checkbox("Enable Directional Light", &_mDirLight);
        ImGui::Checkbox("Enable Point Light", &_mPointLight);
        ImGui::Checkbox("Enable Spot Light", &_mSpotLight);
    });

    // Load lua script
    //_mScriptHost.Load();

    // Physics
    PhysicsStart();

    // Test 2D Mesh
    mesh = Utils::Get2DMesh(
        { 0, 0, app->GetWindow()->GetWidth(), app->GetWindow()->GetHeight() },
        { 0, 1, 1, 0 });

    app->AddShader("ui", new Shader({
        "shaders/ui.vert",
        "shaders/ui.frag" }));


    //test mesh2d
    Shader* ui = app->GetShader("ui");
    ui->Use();
    glm::mat4 proj2d = glm::ortho(0.f, (float)app->GetWindow()->GetWidth(),
        0.f, (float)app->GetWindow()->GetHeight());
    app->GetShader("ui")->SetMat4("proj", proj2d);
}

void GameScene::PhysicsStart()
{
    // Testing Physics System Currently
    //glm::vec3 pos(2.0f, 2.0f, 2.0f);
    //Cube cube;
    //cube.SetPosition(pos);
    //cube.Update();
}

void GameScene::PhysicsUpdate(float dt)
{
    //
    //_mScene.GetGameObjects()["Sphere"]->Update(dt);
}

void GameScene::Update(float dt)
{
    Scene::Update(dt);

    //_mGameObjects["helmet"]->SetRotation(_mGameObjects["helmet"]->GetWorldRotation()
    //    * glm::angleAxis(glm::radians(0.5f) * dt, glm::vec3(0.0f, 0.0f, 1.0f)));
    //_mGameObjects["helmet"]->SetRotation(glm::angleAxis(glm::radians(170.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
    //LogInfo("Scale: %f, %f, %f\n", _mGameObjects["helmet"]->GetScale().x, _mGameObjects["helmet"]->GetScale().y, _mGameObjects["helmet"]->GetScale().z);

    // Get the application for ease.
    App* app = App::Inst();

    // Get reference to each shader
    Shader* passThru = app->GetShader("passThru");
    Shader* normalMapping = app->GetShader("normalMapping");

    passThru->Use();
    glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    passThru->SetVec3("passColor", lightColor);

    // Set Light Position
    normalMapping->Use();

    normalMapping->SetVec3("lightColor", lightColor);
    normalMapping->SetVec4("lightPos", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

    glm::vec3 camPos = App::Inst()->GetCurrentCamera()->GetPosition();
    glm::vec4 eyePos = glm::vec4(camPos.x, camPos.y, camPos.z, 1.0f);
    normalMapping->SetVec4("eyePos", eyePos);
}

void GameScene::Render()
{
    Scene::Render();

    // Render 2D Mesh below

    //glClear(GL_DEPTH_BUFFER_BIT);
    //App* app = App::Inst();
    //Shader* ui = app->GetShader("ui");
    //mesh->Render(ui, glm::mat4(1));
}