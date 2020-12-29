#include <Temporality/Module.hpp>
#include <Temporality/SDL2/SDL2GraphicsDriver.hpp>

namespace Temporality::SDL2 {

void ModuleInit() {
    SetGraphicsDriver(std::unique_ptr<GraphicsDriver>(new SDL2GraphicsDriver()));
}

void ModuleTerm() {
    SetGraphicsDriver(nullptr);
}

TEMPORALITY_MODULE {
    .Name       = "SDL2",
    .Initialize = ModuleInit,
    .Terminate  = ModuleTerm,
};

} // namespace Temporality::SDL2