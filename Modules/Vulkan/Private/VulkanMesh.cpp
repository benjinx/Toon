#include <Temporality/Vulkan/VulkanMesh.hpp>

#include <Temporality/Log.hpp>
#include <Temporality/Vulkan/VulkanGraphicsDriver.hpp>

namespace Temporality::Vulkan {

TEMPORALITY_VULKAN_API
bool VulkanMesh::Load(const std::vector<std::unique_ptr<PrimitiveData>>& data)
{
    for (const auto& primitiveData : data) {
        std::unique_ptr<Primitive> primitive = std::unique_ptr<Primitive>(new VulkanPrimitive());
        if (!primitive->Load(primitiveData)) {
            return false;
        }

        _primitiveList.push_back(std::move(primitive));
    }

    return true;
}

TEMPORALITY_VULKAN_API
void VulkanMesh::Render(RenderContext * ctx)
{
    VulkanRenderContext * vkRenderContext = TEMPORALITY_VULKAN_RENDER_CONTEXT(ctx);
    VkCommandBuffer vkCommandBuffer = vkRenderContext->GetVkCommandBuffer();

    // VulkanPipeline * vkPipeline = TEMPORALITY_VULKAN_PIPELINE(_pipeline.get());
    // vkPipeline->GenerateCommands(vkCommandBuffer);

    for (auto& primitive : _primitiveList) {
        VulkanPrimitive * vkPrimitive = TEMPORALITY_VULKAN_PRIMITIVE(primitive.get());
        vkPrimitive->GenerateCommands(vkCommandBuffer);
    }
}

} // namespace Temporality::Vulkan