#include <Toon/Vulkan/VulkanBuffer.hpp>
#include <Toon/Vulkan/VulkanGraphicsDriver.hpp>

namespace Toon::Vulkan {

TOON_VULKAN_API
bool VulkanBuffer::Initialize(size_t size, uint8_t * data, BufferUsage bufferUsage, MemoryUsage memoryUsage)
{
    bool result;
    VkResult vkResult;

    _bufferUsage = bufferUsage;
    _memoryUsage = memoryUsage;
    _size = static_cast<VkDeviceSize>(size);

    if (!data && _memoryUsage == MemoryUsage::GPU) {
        LogError("Attempting to create an empty buffer with MemoryUsage GPU");
        return false;
    }

    VulkanGraphicsDriver * gfx = TOON_VULKAN_GRAPHICS_DRIVER(GetGraphicsDriver());

    auto vkBufferUsage = GetVkBufferUsage(_bufferUsage);
    auto vkMemoryUsage = GetVkMemoryUsage(_memoryUsage);

    if (!vkBufferUsage || !vkMemoryUsage) {
        return false;
    }

    // If we are uploading to a GPU only buffer, use a staging buffer
    if (_memoryUsage == MemoryUsage::GPU) {
        VkBuffer stagingBuffer;
        VmaAllocation stagingAllocation;

        result = gfx->CreateBuffer(&stagingBuffer, &stagingAllocation, _size, 
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VMA_MEMORY_USAGE_CPU_ONLY);

        if (!result) {
            LogError("CreateBuffer() failed, unable to create staging buffer");
            return false;
        }

        void * ptr;
        vkResult = vmaMapMemory(gfx->GetAllocator(), stagingAllocation, &ptr);
        if (vkResult != VK_SUCCESS) {
            LogError("vmaMapMemory() failed");
            return false;
        }

        memcpy(ptr, data, _size);

        vmaUnmapMemory(gfx->GetAllocator(), stagingAllocation);

        result = gfx->CreateBuffer(&_vkBuffer, &_vmaAllocation, _size, 
            VK_BUFFER_USAGE_TRANSFER_DST_BIT | vkBufferUsage.value(),
            VMA_MEMORY_USAGE_GPU_ONLY);
            
        if (!result) {
            LogError("CreateBuffer() failed, unable to create buffer");
            return false;
        }

        result = gfx->CopyBuffer(stagingBuffer, _vkBuffer, _size);

        if (!result) {
            LogError("CopyBuffer() failed, unable to copy staging buffer to buffer");
        }

        vkDestroyBuffer(gfx->GetDevice(), stagingBuffer, nullptr);
        vmaFreeMemory(gfx->GetAllocator(), stagingAllocation);
    }
    else if (_memoryUsage == MemoryUsage::UploadOnce || 
             _memoryUsage == MemoryUsage::UploadOften) {
                 
        result = gfx->CreateBuffer(&_vkBuffer, &_vmaAllocation, _size, 
            vkBufferUsage.value(),
            vkMemoryUsage.value());
            
        if (!result) {
            LogError("CreateBuffer() failed, unable to create buffer");
            return false;
        }

        if (data) {
            void * ptr;
            vkResult = vmaMapMemory(gfx->GetAllocator(), _vmaAllocation, &ptr);
            if (vkResult != VK_SUCCESS) {
                LogError("vmaMapMemory() failed");
                return false;
            }

            memcpy(ptr, data, _size);

            vmaUnmapMemory(gfx->GetAllocator(), _vmaAllocation);
        }
    }
    else {
        LogError("MemoryUsage::Download is not yet supported");
        return false;
    }

    return true;
}

TOON_VULKAN_API
void VulkanBuffer::Terminate()
{
    VulkanGraphicsDriver * gfx = TOON_VULKAN_GRAPHICS_DRIVER(GetGraphicsDriver());

    vkDestroyBuffer(gfx->GetDevice(), _vkBuffer, nullptr);
    vmaFreeMemory(gfx->GetAllocator(), _vmaAllocation);
}

TOON_VULKAN_API
bool VulkanBuffer::ReadFrom(size_t offset, size_t length, uint8_t * data)
{
    VkResult vkResult;

    if (_memoryUsage != MemoryUsage::Download) {
        LogError("Unable to read data from buffer with MemoryUsage: %s",
            MemoryUsageToString(_memoryUsage));
        return false;
    }

    VulkanGraphicsDriver * gfx = TOON_VULKAN_GRAPHICS_DRIVER(GetGraphicsDriver());

    void * ptr;
    vkResult = vmaMapMemory(gfx->GetAllocator(), _vmaAllocation, &ptr);
    if (vkResult != VK_SUCCESS) {
        LogError("vmaMapMemory() failed");
        return false;
    }

    memcpy(data, static_cast<char *>(ptr) + offset, length);

    vmaUnmapMemory(gfx->GetAllocator(), _vmaAllocation);

    return true;
}

TOON_VULKAN_API
bool VulkanBuffer::WriteTo(size_t offset, size_t length, uint8_t * data)
{
    VkResult vkResult;
    
    if (_memoryUsage != MemoryUsage::UploadOnce && _memoryUsage != MemoryUsage::UploadOften) {
        LogError("Unable to write data to buffer with MemoryUsage: %s",
            MemoryUsageToString(_memoryUsage));
        return false;
    }

    VulkanGraphicsDriver * gfx = TOON_VULKAN_GRAPHICS_DRIVER(GetGraphicsDriver());

    void * ptr;
    vkResult = vmaMapMemory(gfx->GetAllocator(), _vmaAllocation, &ptr);
    if (vkResult != VK_SUCCESS) {
        LogError("vmaMapMemory() failed");
        return false;
    }

    memcpy(static_cast<char *>(ptr) + offset, data, length);

    vmaUnmapMemory(gfx->GetAllocator(), _vmaAllocation);

    return true;
}

}; // namespace Toon::Vulkan