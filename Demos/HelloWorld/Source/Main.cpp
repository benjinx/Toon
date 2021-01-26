#include <Temporality/Temporality.hpp>

#include <cstdio>
#include <memory>

using namespace Temporality;

int main(int argc, char ** argv)
{
    //LoadModule("TemporalityOpenGL");
    LoadModule("TemporalityVulkan");
    LoadModule("TemporalitySTBI");
    LoadModule("TemporalityTinyOBJ");

    auto gfx = GetGraphicsDriver();
    if (gfx) {
        // auto shader = gfx->CreateShader();
        // shader->LoadFromFiles({"shaders/passThruColor.vert.glsl", "shaders/passThruColor.frag.glsl"});

        // auto texture = gfx->CreateTexture();
        // texture->LoadFromFile("images/avatars/logo.jpg");

        // CubeMeshData meshdata;
        // auto mesh = gfx->CreateMesh();
        // mesh->Load((MeshData*)&meshdata);

        // Entity e;

        // auto meshComp = e.AddComponent<MeshComponent>(std::unique_ptr<Component>(new MeshComponent()));
        // meshComp->LoadFromFile("models/ball/model.obj");

        // //e.AddComponent());

        RenderContext* rc = new RenderContext();

        gfx->SetWindowTitle("HelloWorld ~ Temporality");
        gfx->SetWindowSize({ 1024, 768 });
        while (IsRunning()) {
            gfx->ProcessEvents();

            //shader->Bind();
            //mesh->Render();
            //e.Render(rc);

            gfx->Render();
        }
    }

    FreeAllModules();

    return 0;
}