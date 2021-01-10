#ifndef TEMPORALITY_VULKAN_GRAPHICS_DRIVER_HPP
#define TEMPORALITY_VULKAN_GRAPHICS_DRIVER_HPP

#include <Temporality/Vulkan/VulkanConfig.hpp>

#include <Temporality/SDL2/SDL2GraphicsDriver.hpp>

#include <vector>

#include <unordered_map>

#include <string>

#include <vk_mem_alloc.h>

namespace Temporality::Vulkan {

class TEMPORALITY_VULKAN_API VulkanGraphicsDriver : public SDL2::SDL2GraphicsDriver
{
public:
    
    DISALLOW_COPY_AND_ASSIGN(VulkanGraphicsDriver)

    VulkanGraphicsDriver() = default;

    virtual ~VulkanGraphicsDriver() = default;

    bool Initialize() override;

    void Terminate() override;

    void SwapBuffers() override;

    std::shared_ptr<Texture> CreateTexture() override;

    std::shared_ptr<Shader> CreateShader() override;

    std::shared_ptr<Mesh> CreateMesh() override;

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

    std::unordered_map<std::string, VkLayerProperties> _vkAvailableLayers;

    std::unordered_map<std::string, VkExtensionProperties> _vkAvailableInstanceExtensions;

    std::unordered_map<std::string, VkExtensionProperties> _vkAvailableDeviceExtensions;

    VkInstance _vkInstance;

    VkDebugUtilsMessengerEXT _vkDebugMessenger;

    VkSurfaceKHR _vkSurface;

    VkPhysicalDeviceProperties _vkPhysicalDeviceProperties;

    VkPhysicalDeviceFeatures _vkPhysicalDeviceFeatures;

    VkPhysicalDevice _vkPhysicalDevice;

    VkDevice _vkDevice;

    uint32_t _vkGraphicsQueueFamilyIndex;
    uint32_t _vkPresentQueueFamilyIndex;

    VkQueue _vkGraphicsQueue = nullptr;
    VkQueue _vkPresentQueue = nullptr;

    VmaAllocator _vmaAllocator = nullptr;

}; // class VulkanGraphicsDriver

} // namespace Temporality::Vulkan

#endif // TEMPORALITY_VULKAN_GRAPHICS_DRIVER_HPP