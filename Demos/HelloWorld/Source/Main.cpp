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

void run()
{
    LoadModule("ToonSTBI");
    LoadModule("ToonTinyOBJ");

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

    auto gfx = GetGraphicsDriver();
    if (!gfx) {
        return;
    }

    RenderContext * renderCtx = gfx->GetRenderContext();
    ShaderTransform * shaderTransform = renderCtx->GetShaderTransform();

    Camera camera;
    camera.SetAspect(glm::vec2(640.0f, 480.0f));
    camera.SetFOVX(45.0f);
    camera.SetMode(CameraMode::Perspective);
    camera.SetPosition({ 3, 3, 3 });
    camera.SetLookAt({ 0, 0, 0 });

    auto shader = gfx->CreateShader();
    if (!shader->LoadFromFiles({
        "passThruColor.vert.glsl",
        "passThruColor.frag.glsl",
    })) {
        return;
    }

    auto pipeline = gfx->CreatePipeline(shader);

    Scene scene;
    SetCurrentScene(&scene);

    auto mesh = LoadMeshFromFile("models/cube.obj");
    if (!mesh) {
        return;
    }
    
    mesh->SetPipeline(pipeline);

    auto entity = std::unique_ptr<Entity>(new Entity());
    Entity * tmpEntity = entity.get();

    auto meshComponent = std::unique_ptr<MeshComponent>(new MeshComponent());
    meshComponent->SetMesh(mesh);
    entity->AddComponent(std::move(meshComponent));

    scene.AddChild(std::move(entity));

    // Entity e;

    // auto meshComp = e.AddComponent<MeshComponent>(std::unique_ptr<Component>(new MeshComponent()));
    // meshComp->LoadFromFile("models/ball/model.obj");

    // //e.AddComponent());

    shaderTransform->View = camera.GetView();
    shaderTransform->Projection = camera.GetProjection();

    RenderContext* rc = new RenderContext();

    gfx->SetWindowTitle("HelloWorld ~ Toon");
    gfx->SetWindowSize({ 1024, 768 });
    while (IsRunning()) {
        gfx->Render();

        gfx->ProcessEvents();


        

        std::this_thread::sleep_for(16ms);
    }

}


int main(int argc, char ** argv)
{

    SetApplicationName("HelloWorld");
    SetApplicationVersion({1, 0, 0});


    if (!Initialize(argc, argv)) {
        return 1;
    }

    run();

    Terminate();

    return 0;
}

