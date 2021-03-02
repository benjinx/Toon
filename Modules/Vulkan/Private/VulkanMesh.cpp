#include <Toon/Vulkan/VulkanMesh.hpp>

#include <Toon/Log.hpp>
#include <Toon/Vulkan/VulkanGraphicsDriver.hpp>

namespace Toon::Vulkan {

TOON_VULKAN_API
bool VulkanMesh::Initialize()
{
    if (!Mesh::Initialize()) {
        return false;
    }

    return true;
}

TOON_VULKAN_API
void VulkanMesh::Terminate()
{
    Destroy();

    Mesh::Terminate();
}

TOON_VULKAN_API
bool VulkanMesh::Create()
{
    VulkanGraphicsDriver * gfx = TOON_VULKAN_GRAPHICS_DRIVER(GetGraphicsDriver());
    if (!gfx->CreateDescriptorSet(&_vkDescriptorSet)) {
        return false;
    }

    VulkanBuffer * vkTransformBuffer = TOON_VULKAN_BUFFER(_shaderTransformBuffer.get());

    VkDescriptorBufferInfo bufferInfo = {
        .buffer = vkTransformBuffer->GetVkBuffer(),
        .offset = 0,
        .range = VK_WHOLE_SIZE,
    };

    VkWriteDescriptorSet writeDescriptorSet = {
        .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
        .dstSet = _vkDescriptorSet,
        .dstBinding = TOON_SHADER_TRANSFORM_BINDING,
        .dstArrayElement = 0,
        .descriptorCount = 1,
        .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .pBufferInfo = &bufferInfo,
    };

    vkUpdateDescriptorSets(gfx->GetDevice(), 1, &writeDescriptorSet, 0, nullptr);

    return true;
}

TOON_VULKAN_API
void VulkanMesh::Destroy()
{
    // Descriptor Sets are automatically freed (I hope)
    _vkDescriptorSet = nullptr;
}

TOON_VULKAN_API
void VulkanMesh::Render(RenderContext * ctx)
{
    Mesh::Render(ctx);

    VulkanGraphicsDriver * gfx = TOON_VULKAN_GRAPHICS_DRIVER(GetGraphicsDriver());

    VulkanRenderContext * vkRenderContext = TOON_VULKAN_RENDER_CONTEXT(ctx);
    VkCommandBuffer vkCommandBuffer = vkRenderContext->GetVkCommandBuffer();

    if (vkCommandBuffer) {
        VulkanPipeline * vkPipeline = TOON_VULKAN_PIPELINE(_pipeline.get());
        vkPipeline->GenerateCommands(vkCommandBuffer);

        vkCmdBindDescriptorSets(vkCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, 
            gfx->GetPipelineLayout(), 0, 1, &_vkDescriptorSet, 0, nullptr);

        for (auto& primitive : _primitiveList) {
            VulkanPrimitive * vkPrimitive = TOON_VULKAN_PRIMITIVE(primitive.get());
            vkPrimitive->GenerateCommands(vkCommandBuffer);
        }
    }
}

} // namespace Toon::Vulkan