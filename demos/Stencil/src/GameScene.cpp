#include "GameScene.hpp"

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

    app->AddShader("stencil", new Shader({
        "shaders/stencil.vert",
        "shaders/stencil.frag" }));

    // Scene Objs
    _mPlane = AddGameObject("Plane");
    _mPlaneMesh = _mPlane->AddComponent<MeshComponent>(std::make_unique<MeshComponent>());
    _mPlaneMesh->SetShader(app->GetShader("defaultLighting"));

    _mSphere = AddGameObject("Sphere");
    _mSphereMesh = _mSphere->AddComponent<MeshComponent>(std::make_unique<MeshComponent>());
    _mSphereMesh->SetShader(app->GetShader("defaultLighting"));

    _mCube = AddGameObject("Cube");
    _mCubeMesh = _mCube->AddComponent<MeshComponent>(std::make_unique<MeshComponent>());
    _mCubeMesh->SetShader(app->GetShader("defaultLighting"));

    _mTorus = AddGameObject("Torus");
    _mTorusMesh = _mTorus->AddComponent<MeshComponent>(std::make_unique<MeshComponent>());
    _mTorusMesh->SetShader(app->GetShader("defaultLighting"));

    // Scene Objs
    if (_mPlaneMesh->Load("models/Primitives/pPlane.glb"))
    {
        _mPlane->SetPosition(glm::vec3(0.0f, -2.5f, 0.0f));
        _mPlane->SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
        _mPlane->SetScale(glm::vec3(5.0f, 5.0f, 5.0f));
    }

    if (_mSphereMesh->Load("models/Primitives/pSphere.glb"))
    {
        _mSphere->SetPosition(glm::vec3(1.5f, 0.0f, 2.0f));
        _mSphere->SetRotation(glm::vec3(90.0f, 0.0f, 0.0f));
        _mSphere->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
    }

    if (_mCubeMesh->Load("models/Primitives/pCube.glb"))
    {
        _mCube->SetPosition(glm::vec3(-1.5f, -1.0f, 0.0f));
        _mCube->SetRotation(glm::vec3(20.0f, 0.0f, 20.0f));
        _mCube->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));
    }

    if (_mTorusMesh->Load("models/Primitives/pTorus.glb"))
    {
        _mTorus->SetPosition(glm::vec3(1.0f, 1.0f, -1.0f));
        _mTorus->SetRotation(glm::vec3(-90.0f, -90.0f, 0.0f));
        _mTorus->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
    }

    // Setup for Stencil Testing
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
}

void GameScene::Update(float dt)
{
    Scene::Update(dt);
}

void GameScene::Render()
{

    //glStencilMask(0xFF); // each bit is written to the stencil buffer as is 
    //glStencilMask(0x00); // each bit ends up as 0 in the stencil buffer (disabling writes)

    auto app = App::Inst();

    glClear(GL_STENCIL_BUFFER_BIT);

    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);
    _mPlaneMesh->SetShader(app->GetShader("defaultLighting"));
    _mPlane->SetScale(glm::vec3(5.0f));
    //_mPlane->Render();
    //_mPlane->RenderAxis();
    _mSphereMesh->SetShader(app->GetShader("defaultLighting"));
    _mSphere->SetScale(glm::vec3(0.5f));
    _mCubeMesh->SetShader(app->GetShader("defaultLighting"));
    _mCube->SetScale(glm::vec3(1.0f));
    _mTorusMesh->SetShader(app->GetShader("defaultLighting"));
    _mTorus->SetScale(glm::vec3(0.5f));
    Scene::Render();

    // Render the outline
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00);
    glDisable(GL_DEPTH_TEST);
    _mPlaneMesh->SetShader(app->GetShader("stencil"));
    _mPlane->SetScale(glm::vec3(5.05f));
    _mSphereMesh->SetShader(app->GetShader("stencil"));
    _mSphere->SetScale(glm::vec3(0.55f));
    _mCubeMesh->SetShader(app->GetShader("stencil"));
    _mCube->SetScale(glm::vec3(1.05f));
    _mTorusMesh->SetShader(app->GetShader("stencil"));
    _mTorus->SetScale(glm::vec3(0.55f));
    Scene::Render();

    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glEnable(GL_DEPTH_TEST);

}