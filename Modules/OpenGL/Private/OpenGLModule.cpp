#include <Temporality/Module.hpp>
#include <Temporality/OpenGL/OpenGLGraphicsDriver.hpp>

namespace Temporality::OpenGL {

void ModuleInit() {
    SetGraphicsDriver(std::unique_ptr<GraphicsDriver>(new OpenGLGraphicsDriver()));
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