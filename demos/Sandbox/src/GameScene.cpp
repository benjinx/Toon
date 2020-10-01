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
    _mHelmMesh = _mHelm->AddComponent<MeshComponent>(std::make_unique<MeshComponent>());
    _mHelmMesh->SetShader(app->GetShader("normalMapping"));

    if (_mHelmMesh->Load("models/DamagedHelm.glb"))
    {
        _mHelm->SetPosition(glm::vec3(2.0f, 0.0f, 1.0f));
        _mHelm->SetRotation(glm::vec3(0.0f, 9.5f, 9.5f));
    }

    _mCube = AddGameObject("Cube");
    _mCubeMesh = _mCube->AddComponent<MeshComponent>(std::make_unique<MeshComponent>());
    _mCubeMesh->SetShader(app->GetShader("advLighting"));

    if (_mCubeMesh->Load("models/Primitives/pCube.glb"))
    {
        _mCube->SetPosition(glm::vec3(-2.0f, 0.0f, 1.0f));
    }

    //if (!LoadScene("models/defaultSceneLights.glb"))
    //{
    //    LogWarn("Could not load scene!");
    //}

    // Camera
    Camera * camera = (Camera *)AddGameObject("Camera", std::make_unique<Camera>());
    camera->SetPosition(glm::vec3(0.0f, 0.0f, 5.0f));
    App::Inst()->SetCurrentCamera(camera);


    ///
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
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

    glClear(GL_STENCIL_BUFFER_BIT);

    glStencilMask(0x00);
    // Render normal stuff here.

    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);
    _mHelmMesh->SetShader(app->GetShader("normalMapping"));
    _mHelm->SetScale(glm::vec3(1.0f));
    _mCubeMesh->SetShader(app->GetShader("advLighting"));
    _mCube->SetScale(glm::vec3(1.0f));
    Scene::Render();

    // Render the outline
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00);
    glDisable(GL_DEPTH_TEST);
    _mHelmMesh->SetShader(app->GetShader("stencil"));
    _mHelm->SetScale(glm::vec3(1.05f));
    _mCubeMesh->SetShader(app->GetShader("stencil"));
    _mCube->SetScale(glm::vec3(1.05f));
    Scene::Render();

    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glEnable(GL_DEPTH_TEST);

    //glStencilMask(0xFF); // each bit is written to the stencil buffer as is 
    //glStencilMask(0x00); // each bit ends up as 0 in the stencil buffer (disabling writes)
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