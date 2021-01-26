#ifndef TEMPORALITY_VULKAN_BUFFER_HPP
#define TEMPORALITY_VULKAN_BUFFER_HPP

#include <Temporality/Vulkan/VulkanConfig.hpp>
#include <Temporality/Buffer.hpp>

#include <optional>

#include <vk_mem_alloc.h>

namespace Temporality::Vulkan {

#define TEMPORALITY_VULKAN_BUFFER(x) (dynamic_cast<Temporality::Vulkan::VulkanBuffer *>(x))

class TEMPORALITY_VULKAN_API VulkanBuffer : public Buffer
{
public:

    DISALLOW_COPY_AND_ASSIGN(VulkanBuffer)

    VulkanBuffer() = default;

    virtual inline ~VulkanBuffer() {
        Terminate();
    };

    bool Initialize(size_t size, uint8_t * data, BufferUsage bufferUsage, MemoryUsage memoryUsage) override;

    void Terminate() override;

    bool ReadFrom(size_t offset, size_t length, uint8_t * data) override;

    bool WriteTo(size_t offset, size_t length, uint8_t * data) override;

    inline VkBuffer GetVkBuffer() {
        return _vkBuffer;
    }

private:

    VkBuffer _vkBuffer;

    VmaAllocation _vmaAllocation;

    VkDeviceSize _size;
}; // class VulkanBuffer

inline std::optional<VkBufferUsageFlagBits> GetVkBufferUsage(BufferUsage bufferUsage)
{
    switch (bufferUsage) {
    case BufferUsage::Index:
        return VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
    case BufferUsage::Vertex:
        return VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    case BufferUsage::Constant:
        return VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    }

    return {};
}

inline std::optional<VmaMemoryUsage> GetVkMemoryUsage(MemoryUsage memoryUsage)
{
    switch (memoryUsage) {
    case MemoryUsage::GPU:
        return VMA_MEMORY_USAGE_GPU_ONLY;
    case MemoryUsage::UploadOnce:
        return VMA_MEMORY_USAGE_CPU_ONLY;
    case MemoryUsage::UploadOften:
        return VMA_MEMORY_USAGE_CPU_TO_GPU;
    case MemoryUsage::Download:
        return VMA_MEMORY_USAGE_GPU_TO_CPU;
    }

    return {};
}

}; // namespace Temporality::Vulkan

#endif // TEMPORALITY_VULKAN_BUFFER_HPP