#include <Temporality/Vulkan/VulkanGraphicsDriver.hpp>
#include <Temporality/Log.hpp>
#include <Temporality/Temporality.hpp>

#include <set>

#define VMA_IMPLEMENTATION
#include <vk_mem_alloc.h>

namespace Temporality::Vulkan {

bool VulkanGraphicsDriver::Initialize()
{
    BenchmarkStart();

    SDL2GraphicsDriver::Initialize();

    SDL2GraphicsDriver::CreateWindow(SDL_WINDOW_VULKAN);

    if (!InitInstance()) {
        return false;
    }

    #if defined(TEMPORALITY_BUILD_DEBUG)

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

    BenchmarkEnd("VulkanGraphicsDriver::Initialize()");

    return true;
}

void VulkanGraphicsDriver::Terminate()
{
    BenchmarkStart();

    #if defined(TEMPORALITY_BUILD_DEBUG)
        TermDebugUtilsMessenger();
    #endif

    TermInstance();

    SDL2GraphicsDriver::Terminate();

    BenchmarkEnd("VulkanGraphicsDriver::Terminate");
}

void VulkanGraphicsDriver::SwapBuffers()
{

}

std::shared_ptr<Texture> VulkanGraphicsDriver::CreateTexture()
{

    return nullptr;
}

std::shared_ptr<Shader> VulkanGraphicsDriver::CreateShader()
{

    return nullptr;
}

std::shared_ptr<Mesh> VulkanGraphicsDriver::CreateMesh()
{

    return nullptr;
}

bool VulkanGraphicsDriver::IsDeviceSuitable(const VkPhysicalDevice device)
{
    vkGetPhysicalDeviceProperties(device, &_vkPhysicalDeviceProperties);
    vkGetPhysicalDeviceFeatures(device, &_vkPhysicalDeviceFeatures);

    return _vkPhysicalDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU 
        && _vkPhysicalDeviceFeatures.geometryShader;
}

std::vector<const char *> VulkanGraphicsDriver::GetRequiredDeviceLayers()
{
    std::vector<const char*> requiredLayers = {};

    #if defined(TEMPORALITY_BUILD_DEBUG)
        
        auto it = _vkAvailableLayers.find("VK_LAYER_KHRONOS_validation");
        if (it != _vkAvailableLayers.end()) {
            requiredLayers.push_back("VK_LAYER_KHRONOS_validation");
        }

    #endif

    return requiredLayers;
}

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

    #if defined(TEMPORALITY_BUILD_DEBUG)

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
        .pEngineName = "Temporality",
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

    #if defined(TEMPORALITY_ENABLE_VERBOSE_LOGGING)
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

void VulkanGraphicsDriver::TermInstance()
{
    if (!_vkInstance) {
        vkDestroyInstance(_vkInstance, nullptr);
        _vkInstance = nullptr;
    }
}

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

void VulkanGraphicsDriver::TermSurface()
{
    if (_vkSurface) {
        vkDestroySurfaceKHR(_vkInstance, _vkSurface, nullptr);
        _vkSurface = nullptr;
    }
}

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

void VulkanGraphicsDriver::TermLogicalDevice()
{
    if (_vkDevice) {
        vkDestroyDevice(_vkDevice, nullptr);
        _vkDevice = nullptr;
    }
}

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

void VulkanGraphicsDriver::TermAllocator()
{
    vmaDestroyAllocator(_vmaAllocator);
}

} // namespace Temporality::Vulkan