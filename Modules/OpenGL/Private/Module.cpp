#include <Temporality/Module.hpp>
#include <Temporality/OpenGL/GraphicsDriver.hpp>

namespace Temporality::OpenGL {

void ModuleInit() {
    SetGraphicsDriver(std::unique_ptr<Temporality::GraphicsDriver>(new GraphicsDriver()));
}

void ModuleTerm() {
    SetGraphicsDriver(nullptr);
}

TEMPORALITY_MODULE() {
    "OpenGL",
    ModuleInit,
    ModuleTerm,
};

} // namespace Temporality::OpenGL