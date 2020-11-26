#include <cstdio>
#include <Temporality/Module.hpp>
#include <Temporality/Graphics/GraphicsDriver.hpp>
#include <Temporality/Graphics/TextureImporter.hpp>
#include <Temporality/Log.hpp>

int main(int argc, char ** argv)
{
    Temporality::LoadModule("TemporalityOpenGL");
    Temporality::LoadModule("TemporalitySTBI");

    auto gfx = Temporality::GetGraphicsDriver();
    if (gfx) {
        auto shader = gfx->CreateShader();
        shader->LoadFromFiles({"shaders/skybox.vert.glsl", "shaders/skybox.frag.glsl"});

        auto texture = gfx->CreateTexture();
        texture->LoadFromFile("images/avatars/logo.jpg");

        auto mesh = gfx->CreateMesh();
        //mesh->Load();

        //gfx->SetWindowTitle("HelloWorld");
        gfx->SetWindowSize({ 1024, 768 });
        for (int i = 0; i < 100; ++i) {
            gfx->ProcessEvents();
            gfx->SwapBuffers();
        }
    }

    Temporality::FreeModules();

    return 0;
}