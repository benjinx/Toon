#include <Toon/Vulkan/VulkanMesh.hpp>

#include <Toon/Log.hpp>
#include <Toon/Vulkan/VulkanGraphicsDriver.hpp>

namespace Toon::Vulkan {

TOON_VULKAN_API
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

TOON_VULKAN_API
void VulkanMesh::Render(RenderContext * ctx)
{
    VulkanRenderContext * vkRenderContext = TOON_VULKAN_RENDER_CONTEXT(ctx);
    VkCommandBuffer vkCommandBuffer = vkRenderContext->GetVkCommandBuffer();

    // VulkanPipeline * vkPipeline = TOON_VULKAN_PIPELINE(_pipeline.get());
    // vkPipeline->GenerateCommands(vkCommandBuffer);

    for (auto& primitive : _primitiveList) {
        VulkanPrimitive * vkPrimitive = TOON_VULKAN_PRIMITIVE(primitive.get());
        vkPrimitive->GenerateCommands(vkCommandBuffer);
    }
}

} // namespace Toon::Vulkan