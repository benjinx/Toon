#include <Toon/Vulkan/VulkanPrimitive.hpp>

#include <Toon/Vulkan/VulkanGraphicsDriver.hpp>

namespace Toon::Vulkan {

TOON_VULKAN_API
void VulkanPrimitive::Terminate()
{
    if (_vertexBuffer) {
        _vertexBuffer->Terminate();
    }

    if (_indexBuffer) {
        _indexBuffer->Terminate();
    }
}

bool VulkanPrimitive::Load(const std::unique_ptr<PrimitiveData>& data)
{
    bool result;

    const auto& indexList = data->GetIndexList();
    const auto& vertexList = data->GetVertexList();

    _vertexBuffer.reset(new VulkanBuffer());

    result = _vertexBuffer->Initialize(
        vertexList.size() * sizeof(Vertex),
        reinterpret_cast<uint8_t *>(vertexList.data()),
        BufferUsage::Vertex, MemoryUsage::GPU);

    if (!result) {
        return false;
    }

    if (indexList.empty()) {
        _indexed = false;
        _count = vertexList.size();
    }
    else {
        ToonLogFatal("UNTESTED");

        _indexed = true;
        _count = indexList.size();

        _indexBuffer.reset(new VulkanBuffer());

        result = _indexBuffer->Initialize(
            indexList.size() * sizeof(uint32_t), 
            reinterpret_cast<uint8_t *>(indexList.data()), 
            BufferUsage::Index, MemoryUsage::GPU);
        
        if (!result) {
            return false;
        }
    }

    return true;
}

void VulkanPrimitive::GenerateCommands(VkCommandBuffer vkCommandBuffer)
{
    // TODO: Test moving index buffer inside of other if _indexed check
    if (_indexed) {
        VulkanBuffer * vkIndexBuffer = TOON_VULKAN_BUFFER(_indexBuffer.get());
        vkCmdBindIndexBuffer(vkCommandBuffer, vkIndexBuffer->GetVkBuffer(), 0, VK_INDEX_TYPE_UINT32);
    }

    VulkanBuffer * vkVertexBuffer = TOON_VULKAN_BUFFER(_vertexBuffer.get());
    VkBuffer vertexBuffers[] = { vkVertexBuffer->GetVkBuffer() };
    
    VkDeviceSize offsets[] = { 0 };
    vkCmdBindVertexBuffers(vkCommandBuffer, 0, 1, vertexBuffers, offsets);
    
    if (_indexed) {
        vkCmdDrawIndexed(vkCommandBuffer, _count, 1, 0, 0, 0);
    }
    else {
        vkCmdDraw(vkCommandBuffer, _count, 1, 0, 0);
    }
}

} // namespace Toon::Vulkan
