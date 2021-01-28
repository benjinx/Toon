#include <Toon/Module.hpp>
#include <Toon/Vulkan/VulkanGraphicsDriver.hpp>

namespace Toon::Vulkan {

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

TOON_MODULE {
    .Name       = "Vulkan",
    .Initialize = ModuleInit,
    .Terminate  = ModuleTerm,
};

} // namespace Toon::Vulkan