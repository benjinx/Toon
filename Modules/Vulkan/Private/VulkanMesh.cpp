#include <Temporality/Vulkan/VulkanMesh.hpp>

#include <Temporality/Log.hpp>
#include <Temporality/Vulkan/VulkanGraphicsDriver.hpp>

namespace Temporality::Vulkan {

TEMPORALITY_VULKAN_API
VulkanMesh::~VulkanMesh()
{
}

TEMPORALITY_VULKAN_API
bool VulkanMesh::Load(const std::vector<std::unique_ptr<PrimitiveData>>& data)
{
    return true;
}

TEMPORALITY_VULKAN_API
void VulkanMesh::GenerateCommands(VkCommandBuffer vkCommandBuffer)
{
    VulkanPipeline * vkPipeline = TEMPORALITY_VULKAN_PIPELINE(_pipeline.get());
    vkPipeline->GenerateCommands(vkCommandBuffer);

    for (auto& primitive : _primitiveList) {
        VulkanPrimitive * vkPrimitive = TEMPORALITY_VULKAN_PRIMITIVE(primitive.get());
        vkPrimitive->GenerateCommands(vkCommandBuffer);
    }
}

} // namespace Temporality::Vulkan