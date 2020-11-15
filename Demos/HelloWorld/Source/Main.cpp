#include <cstdio>
#include <Temporality/Module.hpp>
#include <Temporality/Graphics/GraphicsDriver.hpp>

int main(int argc, char ** argv)
{
    Temporality::LoadModule("TemporalityOpenGL");

    auto gfx = Temporality::GetGraphicsDriver();
    if (gfx) {
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