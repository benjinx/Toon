#include <Temporality/Module.hpp>
#include <Temporality/OpenGL/OpenGLGraphicsDriver.hpp>

namespace Temporality::OpenGL {

bool ModuleInit() {
    SetGraphicsDriver(std::unique_ptr<GraphicsDriver>(new OpenGLGraphicsDriver()));
    if (!GetGraphicsDriver()->Initialize()) {
        SetGraphicsDriver(nullptr);
        return false;
    }

    return true;
}

void ModuleTerm() {
    SetGraphicsDriver(nullptr);
}

TEMPORALITY_MODULE {
    .Name       = "OpenGL",
    .Initialize = ModuleInit,
    .Terminate  = ModuleTerm,
};

} // namespace Temporality::OpenGL