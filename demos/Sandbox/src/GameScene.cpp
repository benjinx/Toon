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
    //Load("models/testSceneLight.glb");
    Load("models/untitled2.glb");
    //Load("models/TestScene.glb");
    Load("models/DamagedHelm.glb");

    // Initialize Objs
    auto helmet = GetGameObject("node_damagedHelmet_-6514");
    helmet->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    helmet->SetRotation(glm::rotate(glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f)));

    auto sun = GetGameObject("Sun");
    auto cube = GetGameObject("Cube");

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

    app->AddShader("lightCasters", new Shader({
        "shaders/lightCasters.vert",
        "shaders/lightCasters.frag"
        }));
    
    // Setup Shaders
    helmet->SetShader(app->GetShader("lightCasters"));
    sun->SetShader(app->GetShader("lightCasters"));
    cube->SetShader(app->GetShader("lightCasters"));

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
    Shader* lightCasters = app->GetShader("lightCasters");

    passThru->Use();
    glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    passThru->SetVec3("passColor", lightColor);

    auto sun = GetGameObject("Sun");

    // Set Light Position
    normalMapping->Use();

    normalMapping->SetVec3("lightColor", lightColor);
    //normalMapping->SetVec4("lightPos", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    normalMapping->SetVec4("lightPos", glm::vec4(sun->GetWorldPosition(), 1.0f));

    glm::vec3 camPos = App::Inst()->GetCurrentCamera()->GetPosition();
    glm::vec4 eyePos = glm::vec4(camPos.x, camPos.y, camPos.z, 1.0f);
    normalMapping->SetVec4("eyePos", eyePos);

    lightCasters->Use();
    lightCasters->SetVec4("eyePos", eyePos);

    if (_mDirLight)
    {
        auto cube = GetGameObject("Cube");
        auto rot = GetGameObject("Sun")->GetRotation();
        lightCasters->SetVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
        //auto newRot = glm::eulerAngles(sunRot) * 2;
        //newRot -= 1;
        //auto norm = glm::normalize(newRot);

        //auto test = glm::vec3(-0.2f, -1.0f, -0.3f) * 2;
        //test -= 1.0f;

        //auto norm = glm::normalize(test);
        //LogInfo("Norm: %f, %f, %f\n", norm.x, norm.y, norm.z);
        auto sunRot = glm::rotate(rot, glm::vec3(0.0f, 0.0f, -1.0f));
        lightCasters->SetBool("lightCheck.Directional", true);
        lightCasters->SetVec4("dirLight.direction", glm::vec4(sunRot.x, sunRot.y, sunRot.z, 0.0f));
        //LogInfo("sunrot %f, %f, %f\n", sunRot.x, sunRot.y, sunRot.z);
    }
    else
        lightCasters->SetBool("lightCheck.Directional", false);

    lightCasters->SetBool("lightCheck.Point", false);
    lightCasters->SetBool("lightCheck.Spot", false);
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