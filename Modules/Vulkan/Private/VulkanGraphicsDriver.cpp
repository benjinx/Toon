#include <Toon/Vulkan/VulkanGraphicsDriver.hpp>
#include <Toon/Log.hpp>
#include <Toon/Toon.hpp>

#include <set>

#define VMA_IMPLEMENTATION
#include <vk_mem_alloc.h>

namespace Toon::Vulkan {

TOON_VULKAN_API
bool VulkanGraphicsDriver::Initialize()
{
    BenchmarkStart();

    if (!SDL2GraphicsDriver::Initialize()) {
        return false;
    }

    SDL2GraphicsDriver::CreateWindow(SDL_WINDOW_VULKAN);

    if (!InitInstance()) {
        return false;
    }

    #if defined(TOON_BUILD_DEBUG)

        if (!InitDebugUtilsMessenger()) {
            LogWarn("Failed to initialize Vulkan Debug Utils Messenger");
        }

    #endif

    if (!InitSurface()) {
        return false;
    }

    if (!InitPhysicalDevice()) {
        return false;
    }

    if (!InitLogicalDevice()) {
        return false;
    }

    if (!InitAllocator()) {
        return false;
    }

    if (!InitSwapChain()) {
        return false;
    }

    BenchmarkEnd("VulkanGraphicsDriver::Initialize()");

    return true;
}

TOON_VULKAN_API
void VulkanGraphicsDriver::Terminate()
{
    BenchmarkStart();

    #if defined(TOON_BUILD_DEBUG)
        TermDebugUtilsMessenger();
    #endif

    TermInstance();

    SDL2GraphicsDriver::Terminate();

    BenchmarkEnd("VulkanGraphicsDriver::Terminate");
}

TOON_VULKAN_API
void VulkanGraphicsDriver::Render()
{

}

TOON_VULKAN_API
std::shared_ptr<Texture> VulkanGraphicsDriver::CreateTexture()
{

    return nullptr;
}

TOON_VULKAN_API
std::shared_ptr<Shader> VulkanGraphicsDriver::CreateShader()
{
    return std::shared_ptr<Shader>(new VulkanShader());
}

TOON_VULKAN_API
std::shared_ptr<Pipeline> VulkanGraphicsDriver::CreatePipeline(std::shared_ptr<Shader> shader)
{
    auto ptr = std::shared_ptr<Pipeline>(new VulkanPipeline());
    ptr->SetShader(shader);
    // ptr->Initialize();
    _pipelines.push_back(ptr);

    ResetSwapChain();
    return ptr;
}

TOON_VULKAN_API
std::shared_ptr<Mesh> VulkanGraphicsDriver::CreateMesh()
{
    return std::shared_ptr<Mesh>(new VulkanMesh());
}

TOON_VULKAN_API
std::unique_ptr<Primitive> VulkanGraphicsDriver::CreatePrimitive()
{
    return std::unique_ptr<Primitive>(new VulkanPrimitive());
}

TOON_VULKAN_API
uint32_t VulkanGraphicsDriver::FindMemoryType(uint32_t filter, VkMemoryPropertyFlags props)
{
    VkPhysicalDeviceMemoryProperties memoryProperties;
    vkGetPhysicalDeviceMemoryProperties(_vkPhysicalDevice, &memoryProperties);

    for (unsigned i = 0; i < memoryProperties.memoryTypeCount; ++i) {
        if ((filter & (1 << i)) && 
            (memoryProperties.memoryTypes[i].propertyFlags & props) == props) {
            return i;
        }
    }

    LogError("Failed to find suitable memory type");
    return UINT32_MAX;
}

TOON_VULKAN_API
bool VulkanGraphicsDriver::CreateBuffer(VkBuffer * buffer, VmaAllocation * vmaAllocation, VkDeviceSize size, VkBufferUsageFlags bufferUsage, VmaMemoryUsage memoryUsage)
{
    VkResult vkResult;
    
    VkBufferCreateInfo bufferCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .size = size,
        .usage = bufferUsage,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
    };

    VmaAllocationCreateInfo allocationCreateInfo = {
        .flags = VMA_ALLOCATION_CREATE_MAPPED_BIT,
        .usage = memoryUsage,
    };

    VmaAllocationInfo allocationInfo = {
    };

    vkResult = vmaCreateBuffer(_vmaAllocator, &bufferCreateInfo, &allocationCreateInfo, buffer, vmaAllocation, &allocationInfo);
    if (vkResult != VK_SUCCESS) {
        LogError("Failed to create buffer");
        return false;
    }

    return true;
}

TOON_VULKAN_API
bool VulkanGraphicsDriver::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
    VkResult vkResult;

    VkCommandBufferAllocateInfo commandBufferAllocateInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .pNext = nullptr,
        .commandPool = _vkCommandPool,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1,
    };

    VkCommandBuffer commandBuffer;
    vkResult = vkAllocateCommandBuffers(_vkDevice, &commandBufferAllocateInfo, &commandBuffer);
    if (vkResult != VK_SUCCESS) {
        LogError("Failed to allocate command buffer");
        return false;
    }

    VkCommandBufferBeginInfo commandBufferBeginInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
    };

    vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo);

    VkBufferCopy copyRegion = {
        .srcOffset = 0,
        .dstOffset = 0,
        .size = size,
    };

    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    vkResult = vkEndCommandBuffer(commandBuffer);
    if (vkResult != VK_SUCCESS) {
        LogError("Failed to build command buffer");
        return false;
    }

    VkSubmitInfo submitInfo = {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .pNext = nullptr,
        .commandBufferCount = 1,
        .pCommandBuffers = &commandBuffer,
    };

    vkResult = vkQueueSubmit(_vkGraphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(_vkGraphicsQueue);

    return true;
}

TOON_VULKAN_API
bool VulkanGraphicsDriver::IsDeviceSuitable(const VkPhysicalDevice device)
{
    vkGetPhysicalDeviceProperties(device, &_vkPhysicalDeviceProperties);
    vkGetPhysicalDeviceFeatures(device, &_vkPhysicalDeviceFeatures);

    return _vkPhysicalDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU 
        && _vkPhysicalDeviceFeatures.geometryShader;
}

TOON_VULKAN_API
std::vector<const char *> VulkanGraphicsDriver::GetRequiredDeviceLayers()
{
    std::vector<const char*> requiredLayers = {};

    #if defined(TOON_BUILD_DEBUG)
        
        auto it = _vkAvailableLayers.find("VK_LAYER_KHRONOS_validation");
        if (it != _vkAvailableLayers.end()) {
            requiredLayers.push_back("VK_LAYER_KHRONOS_validation");
        }

    #endif

    return requiredLayers;
}

TOON_VULKAN_API
std::vector<const char *> VulkanGraphicsDriver::GetRequiredDeviceExtensions()
{
    std::vector<const char*> requiredExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    };

    auto it = _vkAvailableDeviceExtensions.find(VK_KHR_GET_MEMORY_REQUIREMENTS_2_EXTENSION_NAME);
    if (it != _vkAvailableDeviceExtensions.end()) {
        requiredExtensions.push_back(VK_KHR_GET_MEMORY_REQUIREMENTS_2_EXTENSION_NAME);
    }

    it = _vkAvailableDeviceExtensions.find(VK_KHR_DEDICATED_ALLOCATION_EXTENSION_NAME);
    if (it != _vkAvailableDeviceExtensions.end()) {
        requiredExtensions.push_back(VK_KHR_DEDICATED_ALLOCATION_EXTENSION_NAME);
    }

    it = _vkAvailableDeviceExtensions.find(VK_KHR_BIND_MEMORY_2_EXTENSION_NAME);
    if (it != _vkAvailableDeviceExtensions.end()) {
        requiredExtensions.push_back(VK_KHR_BIND_MEMORY_2_EXTENSION_NAME);
    }

    it = _vkAvailableDeviceExtensions.find(VK_EXT_MEMORY_BUDGET_EXTENSION_NAME);
    if (it != _vkAvailableDeviceExtensions.end()) {
        requiredExtensions.push_back(VK_EXT_MEMORY_BUDGET_EXTENSION_NAME);
    }

    return requiredExtensions;
}

TOON_VULKAN_API
std::vector<const char *> VulkanGraphicsDriver::GetRequiredInstanceExtensions()
{
    SDL_bool sdlResult;

    uint32_t requiredExtensionCount;
    SDL_Vulkan_GetInstanceExtensions(GetSDL2Window(), &requiredExtensionCount, nullptr);

    std::vector<const char*> requiredExtensions(requiredExtensionCount);
    sdlResult = SDL_Vulkan_GetInstanceExtensions(GetSDL2Window(), &requiredExtensionCount, requiredExtensions.data());
    if (!sdlResult) {
        LogError("SDL_Vulkan_GetInstanceExtensions() failed, %s", SDL_GetError());
        return std::vector<const char *>();
    }

    #if defined(TOON_BUILD_DEBUG)

        auto it = _vkAvailableInstanceExtensions.find(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        if (it != _vkAvailableInstanceExtensions.end()) {
            requiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }
        else {
            LogWarn("Vulkan Extension '%s' not available", VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }
    #endif

    return requiredExtensions;
}

TOON_VULKAN_API
bool VulkanGraphicsDriver::InitInstance()
{
    VkResult vkResult;

    int vkVersion = gladLoaderLoadVulkan(nullptr, nullptr, nullptr);
    if (vkVersion == 0) {
        LogError("gladLoaderLoadVulkan() failed");
        return false;
    }

    LogVerbose("Vulkan %d.%d", GLAD_VERSION_MAJOR(vkVersion), GLAD_VERSION_MINOR(vkVersion));

    uint32_t availableLayerCount = 0;
    vkEnumerateInstanceLayerProperties(&availableLayerCount, nullptr);

    if (availableLayerCount == 0) {
        LogError("vkEnumerateInstanceLayerProperties() failed, no available layers");
        return false;
    }

    std::vector<VkLayerProperties> availableLayers(availableLayerCount);
    vkResult = vkEnumerateInstanceLayerProperties(&availableLayerCount, availableLayers.data());
    if (vkResult != VK_SUCCESS) {
        LogError("vkEnumerateInstanceLayerProperties() failed");
        return false;
    }

    LogVerbose("Available Vulkan Layers:");
    for (const auto& layer : availableLayers)
    {
        LogVerbose("\t%s: %s", layer.layerName, layer.description);
        _vkAvailableLayers.emplace(layer.layerName, layer);
    }

    uint32_t availableExtensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, nullptr);

    if (availableExtensionCount == 0) {
        LogError("vkEnumerateInstanceExtensionProperties() failed, no extensions available");
        return false;
    }

    std::vector<VkExtensionProperties> availableExtensions(availableExtensionCount);
    vkResult = vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, availableExtensions.data());
    if (vkResult != VK_SUCCESS) {
        LogError("vkEnumerateInstanceExtensionProperties() failed");
        return false;
    }

    LogVerbose("Available Vulkan Instance Extensions:");
    for (const auto& extension : availableExtensions) {
        LogVerbose("\t%s", extension.extensionName);
        _vkAvailableInstanceExtensions.emplace(extension.extensionName, extension);
    }

    const auto& requiredExtensions = GetRequiredInstanceExtensions();
    if (requiredExtensions.empty())
    {
        LogError("Failed to get Required Instance Extensions");
        return false;
    }

    LogVerbose("Required Vulkan Instance Extensions:");
    for (const auto& extension : requiredExtensions) {
        LogVerbose("\t%s", extension);
    }

    const auto& engineVersion = GetVersion();
    const auto& appVersion = GetApplicationVersion();
    const auto& appName = GetApplicationName();

    VkApplicationInfo applicationInfo = {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pNext = nullptr,
        .pApplicationName = appName.c_str(),
        .applicationVersion = VK_MAKE_VERSION(
            appVersion.Major,
            appVersion.Minor,
            appVersion.Patch
        ),
        .pEngineName = "Toon",
        .engineVersion = VK_MAKE_VERSION(
            engineVersion.Major,
            engineVersion.Minor,
            engineVersion.Patch
        ),
        .apiVersion = VK_API_VERSION_1_1,
    };

    VkInstanceCreateInfo createInfo = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .pApplicationInfo = &applicationInfo,
        .enabledLayerCount = 0,
        .ppEnabledLayerNames = nullptr,
        .enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size()),
        .ppEnabledExtensionNames = requiredExtensions.data(),
    };

    vkResult = vkCreateInstance(&createInfo, nullptr, &_vkInstance);
    if (vkResult != VK_SUCCESS) {
        LogError("vkCreateInstance() failed");
        return false;
    }

    // Reload glad to load instance pointers and populate available extensions
    vkVersion = gladLoaderLoadVulkan(_vkInstance, nullptr, nullptr);
    if (vkVersion == 0) {
        LogError("gladLoaderLoadVulkan() failed, unable to reload symbols with VkInstance");
        return false;
    }

    return true;
}

static VKAPI_ATTR VkBool32 VKAPI_CALL _VulkanDebugMessageCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
    void* userData)
{
    if ((messageType & VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT) > 0) {
        Log(LogLevel::Performance, "[PERF](VkDebugUtilsMessenger) %s\n", callbackData->pMessage);
    }
    else {
        if ((messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) > 0) {
            Log(LogLevel::Error, "[ERRO](VkDebugUtilsMessenger) %s\n", callbackData->pMessage);
        }
        else if ((messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) > 0) {
            Log(LogLevel::Warning, "[WARN](VkDebugUtilsMessenger) %s\n", callbackData->pMessage);
        }
        else if ((messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) > 0) {
            Log(LogLevel::Info, "[INFO](VkDebugUtilsMessenger) %s\n", callbackData->pMessage);
        }
        else if ((messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) > 0) {
            Log(LogLevel::Verbose, "[VERB](VkDebugUtilsMessenger) %s\n", callbackData->pMessage);
        }
    }

    return VK_FALSE;
}

TOON_VULKAN_API
bool VulkanGraphicsDriver::InitDebugUtilsMessenger()
{
    VkResult vkResult;

    auto it = _vkAvailableLayers.find("VK_LAYER_KHRONOS_validation");
    if (it == _vkAvailableLayers.end()) {
        LogWarn("Unable to find VK_LAYER_KHRONOS_validation");
        return false;
    }

    VkDebugUtilsMessageSeverityFlagsEXT messageSeverity =
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT;

    #if defined(TOON_ENABLE_VERBOSE_LOGGING)
        messageSeverity |= VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT;
    #endif

    VkDebugUtilsMessageTypeFlagsEXT messageType = 
        VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

    VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
        .pNext = nullptr,
        .flags = 0,
        .messageSeverity = messageSeverity,
        .messageType = messageType,
        .pfnUserCallback = _VulkanDebugMessageCallback,
        .pUserData = nullptr
    };

    if (!vkCreateDebugUtilsMessengerEXT) {
        LogWarn("vkCreateDebugUtilsMessengerEXT() is not bound");
        return false;
    }

    vkResult = vkCreateDebugUtilsMessengerEXT(_vkInstance, &debugUtilsMessengerCreateInfo, nullptr, &_vkDebugMessenger);
    if (vkResult != VK_SUCCESS) {
        LogError("vkCreateDebugUtilsMessengerEXT() failed");
        return false;
    }

    return true;
}

TOON_VULKAN_API
void VulkanGraphicsDriver::TermDebugUtilsMessenger()
{
    if (!_vkDebugMessenger) {
        return;
    }

    if (!vkDestroyDebugUtilsMessengerEXT) {
        LogWarn("vkDestroyDebugUtilsMessengerEXT() is not bound");
        return;
    }

    vkDestroyDebugUtilsMessengerEXT(_vkInstance, _vkDebugMessenger, nullptr);
}

TOON_VULKAN_API
void VulkanGraphicsDriver::TermInstance()
{
    if (!_vkInstance) {
        vkDestroyInstance(_vkInstance, nullptr);
        _vkInstance = nullptr;
    }
}

TOON_VULKAN_API
bool VulkanGraphicsDriver::InitSurface()
{
    SDL_bool sdlResult;

    sdlResult = SDL_Vulkan_CreateSurface(GetSDL2Window(), _vkInstance, &_vkSurface);
    if (!sdlResult) {
        LogError("SDL_Vulkan_CreateSurface() failed, %s", SDL_GetError());
        return false;
    }

    return true;
}

TOON_VULKAN_API
void VulkanGraphicsDriver::TermSurface()
{
    if (_vkSurface) {
        vkDestroySurfaceKHR(_vkInstance, _vkSurface, nullptr);
        _vkSurface = nullptr;
    }
}

TOON_VULKAN_API
bool VulkanGraphicsDriver::InitPhysicalDevice()
{
    VkResult vkResult;

    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(_vkInstance, &deviceCount, nullptr);
    
    if (deviceCount == 0) {
        LogError("vkEnumeratePhysicalDevices() failed, no devices found");
        return false;
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkResult = vkEnumeratePhysicalDevices(_vkInstance, &deviceCount, devices.data());
    if (vkResult != VK_SUCCESS) {
        LogError("vkEnumeratePhysicalDevices() failed");
        return false;
    }

    for (const auto& device : devices) {
        if (IsDeviceSuitable(device)) {
            _vkPhysicalDevice = device;
            break;
        }
    }

    if (_vkPhysicalDevice == VK_NULL_HANDLE) {
        LogError("No suitable graphics device found");
        return false;
    }

    LogVerbose("Physical Device Name: %s", _vkPhysicalDeviceProperties.deviceName);

    // Reload glad to load instance pointers and populate available extensions
    int vkVersion = gladLoaderLoadVulkan(_vkInstance, _vkPhysicalDevice, nullptr);
    if (vkVersion == 0) {
        LogError("gladLoaderLoadVulkan() failed, unable to reload symbols with VkInstance and VkPhysicalDevice");
        return false;
    }

    uint32_t availableExtensionCount = 0;
    vkEnumerateDeviceExtensionProperties(_vkPhysicalDevice, nullptr, &availableExtensionCount, nullptr);

    if (availableExtensionCount == 0) {
        LogError("vkEnumerateDeviceExtensionProperties() failed, no extensions available");
        return false;
    }

    std::vector<VkExtensionProperties> availableExtensions(availableExtensionCount);
    vkResult = vkEnumerateDeviceExtensionProperties(_vkPhysicalDevice, nullptr, &availableExtensionCount, availableExtensions.data());
    if (vkResult != VK_SUCCESS) {
        LogError("vkEnumerateDeviceExtensionsProperties() failed");
        return false;
    }

    LogVerbose("Available Vulkan Device Extensions:");
    for (const auto& extension : availableExtensions) {
        LogVerbose("\t%s", extension.extensionName);
        _vkAvailableDeviceExtensions.emplace(extension.extensionName, extension);
    }

    return true;
}

TOON_VULKAN_API
bool VulkanGraphicsDriver::InitLogicalDevice()
{
    VkResult vkResult;

    uint32_t queueFamilyCount;
    vkGetPhysicalDeviceQueueFamilyProperties(_vkPhysicalDevice, &queueFamilyCount, nullptr);
    if (queueFamilyCount == 0) {
        LogError("vkGetPhysicalDeviceQueueFamilyProperties(), no queues found");
        return false;
    }

    std::vector<VkQueueFamilyProperties> queueFamilyProps(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(_vkPhysicalDevice, &queueFamilyCount, queueFamilyProps.data());

    _vkGraphicsQueueFamilyIndex = UINT32_MAX;
    _vkPresentQueueFamilyIndex = UINT32_MAX;

    for (const auto& prop : queueFamilyProps) {
        std::string types;
        if (prop.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            types += "Graphics ";
        }
        if (prop.queueFlags & VK_QUEUE_COMPUTE_BIT) {
            types += "Compute ";
        }
        if (prop.queueFlags & VK_QUEUE_TRANSFER_BIT) {
            types += "Transfer ";
        }
        if (prop.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT) {
            types += "SparseBinding ";
        }

        LogVerbose("Queue #%d: %s", prop.queueCount, types);
    }

    uint32_t index = 0;
    for (const auto& prop : queueFamilyProps) {
        if (prop.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            _vkGraphicsQueueFamilyIndex = index;
        }

        VkBool32 presentSupported = VK_FALSE;
        vkGetPhysicalDeviceSurfaceSupportKHR(_vkPhysicalDevice, index, _vkSurface, &presentSupported);

        if (presentSupported) {
            _vkPresentQueueFamilyIndex = index;
        }

        if (_vkGraphicsQueueFamilyIndex != UINT32_MAX && _vkPresentQueueFamilyIndex != UINT32_MAX) {
            break;
        }

        ++index;
    }

    if (_vkGraphicsQueueFamilyIndex == UINT32_MAX) {
        LogError("No suitable graphics queue found");
        return false;
    }

    if (_vkPresentQueueFamilyIndex == UINT32_MAX) {
        LogError("No suitable present queue found");
        return false;
    }

    const float queuePriorities = 1.0f;

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfoList;
    std::set<uint32_t> uniqueQueueFamilies = { _vkGraphicsQueueFamilyIndex, _vkPresentQueueFamilyIndex };

    for (auto family : uniqueQueueFamilies) {
        queueCreateInfoList.push_back({
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .queueFamilyIndex = family,
            .queueCount = 1,
            .pQueuePriorities = &queuePriorities,
        });
    }

    VkPhysicalDeviceFeatures requiredDeviceFeatures = { };
    
    const auto& requiredLayers = GetRequiredDeviceLayers();

    LogVerbose("Required Vulkan Device Layers:");
    for (const auto& layer : requiredLayers) {
        LogVerbose("\t%s", layer);
    }

    const auto& requiredExtensions = GetRequiredDeviceExtensions();

    LogVerbose("Required Vulkan Device Extensions:");
    for (const auto& extension : requiredExtensions) {
        LogVerbose("\t%s", extension);
    }

    VkDeviceCreateInfo deviceCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfoList.size()),
        .pQueueCreateInfos = queueCreateInfoList.data(),
        .enabledLayerCount = static_cast<uint32_t>(requiredLayers.size()),
        .ppEnabledLayerNames = requiredLayers.data(),
        .enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size()),
        .ppEnabledExtensionNames = requiredExtensions.data(),
        .pEnabledFeatures = &requiredDeviceFeatures,
    };

    vkResult = vkCreateDevice(_vkPhysicalDevice, &deviceCreateInfo, nullptr, &_vkDevice);
    if (vkResult != VK_SUCCESS) {
        LogError("vkCreateDevice() failed");
        return false;
    }

    // Reload glad to load instance pointers and populate available extensions
    int vkVersion = gladLoaderLoadVulkan(_vkInstance, _vkPhysicalDevice, _vkDevice);
    if (vkVersion == 0) {
        LogError("gladLoaderLoadVulkan() failed, unable to reload symbols with VkInstance, VkPhysicalDevice, and VkDevice");
        return false;
    }

    vkGetDeviceQueue(_vkDevice, _vkGraphicsQueueFamilyIndex, 0, &_vkGraphicsQueue);
    vkGetDeviceQueue(_vkDevice, _vkPresentQueueFamilyIndex, 0, &_vkPresentQueue);
    
    return true;
}

TOON_VULKAN_API
void VulkanGraphicsDriver::TermLogicalDevice()
{
    if (_vkDevice) {
        vkDestroyDevice(_vkDevice, nullptr);
        _vkDevice = nullptr;
    }
}

TOON_VULKAN_API
bool VulkanGraphicsDriver::InitAllocator()
{
    VkResult vkResult;

    VmaAllocatorCreateFlags flags = 0;

    auto it = _vkAvailableDeviceExtensions.find(VK_KHR_GET_MEMORY_REQUIREMENTS_2_EXTENSION_NAME);
    if (it != _vkAvailableDeviceExtensions.end()) {
        flags |= VMA_ALLOCATOR_CREATE_KHR_BIND_MEMORY2_BIT;
    }

    it = _vkAvailableDeviceExtensions.find(VK_KHR_DEDICATED_ALLOCATION_EXTENSION_NAME);
    if (it != _vkAvailableDeviceExtensions.end()) {
        flags |= VMA_ALLOCATOR_CREATE_KHR_DEDICATED_ALLOCATION_BIT;
    }

    it = _vkAvailableDeviceExtensions.find(VK_EXT_MEMORY_BUDGET_EXTENSION_NAME);
    if (it != _vkAvailableDeviceExtensions.end()) {
        flags |= VMA_ALLOCATOR_CREATE_EXT_MEMORY_BUDGET_BIT;
    }

    VmaAllocatorCreateInfo allocatorCreateInfo = {
        .flags = flags,
        .physicalDevice = _vkPhysicalDevice,
        .device = _vkDevice,
        .instance = _vkInstance,
        .vulkanApiVersion = VK_API_VERSION_1_1,
    };

    vkResult = vmaCreateAllocator(&allocatorCreateInfo, &_vmaAllocator);
    if (vkResult != VK_SUCCESS) {
        LogError("vmaCreateAllocator() failed");
        return false;
    }

    return true;
}

TOON_VULKAN_API
void VulkanGraphicsDriver::TermAllocator()
{
    vmaDestroyAllocator(_vmaAllocator);
}

TOON_VULKAN_API
bool VulkanGraphicsDriver::InitSwapChain()
{
    VkResult vkResult;

    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(_vkPhysicalDevice, _vkSurface, &surfaceCapabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(_vkPhysicalDevice, _vkSurface, &formatCount, nullptr);

    std::vector<VkSurfaceFormatKHR> formats(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(_vkPhysicalDevice, _vkSurface, &formatCount, formats.data());

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(_vkPhysicalDevice, _vkSurface, &presentModeCount, nullptr);

    std::vector<VkPresentModeKHR> presentModes(presentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(_vkPhysicalDevice, _vkSurface, &presentModeCount, presentModes.data());

    // VK_FORMAT_R8G8B8A8_UNORM
    _vkSwapChainImageFormat = formats[0];
    for (const auto& format : formats) {
        if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            _vkSwapChainImageFormat = format;
            break;
        }
    }

    // TODO: Mine will use the VK_PRESENT_MODE_MAILBOX_KHR, stephens will use VK_PRESENT_MODE_FIFO_KHR
    VkPresentModeKHR swapChainPresentMode = VK_PRESENT_MODE_FIFO_KHR;
    for (const auto& presentMode : presentModes) {
        if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            swapChainPresentMode = presentMode;
            break;
        }
    }

    _vkSwapChainExtent = surfaceCapabilities.currentExtent;
    if (_vkSwapChainExtent.width == UINT32_MAX) {
        glm::ivec2 size = GetWindowSize();

        _vkSwapChainExtent.width = std::clamp(static_cast<uint32_t>(size.x),
            surfaceCapabilities.minImageExtent.width, surfaceCapabilities.maxImageExtent.width);

        _vkSwapChainExtent.height = std::clamp(static_cast<uint32_t>(size.y),
            surfaceCapabilities.minImageExtent.height, surfaceCapabilities.maxImageExtent.height);
    }

    /*uint32_t backbufferCount = std::clamp(
        GetBackbufferCount(),
        surfaceCapabilities.minImageCount,
        surfaceCapabilities.maxImageCount
    );*/

    uint32_t backbufferCount = 2;

    VkSwapchainCreateInfoKHR swapChainCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .pNext = nullptr,
        .flags = 0,
        .surface = _vkSurface,
        .minImageCount = static_cast<uint32_t>(backbufferCount),
        .imageFormat = _vkSwapChainImageFormat.format,
        .imageColorSpace = _vkSwapChainImageFormat.colorSpace,
        .imageExtent = _vkSwapChainExtent,
        .imageArrayLayers = 1,
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .queueFamilyIndexCount = 0,
        .pQueueFamilyIndices = nullptr,
        .preTransform = surfaceCapabilities.currentTransform,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode = swapChainPresentMode,
        .clipped = VK_TRUE,
    };

    uint32_t queueFamilyIndices[] = {
        _vkGraphicsQueueFamilyIndex,
        _vkPresentQueueFamilyIndex
    };

    if (_vkGraphicsQueueFamilyIndex != _vkPresentQueueFamilyIndex) {
        swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swapChainCreateInfo.queueFamilyIndexCount = 2;
        swapChainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
    }

    vkResult = vkCreateSwapchainKHR(_vkDevice, &swapChainCreateInfo, nullptr, &_vkSwapChain);
    if (vkResult != VK_SUCCESS) {
        LogError("vkCreateSwapchainKHR() failed");
        return false;
    }

    vkGetSwapchainImagesKHR(_vkDevice, _vkSwapChain, &backbufferCount, nullptr);

    _vkSwapChainImages.resize(backbufferCount);
    vkGetSwapchainImagesKHR(_vkDevice, _vkSwapChain, &backbufferCount, _vkSwapChainImages.data());

    _vkSwapChainImageViews.resize(backbufferCount);

    for (size_t i = 0; i < _vkSwapChainImages.size(); ++i) {
        VkImageViewCreateInfo VkImageViewCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .image = _vkSwapChainImages[i],
            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .format = _vkSwapChainImageFormat.format,
            .subresourceRange = {
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .baseMipLevel = 0,
                .levelCount = 1,
                .baseArrayLayer = 0,
                .layerCount = 1,
            },
        };

        vkResult = vkCreateImageView(_vkDevice, &VkImageViewCreateInfo, nullptr, &_vkSwapChainImageViews[i]);
        if (vkResult != VK_SUCCESS) {
            LogError("vkCreateImageView() failed");
            return false;
        }
    }

    return true;
}

void VulkanGraphicsDriver::TermSwapChain()
{
    if (_vkDepthImageView) {
        vkDestroyImageView(_vkDevice, _vkDepthImageView, nullptr);
        _vkDepthImageView = nullptr;
    }

    if (_vkDepthImage) {
        vkDestroyImage(_vkDevice, _vkDepthImage, nullptr);
        _vkDepthImage = nullptr;
    }

    if (_vkDepthImageMemory) {
        vkFreeMemory(_vkDevice, _vkDepthImageMemory, nullptr);
        _vkDepthImageMemory = nullptr;
    }

    for (auto& framebuffer : _vkFramebuffers) {
        if (framebuffer) {
            vkDestroyFramebuffer(_vkDevice, framebuffer, nullptr);
            framebuffer = nullptr;
        }
    }

    if (!_vkCommandBuffers.empty()) {
        vkFreeCommandBuffers(_vkDevice, _vkCommandPool, static_cast<size_t>(_vkCommandBuffers.size()), _vkCommandBuffers.data());
        _vkCommandBuffers.assign(_vkCommandBuffers.size(), nullptr);
    }

    // for (const auto& pipeline : _pipelines) {
    //     pipeline->Terminate();
    // }

    if (_vkPipelineLayout) {
        vkDestroyPipelineLayout(_vkDevice, _vkPipelineLayout, nullptr);
        _vkPipelineLayout = nullptr;
    }

    if (_vkRenderPass) {
        vkDestroyRenderPass(_vkDevice, _vkRenderPass, nullptr);
        _vkRenderPass = nullptr;
    }

    for (auto& imageView : _vkSwapChainImageViews) {
        if (imageView) {
            vkDestroyImageView(_vkDevice, imageView, nullptr);
            imageView = nullptr;
        }
    }

    if (_vkSwapChain) {
        vkDestroySwapchainKHR(_vkDevice, _vkSwapChain, nullptr);
        _vkSwapChain = nullptr;
    }

    if (_vkDescriptorPool) {
        vkDestroyDescriptorPool(_vkDevice, _vkDescriptorPool, nullptr);
        _vkDescriptorPool = nullptr;
    }
}

TOON_VULKAN_API
bool VulkanGraphicsDriver::ResetSwapChain()
{
    BenchmarkStart();

    vkDeviceWaitIdle(_vkDevice);

    TermSwapChain();

    if (!InitSwapChain()) {
        return false;
    }

    if (!InitRenderPass()) {
        return false;
    }

    if (!InitDescriptorPool()) {
        return false;
    }

    if (!InitGraphicsPipelines()) {
        return false;
    }

    if (!InitDepthBuffer()) {
        return false;
    }

    if (!InitFramebuffers()) {
        return false;
    }

    if (!InitCommandPool()) {
        return false;
    }

    if (!InitCommandBuffers()) {
        return false;
    }

    BenchmarkEnd("VulkanGraphicsDriver::ResetSwapChain()");
    return true;
}

TOON_VULKAN_API
bool VulkanGraphicsDriver::InitRenderPass()
{
    VkResult vkResult;

    VkAttachmentDescription colorAttachmentDescription = {
        .flags = 0,
        .format = _vkSwapChainImageFormat.format,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
        .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
        .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
    };

    VkAttachmentDescription depthAttachmentDescription = {
        .flags = 0,
        .format = _vkDepthImageFormat,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
        .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        .finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
    };

    VkAttachmentReference colorAttachmentReference = {
        .attachment = 0,
        .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
    };

    VkAttachmentReference depthAttachmentReference = {
        .attachment = 1,
        .layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
    };

    VkSubpassDescription subpassDescription = {
        .flags = 0,
        .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
        .inputAttachmentCount = 0,
        .pInputAttachments = nullptr,
        .colorAttachmentCount = 1,
        .pColorAttachments = &colorAttachmentReference,
        .pResolveAttachments = nullptr,
        .pDepthStencilAttachment = &depthAttachmentReference,
        .preserveAttachmentCount = 0,
        .pPreserveAttachments = nullptr,
    };

    VkSubpassDependency subpassDependency = {
        .srcSubpass = VK_SUBPASS_EXTERNAL,
        .dstSubpass = 0,
        .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
        .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
        .srcAccessMask = 0,
        .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
    };

    std::array<VkAttachmentDescription, 2> attachments = {
        colorAttachmentDescription,
        depthAttachmentDescription,
    };

    VkRenderPassCreateInfo renderPassCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .attachmentCount = static_cast<uint32_t>(attachments.size()),
        .pAttachments = attachments.data(),
        .subpassCount = 1,
        .pSubpasses = &subpassDescription,
        .dependencyCount = 1,
        .pDependencies = &subpassDependency,
    };

    vkResult = vkCreateRenderPass(_vkDevice, &renderPassCreateInfo, nullptr, &_vkRenderPass);
    if (vkResult != VK_SUCCESS) {
        LogError("vkCreateRenderPass() failed");
        return false;
    }

    return true;
}

TOON_VULKAN_API
bool VulkanGraphicsDriver::InitDescriptorPool()
{
    VkResult vkResult;

    VkDescriptorPoolSize descriptorPoolSize = {
        .type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .descriptorCount = static_cast<uint32_t>(_vkSwapChainImages.size()),
    };

    VkDescriptorPoolCreateInfo descriptorPoolCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
        .maxSets = static_cast<uint32_t>(_vkSwapChainImages.size()),
        .poolSizeCount = 1,
        .pPoolSizes = &descriptorPoolSize,
    };

    vkResult = vkCreateDescriptorPool(_vkDevice, &descriptorPoolCreateInfo, nullptr, &_vkDescriptorPool);
    if (vkResult != VK_SUCCESS) {
        LogError("vkCreateDescriptorPool() failed");
        return false;
    }

    std::vector<VkDescriptorSetLayoutBinding> layoutBindingList;
    //layoutBindingList.resize(_constantBufferBindings.size());

    // size_t i = 0;
    // for (const auto& it : _constantBufferBindings) {
    //     layoutBindingList[i] = {
    //         .binding = it.first,
    //         .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
    //         .descriptorCount = 1,
    //         .stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
    //         .pImmutableSamplers = nullptr,
    //     };
    // }

    VkDescriptorSetLayoutCreateInfo descriptorLayoutCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .bindingCount = static_cast<uint32_t>(layoutBindingList.size()),
        .pBindings = layoutBindingList.data(),
    };

    vkResult = vkCreateDescriptorSetLayout(_vkDevice, &descriptorLayoutCreateInfo, nullptr, &_vkDescriptorSetLayout);
    if (vkResult != VK_SUCCESS) {
        LogError("vkCreateDescriptorSetLayout() failed");
        return false;
    }

    VkDescriptorSetLayout setLayouts[] = { _vkDescriptorSetLayout };

    // TODO: Move into Pipeline
    // Get "default" Registered Constant Buffers from the graphics driver at creation time
    VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .setLayoutCount = 1,
        .pSetLayouts = setLayouts,
        .pushConstantRangeCount = 0,
        .pPushConstantRanges = nullptr,
    };

    vkResult = vkCreatePipelineLayout(_vkDevice, &pipelineLayoutCreateInfo, nullptr, &_vkPipelineLayout);
    if (vkResult != VK_SUCCESS) {
        LogFatal("vkCreatePipelineLayout() failed");
    }

    std::vector<VkDescriptorSetLayout> layouts(_vkSwapChainImages.size(), _vkDescriptorSetLayout);
    VkDescriptorSetAllocateInfo allocateInfo = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        .pNext = nullptr,
        .descriptorPool = _vkDescriptorPool,
        .descriptorSetCount = static_cast<uint32_t>(_vkSwapChainImages.size()),
        .pSetLayouts = layouts.data(),
    };

    _vkDescriptorSets.resize(_vkSwapChainImages.size());
    vkResult = vkAllocateDescriptorSets(_vkDevice, &allocateInfo, _vkDescriptorSets.data());
    if (vkResult != VK_SUCCESS) {
        LogError("vkAllocateDescriptorSets() failed");
        return false;
    }

    return true;
}

TOON_VULKAN_API
bool VulkanGraphicsDriver::InitGraphicsPipelines()
{
    for (auto& pipeline : _pipelines) {
        if (!pipeline->Initialize()) {
            return false;
        }
    }

    return false;
}

TOON_VULKAN_API
bool VulkanGraphicsDriver::InitDepthBuffer()
{
    return false;
}

TOON_VULKAN_API
bool VulkanGraphicsDriver::InitFramebuffers()
{
    return false;
}

TOON_VULKAN_API
bool VulkanGraphicsDriver::InitCommandPool()
{
    return false;
}

TOON_VULKAN_API
bool VulkanGraphicsDriver::InitCommandBuffers()
{
    return false;
}

} // namespace Toon::Vulkan