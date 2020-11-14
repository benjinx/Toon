#include "GameScene.hpp"

void GameScene::Start()
{
    Scene::Start();

    LogTest();

    // Light Source
    auto Light = AddGameObject("Light");
    Light->SetPosition(glm::vec3(0.0f, 0.0f, 2.0f));

    // Shaders
    printf("\nLoading Shaders\n");

    App* app = App::Inst();
    app->AddShader("passThru", new Shader({
        "shaders/passThruColor.vert",
        "shaders/passThruColor.frag" }));

    app->AddShader("normalMapping", new Shader({
        "shaders/normalMapping.vert",
        "shaders/normalMapping.frag" }));

    app->AddShader("advLighting", new Shader({
        "shaders/advLighting.vert",
        "shaders/advLighting.frag" }));
    
    app->AddShader("stencil", new Shader({
        "shaders/stencil.vert",
        "shaders/stencil.frag" }));

    // Scene Objs
     _mHelm = AddGameObject("helm");
     _mHelmMesh = _mHelm->AddComponent<StaticMeshComponent>(std::make_unique<StaticMeshComponent>());
     _mHelmMesh->SetShader(app->GetShader("normalMapping"));

     if (_mHelmMesh->Load("models/DamagedHelm.glb"))
     {
         _mHelm->SetPosition(glm::vec3(2.0f, 0.0f, 1.0f));
         //_mHelm->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
         _mHelm->SetRotation(glm::vec3(0.0f, 9.5f, 9.5f));
     }

    // _mCube = AddGameObject("Cube");
    // _mCubeMesh = _mCube->AddComponent<StaticMeshComponent>(std::make_unique<StaticMeshComponent>());
    // _mCubeMesh->SetShader(app->GetShader("advLighting"));

    // //if (_mCubeMesh->Load("models/Primitives/pCube.glb"))
    // if (_mCubeMesh->Load("models/LowPolyMale_Rigging_AnimationStomp.glb"))
    // {
    //     _mCube->SetPosition(glm::vec3(-2.0f, 0.0f, 1.0f));
    // }

    if (!LoadScene("models/LowPolyMale_Rigging_AnimationStomp2.glb"))
    {
       LogWarn("Could not load scene!");
    }
    
    // if (!LoadScene("models/Donut_Texture_Displacement.glb"))
    // {
    //    LogWarn("Could not load scene!");
    // }

    // Camera
    Camera * camera = (Camera *)AddGameObject("Camera", std::make_unique<Camera>());
    camera->SetPosition(glm::vec3(0.0f, 0.0f, 5.0f));
    App::Inst()->SetCurrentCamera(camera);
}

void GameScene::Update(float dt)
{
    Scene::Update(dt);

    App* app = App::Inst();

    //
    auto norm = app->GetShader("normalMapping");
    glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    norm->Use();
    norm->SetVec3("lightColor", lightColor);
    glm::vec4 lightPos = glm::vec4(FindGameObject("Light")->GetWorldPosition(), 1.0f);
    norm->SetVec4("lightPos", lightPos);

    auto adv = app->GetShader("advLighting");
    adv->Use();
    adv->SetVec3("lightColor", lightColor);
    adv->SetVec4("lightVec", lightPos);
    
    adv->SetVec4("eyePos", glm::vec4(app->GetCurrentCamera()->GetPosition(), 1.0f));
}

void GameScene::Render()
{
    auto app = App::Inst();

    Scene::Render();
}

void GameScene::LogTest()
{
    // Log Test
    printf("\n");
    LogInfo("Info");
    LogWarn("Warning");
    LogError("Error");
    LogPerf("Perf");
    LogVerbose("Verb");
    LogLoad("Load");
}