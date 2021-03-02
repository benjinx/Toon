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
        ToonLogError("Attempting to create an empty buffer with MemoryUsage GPU");
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
        VkBufferCreateInfo stagingBufferCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .size = _size,
            .usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        };

        VmaAllocationCreateInfo stagingAllocationCreateInfo = {
            .flags = VMA_ALLOCATION_CREATE_MAPPED_BIT,
            .usage = VMA_MEMORY_USAGE_CPU_ONLY,
        };

        VkBuffer stagingBuffer;
        VmaAllocation stagingAllocation;
        VmaAllocationInfo stagingAllocationInfo;
        
        vkResult = vmaCreateBuffer(
            gfx->GetAllocator(),
            &stagingBufferCreateInfo,
            &stagingAllocationCreateInfo,
            &stagingBuffer,
            &stagingAllocation,
            &stagingAllocationInfo
        );

        if (vkResult != VK_SUCCESS) {
            ToonLogError("vmaCreateBuffer() failed, unable to create staging buffer");
            return false;
        }

        memcpy(stagingAllocationInfo.pMappedData, data, _size);

        VkBufferUsageFlags bufferUsageFlags = VK_BUFFER_USAGE_TRANSFER_DST_BIT | vkBufferUsage.value();

        VkBufferCreateInfo bufferCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .size = _size,
            .usage = bufferUsageFlags,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        };

        VmaAllocationCreateInfo allocationCreateInfo = {
            .flags = 0,
            .usage = vkMemoryUsage.value(),
        };

        vkResult = vmaCreateBuffer(
            gfx->GetAllocator(),
            &bufferCreateInfo,
            &allocationCreateInfo,
            &_vkBuffer,
            &_vmaAllocation,
            nullptr
        );
            
        if (vkResult != VK_SUCCESS) {
            ToonLogError("vmaCreateBuffer() failed, unable to create buffer");
            return false;
        }

        result = gfx->CopyBuffer(stagingBuffer, _vkBuffer, _size);

        if (!result) {
            ToonLogError("CopyBuffer() failed, unable to copy staging buffer to buffer");
        }

        vkDestroyBuffer(gfx->GetDevice(), stagingBuffer, nullptr);
        vmaFreeMemory(gfx->GetAllocator(), stagingAllocation);
    }
    else {

        VkBufferUsageFlags bufferUsageFlags = vkBufferUsage.value();

        VkBufferCreateInfo bufferCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .size = _size,
            .usage = bufferUsageFlags,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        };

        VmaAllocationCreateInfo allocationCreateInfo = {
            .flags = VMA_ALLOCATION_CREATE_MAPPED_BIT,
            .usage = vkMemoryUsage.value(),
        };

        VmaAllocationInfo allocationInfo;

        vkResult = vmaCreateBuffer(
            gfx->GetAllocator(),
            &bufferCreateInfo,
            &allocationCreateInfo,
            &_vkBuffer,
            &_vmaAllocation,
            &allocationInfo
        );
            
        if (vkResult != VK_SUCCESS) {
            ToonLogError("CreateBuffer() failed, unable to create buffer");
            return false;
        }

        _mappedBufferMemory = allocationInfo.pMappedData;

        bool upload = (_memoryUsage == MemoryUsage::UploadOnce || _memoryUsage == MemoryUsage::UploadOften);

        if (data && upload) {
            memcpy(_mappedBufferMemory, data, _size);
        }
    }

    return true;
}

TOON_VULKAN_API
void VulkanBuffer::Terminate()
{
    VulkanGraphicsDriver * gfx = TOON_VULKAN_GRAPHICS_DRIVER(GetGraphicsDriver());

    if (_mappedBufferMemory) {
        vmaUnmapMemory(gfx->GetAllocator(), _vmaAllocation);
        _mappedBufferMemory = nullptr;
    }

    vkDestroyBuffer(gfx->GetDevice(), _vkBuffer, nullptr);
    _vkBuffer = nullptr;

    vmaFreeMemory(gfx->GetAllocator(), _vmaAllocation);
    _vmaAllocation = nullptr;
}

} // namespace Toon::Vulkan