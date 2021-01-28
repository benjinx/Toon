#include <Toon/Toon.hpp>

#include <cstdio>
#include <memory>
#include <thread>

#include <chrono>

using namespace std::chrono;

void run()
{
    Toon::LoadModule("ToonSTBI");
    Toon::LoadModule("ToonTinyOBJ");

    const char * graphicsDriver = getenv("TOON_GRAPHICS_DRIVER");
    
    if (!graphicsDriver) {
        graphicsDriver = "Vulkan";
    }

    if (strcmp(graphicsDriver, "OpenGL") == 0) {
        Toon::LoadModule("ToonOpenGL");
    }
    else if (strcmp(graphicsDriver, "DirectX") == 0) {
        Toon::LoadModule("ToonDirectX");
    }
    else {
        Toon::LoadModule("ToonVulkan");
    }

    auto gfx = Toon::GetGraphicsDriver();
    if (!gfx) {
        return;
    }

    Toon::RenderContext * renderCtx = gfx->GetRenderContext();
    Toon::TransformData * transformData = renderCtx->GetTransformData();

    Toon::Camera camera;
    camera.SetAspect(glm::vec2(640.0f, 480.0f));
    camera.SetFOVX(45.0f);
    camera.SetMode(Toon::CameraMode::Perspective);
    camera.SetPosition({ 3, 3, 3 });
    camera.SetLookAt({ 0, 0, 0 });

    auto shader = gfx->CreateShader();
    if (!shader->LoadFromFiles({
        "shaders/passThruColor.vert.glsl",
        "shaders/passThruColor.frag.glsl",
    })) {
        return;
    }

    auto pipeline = gfx->CreatePipeline(shader);

    Toon::Scene scene;
    gfx->SetCurrentScene(&scene);

    auto mesh = Toon::LoadMeshFromFile("models/cube.obj");
    if (!mesh) {
        return;
    }
    
    mesh->SetPipeline(pipeline);

    auto entity = std::unique_ptr<Toon::Entity>(new Toon::Entity());
    Toon::Entity * tmpEntity = entity.get();

    auto meshComponent = std::unique_ptr<Toon::MeshComponent>(new Toon::MeshComponent());
    meshComponent->SetMesh(mesh);
    entity->AddComponent(std::move(meshComponent));

    scene.AddChild(std::move(entity));

    // Entity e;

    // auto meshComp = e.AddComponent<MeshComponent>(std::unique_ptr<Component>(new MeshComponent()));
    // meshComp->LoadFromFile("models/ball/model.obj");

    // //e.AddComponent());

    Toon::RenderContext* rc = new Toon::RenderContext();

    gfx->SetWindowTitle("HelloWorld ~ Toon");
    gfx->SetWindowSize({ 1024, 768 });
    while (Toon::IsRunning()) {
        gfx->Render();

        gfx->ProcessEvents();

        transformData->View = camera.GetView();
        transformData->Projection = camera.GetProjection();
        

        std::this_thread::sleep_for(16ms);
    }

}


int main(int argc, char ** argv)
{

    Toon::SetApplicationName("HelloWorld");
    Toon::SetApplicationVersion({1, 0, 0});


    if (!Toon::Initialize(argc, argv)) {
        return 1;
    }

    run();

    Toon::Terminate();

    return 0;
}

