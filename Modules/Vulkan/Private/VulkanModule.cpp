#include <Temporality/Module.hpp>
#include <Temporality/Vulkan/VulkanGraphicsDriver.hpp>

namespace Temporality::Vulkan {

bool ModuleInit() {
    SetGraphicsDriver(std::unique_ptr<GraphicsDriver>(new VulkanGraphicsDriver()));
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
    .Name       = "Vulkan",
    .Initialize = ModuleInit,
    .Terminate  = ModuleTerm,
};

} // namespace Temporality::Vulkan