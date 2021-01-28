#ifndef TOON_VULKAN_GRAPHICS_DRIVER_HPP
#define TOON_VULKAN_GRAPHICS_DRIVER_HPP

#include <Toon/Vulkan/VulkanConfig.hpp>

#include <Toon/SDL2/SDL2GraphicsDriver.hpp>
#include <Toon/Vulkan/VulkanPipeline.hpp>
//#include <Toon/Vulkan/VulkanTexture.hpp>
#include <Toon/Vulkan/VulkanShader.hpp>
#include <Toon/Vulkan/VulkanMesh.hpp>
#include <Toon/Vulkan/VulkanPrimitive.hpp>
#include <Toon/Vulkan/VulkanBuffer.hpp>
#include <Toon/Vulkan/VulkanRenderContext.hpp>

#include <vector>

#include <unordered_map>

#include <string>

#include <vk_mem_alloc.h>

namespace Toon::Vulkan {

#define TOON_VULKAN_GRAPHICS_DRIVER(x) (dynamic_cast<Toon::Vulkan::VulkanGraphicsDriver *>(x))

class TOON_VULKAN_API VulkanGraphicsDriver : public SDL2::SDL2GraphicsDriver
{
public:
    
    DISALLOW_COPY_AND_ASSIGN(VulkanGraphicsDriver)

    VulkanGraphicsDriver() = default;

    virtual ~VulkanGraphicsDriver() = default;

    bool Initialize() override;

    void Terminate() override;

    void Render() override;

    std::shared_ptr<Texture> CreateTexture() override;

    std::shared_ptr<Shader> CreateShader() override;

    std::shared_ptr<Pipeline> CreatePipeline(std::shared_ptr<Shader> shader) override;

    std::shared_ptr<Mesh> CreateMesh() override;

    std::unique_ptr<Primitive> CreatePrimitive() override;

    inline VkDevice GetDevice() const {
        return _vkDevice;
    }

    inline VmaAllocator GetAllocator() const {
        return _vmaAllocator;
    }

    inline VkExtent2D GetSwapChainExtent() const {
        return _vkSwapChainExtent;
    }

    inline VkPipelineLayout GetPipelineLayout() {
        return _vkPipelineLayout;
    }

    inline VkRenderPass GetRenderPass() const {
        return _vkRenderPass;
    }

    uint32_t FindMemoryType(uint32_t filter, VkMemoryPropertyFlags props);

    bool CreateBuffer(VkBuffer * buffer, VmaAllocation * vmaAllocation, VkDeviceSize size, VkBufferUsageFlags bufferUsage, VmaMemoryUsage memoryUsage);

    bool CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

private:

    bool IsDeviceSuitable(const VkPhysicalDevice device);

    std::vector<const char *> GetRequiredDeviceLayers();

    std::vector<const char *> GetRequiredDeviceExtensions();

    std::vector<const char *> GetRequiredInstanceExtensions();

    bool InitWindow();

    void TermWindow();

    bool InitDebugUtilsMessenger();

    void TermDebugUtilsMessenger();

    bool InitInstance();

    void TermInstance();

    bool InitSurface();

    void TermSurface();

    bool InitPhysicalDevice();

    bool InitLogicalDevice();

    void TermLogicalDevice();

    bool InitAllocator();

    void TermAllocator();

    bool InitSwapChain();

    void TermSwapChain();

    bool ResetSwapChain();

    bool InitRenderPass();

    bool InitDescriptorPool();

    bool InitGraphicsPipelines();

    bool InitDepthBuffer();

    bool InitFramebuffers();

    bool InitCommandPool();

    bool InitCommandBuffers();

    bool InitSyncObjects();

    std::unordered_map<std::string, VkLayerProperties> _vkAvailableLayers;

    std::unordered_map<std::string, VkExtensionProperties> _vkAvailableInstanceExtensions;

    std::unordered_map<std::string, VkExtensionProperties> _vkAvailableDeviceExtensions;

    VkInstance _vkInstance = nullptr;

    VkDebugUtilsMessengerEXT _vkDebugMessenger = nullptr;

    VkSurfaceKHR _vkSurface = nullptr;

    VkPhysicalDeviceProperties _vkPhysicalDeviceProperties;

    VkPhysicalDeviceFeatures _vkPhysicalDeviceFeatures;

    VkPhysicalDevice _vkPhysicalDevice = nullptr;

    VkDevice _vkDevice = nullptr;

    uint32_t _vkGraphicsQueueFamilyIndex;
    uint32_t _vkPresentQueueFamilyIndex;

    VkQueue _vkGraphicsQueue = nullptr;
    VkQueue _vkPresentQueue = nullptr;

    VmaAllocator _vmaAllocator = nullptr;

    VkSurfaceFormatKHR _vkSwapChainImageFormat;

    VkExtent2D _vkSwapChainExtent;

    VkSwapchainKHR _vkSwapChain = nullptr;

    std::vector<VkImage> _vkSwapChainImages;

    std::vector<VkImageView> _vkSwapChainImageViews;

    VkRenderPass _vkRenderPass = nullptr;

    VkFormat _vkDepthImageFormat;

    VkImage _vkDepthImage = nullptr;

    VkDeviceMemory _vkDepthImageMemory = nullptr;

    VkImageView _vkDepthImageView = nullptr;

    std::vector<VkFramebuffer> _vkFramebuffers;

    VkCommandPool _vkCommandPool = nullptr;

    std::vector<VkCommandBuffer> _vkCommandBuffers;

    std::vector<VkSemaphore> _vkImageAvailableSemaphores;

    std::vector<VkSemaphore> _vkRenderingFinishedSemaphores;

    std::vector<VkFence> _vkInFlightFences;

    std::vector<VkFence> _vkImagesInFlight;

    std::vector<VkBuffer> _vkUniformBuffers;

    std::vector<VmaAllocation> _vkUniformBufferAllocations;

    VkDescriptorPool _vkDescriptorPool = nullptr;

    VkDescriptorSetLayout _vkDescriptorSetLayout = nullptr;

    VkPipelineLayout _vkPipelineLayout = nullptr;

    std::vector<VkDescriptorSet> _vkDescriptorSets;

    int _currentFrame = 0;

    std::vector<std::shared_ptr<Pipeline>> _pipelines;

}; // class VulkanGraphicsDriver

} // namespace Toon::Vulkan

#endif // TOON_VULKAN_GRAPHICS_DRIVER_HPP