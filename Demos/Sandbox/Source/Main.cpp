#include <Toon/Toon.hpp>
#include <Toon/Module.hpp>

#include <Toon/Camera.hpp>
#include <Toon/Entity.hpp>
#include <Toon/RenderContext.hpp>
#include <Toon/UpdateContext.hpp>
#include <Toon/Component.hpp>
#include <Toon/MeshComponent.hpp>
#include <Toon/Light.hpp>
#include <Toon/Log.hpp>
#include <Toon/Mesh.hpp>
#include <Toon/Scene.hpp>
#include <Toon/Shader.hpp>
#include <Toon/Texture.hpp>
#include <Toon/Util.hpp>
#include <Toon/GraphicsDriver.hpp>
#include <Toon/TextureImporter.hpp>
#include <Toon/MeshImporter.hpp>
#include <Toon/Version.hpp>

#include <cstdio>
#include <memory>
#include <thread>

#include <chrono>

using namespace std::chrono;
using namespace Toon;

void Run()
{
    // Load Modules
    LoadModule("ToonSTBI");
    LoadModule("ToonTinyOBJ");

    // Decide our Graphics driver
    const char * graphicsDriver = getenv("TOON_GRAPHICS_DRIVER");
    
    if (!graphicsDriver) {
        graphicsDriver = "Vulkan";
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

    // Create a render context and transform data
    RenderContext * renderCtx = gfx->GetRenderContext();
    ShaderTransform * shaderTransform = renderCtx->GetShaderTransform();

    // Create camera
    Camera camera;
    camera.SetAspect(glm::vec2(640.0f, 480.0f));
    camera.SetFOVX(45.0f);
    camera.SetMode(CameraMode::Perspective);
    camera.SetPosition({ 3, 3, 3 });
    camera.SetLookAt({ 0, 0, 0 });

    // Light Source
    /*auto Light = scene.AddChild(std::unique_ptr<Light>(new Light()));
    Light->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));*/

    // Create our shader and load them
    auto shader = gfx->CreateShader();
    if (!shader->LoadFromFiles({
        "Toon/DebugNormalColor.vert",
        "Toon/DebugNormalColor.frag",
    })) {
        return;
    }

    // Create our pipeline (we need a pipeline for each shader)
    auto pipeline = gfx->CreatePipeline(shader);

    // Create and load a mesh
    auto mesh = LoadMeshFromFile("models/cube.obj");
    if (!mesh) {
        return;
    }
    
    // Set the pipeline for the mesh, we can set any pipeline for any mesh as needed.
    mesh->SetPipeline(pipeline);

    // Create an entity
    auto entity = std::unique_ptr<Entity>(new Entity());
    Entity * tmpEntity = entity.get();

    // Set position/orientation/scale
    entity->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    entity->SetOrientation(glm::quat(0.0f, 0.0f, 0.0f, 1.0f));
    entity->SetScale(glm::vec3(1.0f));

    // Add components to entity
    auto meshComponent = std::unique_ptr<MeshComponent>(new MeshComponent());
    meshComponent->SetMesh(mesh);
    entity->AddComponent(std::move(meshComponent));

    // How are we handling textures? like this?
    /*auto textureComponent = std::unique_ptr<TextureComponent>(new TextureComponent());
    textureComponent->SetTexture();
    entity->AddComponent(std::move(textureComponent));*/
    


    // Add the new entity to the scene
    scene.AddChild(std::move(entity));

    
    shaderTransform->View = camera.GetView();
    shaderTransform->Projection = camera.GetProjection();

    // Game loop
    while (IsRunning()) {
        gfx->Render();

        gfx->ProcessEvents();

        
        // How do i pass in a uniform for things such as light direction or
        // A color or something

        std::this_thread::sleep_for(16ms);
    }

}


int main(int argc, char ** argv)
{

    // Set application info
    SetApplicationName("Sandbox");
    SetApplicationVersion({TOON_VERSION_MAJOR, TOON_VERSION_MINOR, TOON_VERSION_PATCH});

    // Initialize our demo
    if (!Initialize(argc, argv)) {
        return 1;
    }

    // Run
    Run();

    // Terminate
    Terminate();

    return 0;
}

