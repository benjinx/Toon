#include <Toon/Toon.hpp>

#include <cstdio>
#include <memory>
#include <thread>

#include <chrono>

using namespace std::chrono;

void Run()
{
    // Load Modules
    Toon::LoadModule("ToonSTBI");
    Toon::LoadModule("ToonTinyOBJ");


    // Decide our Graphics driver
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

    // Grab the current graphicsdriver
    auto gfx = Toon::GetGraphicsDriver();
    if (!gfx) {
        return;
    }

    // Set our options
    gfx->SetWindowSize({ 1024, 768 });

    // Create our scene
    Toon::Scene scene;
    gfx->SetCurrentScene(&scene);

    // Create a render context and transform data
    Toon::RenderContext * renderCtx = gfx->GetRenderContext();
    Toon::TransformData * transformData = renderCtx->GetTransformData();

    // Create camera
    Toon::Camera camera;
    camera.SetAspect(glm::vec2(640.0f, 480.0f));
    camera.SetFOVX(45.0f);
    camera.SetMode(Toon::CameraMode::Perspective);
    camera.SetPosition({ 3, 3, 3 });
    camera.SetLookAt({ 0, 0, 0 });

    // Light Source
    /*auto Light = scene.AddChild(std::unique_ptr<Toon::Light>(new Toon::Light()));
    Light->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));*/

    // Create our shader and load them
    auto shader = gfx->CreateShader();
    if (!shader->LoadFromFiles({
        "shaders/passThruColor.vert.glsl",
        "shaders/passThruColor.frag.glsl",
    })) {
        return;
    }

    // Create our pipeline (we need a pipeline for each shader)
    auto pipeline = gfx->CreatePipeline(shader);

    // Create and load a mesh
    auto mesh = Toon::LoadMeshFromFile("models/cube.obj");
    if (!mesh) {
        return;
    }
    
    // Set the pipeline for the mesh, we can set any pipeline for any mesh as needed.
    mesh->SetPipeline(pipeline);

    // Create an entity
    auto entity = std::unique_ptr<Toon::Entity>(new Toon::Entity());
    Toon::Entity * tmpEntity = entity.get();

    // Set position/orientation/scale
    entity->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    entity->SetOrientation(glm::quat(0.0f, 0.0f, 0.0f, 1.0f));
    entity->SetScale(glm::vec3(1.0f));

    // Add components to entity
    auto meshComponent = std::unique_ptr<Toon::MeshComponent>(new Toon::MeshComponent());
    meshComponent->SetMesh(mesh);
    entity->AddComponent(std::move(meshComponent));

    // How are we handling textures? like this?
    /*auto textureComponent = std::unique_ptr<Toon::TextureComponent>(new Toon::TextureComponent());
    textureComponent->SetTexture();
    entity->AddComponent(std::move(textureComponent));*/
    


    // Add the new entity to the scene
    scene.AddChild(std::move(entity));

    // Game loop
    while (Toon::IsRunning()) {
        gfx->Render();

        gfx->ProcessEvents();

        transformData->View = camera.GetView();
        transformData->Projection = camera.GetProjection();
        
        // How do i pass in a uniform for things such as light direction or
        // A color or something

        std::this_thread::sleep_for(16ms);
    }

}


int main(int argc, char ** argv)
{

    // Set application info
    Toon::SetApplicationName("HelloWorld");
    Toon::SetApplicationVersion({1, 0, 0});

    // Initialize our demo
    if (!Toon::Initialize(argc, argv)) {
        return 1;
    }

    // Run
    Run();

    // Terminate
    Toon::Terminate();

    return 0;
}

