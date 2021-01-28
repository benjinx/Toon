#ifndef VULKAN_RENDER_CONTEXT_HPP
#define VULKAN_RENDER_CONTEXT_HPP

#include <Toon/Vulkan/VulkanConfig.hpp>
#include <Toon/RenderContext.hpp>

namespace Toon::Vulkan {

#define TOON_VULKAN_RENDER_CONTEXT(x) (dynamic_cast<Toon::Vulkan::VulkanRenderContext *>(x))

class TOON_VULKAN_API VulkanRenderContext : public RenderContext
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


} // namespace Toon::Vulkan

#endif // VULKAN_RENDER_CONTEXT_HPP