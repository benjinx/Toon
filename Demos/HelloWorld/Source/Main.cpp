#include <Temporality/Temporality.hpp>

#include <cstdio>
#include <memory>
#include <thread>

#include <chrono>

using namespace std::chrono;

void run()
{
    Temporality::LoadModule("TemporalitySTBI");
    Temporality::LoadModule("TemporalityTinyOBJ");

    const char * graphicsDriver = getenv("TEMPORALITY_GRAPHICS_DRIVER");
    
    if (!graphicsDriver) {
        graphicsDriver = "Vulkan";
    }

    if (strcmp(graphicsDriver, "OpenGL") == 0) {
        Temporality::LoadModule("TemporalityOpenGL");
    }
    else if (strcmp(graphicsDriver, "DirectX") == 0) {
        Temporality::LoadModule("TemporalityDirectX");
    }
    else {
        Temporality::LoadModule("TemporalityVulkan");
    }

    auto gfx = Temporality::GetGraphicsDriver();
    if (!gfx) {
        return;
    }

    Temporality::RenderContext * renderCtx = gfx->GetRenderContext();
    Temporality::TransformData * transformData = renderCtx->GetTransformData();

    Temporality::Camera camera;
    camera.SetAspect(glm::vec2(640.0f, 480.0f));
    camera.SetFOVX(45.0f);
    camera.SetMode(Temporality::CameraMode::Perspective);
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

    Temporality::Scene scene;
    gfx->SetCurrentScene(&scene);

    auto mesh = Temporality::LoadMeshFromFile("models/cube.obj");
    if (!mesh) {
        return;
    }
    
    mesh->SetPipeline(pipeline);

    auto entity = std::unique_ptr<Temporality::Entity>(new Temporality::Entity());
    Temporality::Entity * tmpEntity = entity.get();

    auto meshComponent = std::unique_ptr<Temporality::MeshComponent>(new Temporality::MeshComponent());
    meshComponent->SetMesh(mesh);
    entity->AddComponent(std::move(meshComponent));

    scene.AddChild(std::move(entity));

    // Entity e;

    // auto meshComp = e.AddComponent<MeshComponent>(std::unique_ptr<Component>(new MeshComponent()));
    // meshComp->LoadFromFile("models/ball/model.obj");

    // //e.AddComponent());

    Temporality::RenderContext* rc = new Temporality::RenderContext();

    gfx->SetWindowTitle("HelloWorld ~ Temporality");
    gfx->SetWindowSize({ 1024, 768 });
    while (Temporality::IsRunning()) {
        gfx->Render();

        gfx->ProcessEvents();

        transformData->View = camera.GetView();
        transformData->Projection = camera.GetProjection();
        

        std::this_thread::sleep_for(16ms);
    }

}


int main(int argc, char ** argv)
{

    Temporality::SetApplicationName("HelloWorld");
    Temporality::SetApplicationVersion({1, 0, 0});


    if (!Temporality::Initialize(argc, argv)) {
        return 1;
    }

    run();

    Temporality::Terminate();

    return 0;
}

