#ifndef VULKAN_RENDER_CONTEXT_HPP
#define VULKAN_RENDER_CONTEXT_HPP

#include <Temporality/Vulkan/VulkanConfig.hpp>
#include <Temporality/RenderContext.hpp>

namespace Temporality::Vulkan {

#define TEMPORALITY_VULKAN_RENDER_CONTEXT(x) (dynamic_cast<Temporality::Vulkan::VulkanRenderContext *>(x))

class TEMPORALITY_VULKAN_API VulkanRenderContext : public RenderContext
{
public:

    DISALLOW_COPY_AND_ASSIGN(VulkanRenderContext)

    VulkanRenderContext() = default;

    virtual ~VulkanRenderContext() = default;

    inline void SetVkCommandBuffer(VkCommandBuffer vkCommandBuffer) {
        _vkCommandBuffer = vkCommandBuffer;
    }

    inline VkCommandBuffer GetVkCommandBuffer() {
        return _vkCommandBuffer;
    }

private:

    VkCommandBuffer _vkCommandBuffer;
}; // class VulkanRenderContext


} // namespace Temporality::Vulkan

#endif // VULKAN_RENDER_CONTEXT_HPP