#include <Toon/Module.hpp>
#include <Toon/OpenGL/OpenGLGraphicsDriver.hpp>

namespace Toon::OpenGL {

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

TOON_MODULE {
    .Name       = "OpenGL",
    .Initialize = ModuleInit,
    .Terminate  = ModuleTerm,
};

} // namespace Toon::OpenGL