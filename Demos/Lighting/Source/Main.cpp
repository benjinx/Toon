#include <Toon/Toon.hpp>

#include <Toon/Camera.hpp>
#include <Toon/Entity.hpp>
#include <Toon/GraphicsDriver.hpp>
#include <Toon/Light.hpp>
#include <Toon/Log.hpp>
#include <Toon/Mesh.hpp>
#include <Toon/MeshComponent.hpp>
#include <Toon/Module.hpp>
#include <Toon/RenderContext.hpp>
#include <Toon/Scene.hpp>
#include <Toon/Texture.hpp>
#include <Toon/UpdateContext.hpp>
#include <Toon/Version.hpp>

#include <cstdio>
#include <memory>
#include <thread>

using namespace Toon;

void Run()
{
    // Load Modules
    LoadModule("ToonSTBI");
    LoadModule("ToonTinyOBJ");
    LoadModule("ToonGLTF2");

    // Decide our Graphics driver
    const char * graphicsDriver = getenv("TOON_GRAPHICS_DRIVER");
    
    if (!graphicsDriver) {
        graphicsDriver = "OpenGL";
    }

    if (strcmp(graphicsDriver, "OpenGL") == 0) {
        LoadModule("ToonOpenGL");
    }
    else if (strcmp(graphicsDriver, "DirectX") == 0) {
        LoadModule("ToonDirectX");
    }
    else {
        LoadModule("ToonVulkan");
    }

    // Grab the current graphicsdriver
    auto gfx = GetGraphicsDriver();
    if (!gfx) {
        return;
    }

    // Set our options
    gfx->SetWindowSize({ 1024, 768 });

    // Create our scene
    Scene scene;
    SetCurrentScene(&scene);

    // Create camera
    Camera camera;
    camera.SetAspect(glm::vec2(640.0f, 480.0f));
    camera.SetFOVX(45.0f);
    camera.SetMode(CameraMode::Perspective);
    camera.SetPosition({ 1.5, 1.5, 1.5 });
    camera.SetLookAt({ 0, 0, 0 });

    // Create our shader and load them
    auto shader = gfx->CreateShader();
    if (!shader->LoadFromFiles({
        "Lighting.vert",
        "Lighting.frag",
    })) {
        return;
    }

    // Create our pipeline (we need a pipeline for each shader)
    auto pipeline = gfx->CreatePipeline(shader);

    // Create and load a mesh
    auto mesh = LoadMeshFromFile("DamagedHelmet.glb");
    if (!mesh) {
        return;
    }

    // Set the pipeline for the mesh, we can set any pipeline for any mesh as needed.
    mesh->SetPipeline(pipeline);

    // Create an entity
    auto entity = std::unique_ptr<Entity>(new Entity());
    Entity * tmpEntity = entity.get();

    // Add components to entity
    auto meshComponent = std::unique_ptr<MeshComponent>(new MeshComponent());
    meshComponent->SetMesh(mesh);
    entity->AddComponent(std::move(meshComponent));

    // Add the new entity to the scene
    auto e = scene.AddChild(std::move(entity));

    // Create a render context and transform data
    RenderContext * renderCtx = gfx->GetRenderContext();
    ShaderTransform * shaderTransform = renderCtx->GetShaderTransform();

    // Set our view and proj matrix (in shaders).
    shaderTransform->View = camera.GetView();
    shaderTransform->Projection = camera.GetProjection();

    // Set camera position (in shaders).
    auto globals = renderCtx->GetShaderGlobals();
    globals->CameraPosition = glm::vec4(camera.GetPosition(), 1.0f);

    // Light Source
    Light* light = new Light();
    light->SetPosition(glm::vec3(10.0f, 10.0f, 10.0f));
    light->SetColor(glm::vec3(1.0f, 1.0f, 0.0f));
    light->SetOrientation(glm::quat(0.0f, 0.0f, 0.0f, 1.0f));
    scene.AddChild(std::unique_ptr<Entity>(light));

    // Add our light(s) to our shaders.
    globals->Lights[globals->LightCount].Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    globals->Lights[globals->LightCount].Direction = glm::vec4(GetWorldForward() * light->GetOrientation(), 1.0f);
    globals->Lights[globals->LightCount].Position = glm::vec4(light->GetPosition(), 1.0f);
    globals->LightCount++;

    // Game loop
    Toon::Run([&]() {
        gfx->Render();

        gfx->ProcessEvents();

        //camera.HandleMovement(gfx->GetUpdateContext()->GetFrameSpeedRatio());

        // Set our view and proj matrix (in shaders).
        shaderTransform->View = camera.GetView();
        shaderTransform->Projection = camera.GetProjection();

        auto uctx = gfx->GetUpdateContext();
        e->SetOrientation(e->GetOrientation() * glm::angleAxis(glm::radians(0.25f * uctx->GetFrameSpeedRatio()), GetWorldUp()));

    });
}


int main(int argc, char ** argv)
{
    // Lets log things
    AddLogFile("lastrun.log");

    // Set application info
    SetApplicationName("Lighting");
    SetApplicationVersion({TOON_VERSION_MAJOR, TOON_VERSION_MINOR, TOON_VERSION_PATCH});

    // Initialize our demo
    if (!Initialize(argc, argv)) {
        return 1;
    }

    // Run
    Run();

    // Terminate
    Terminate();

    // Lets not log things anymore
    CloseAllLogFiles();

    return 0;
}

