#include <Toon/Vulkan/VulkanGraphicsDriver.hpp>

#include <Toon/Toon.hpp>
#include <Toon/Log.hpp>
#include <Toon/Benchmark.hpp>
#include <Toon/MeshImporter.hpp>
#include <Toon/Camera.hpp>
#include <Toon/Scene.hpp>

#include <algorithm>
#include <set>

DISABLE_WARNINGS()

    #define VMA_IMPLEMENTATION
    #include <vk_mem_alloc.h>

ENABLE_WARNINGS()

namespace Toon::Vulkan {

TOON_VULKAN_API
bool VulkanGraphicsDriver::Initialize()
{
    ToonBenchmarkStart();

    SDL2GraphicsDriver::Initialize();
    
    SDL2GraphicsDriver::CreateWindow(SDL_WINDOW_VULKAN);

    if (!InitInstance()) {
        return false;
    }

    #if defined(TOON_BUILD_DEBUG)

        if (!InitDebugUtilsMessenger()) {
            ToonLogWarn("Failed to initialize Vulkan Debug Utils Messenger");
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

    InitializeUpdateContext();
    InitializeRenderContext();

    if (!InitializeConstantBuffers()) {
        return false;
    }
    
    if (!InitSwapChain()) {
        return false;
    }

    if (!InitSyncObjects()) {
        return false;
    }

    VmaStats stats;
    vmaCalculateStats(_vmaAllocator, &stats);
    
    ToonLogVerbose("Allocated Memory Blocks: %u", stats.total.blockCount);
    ToonLogVerbose("Allocation Objects: %u", stats.total.allocationCount);
    ToonLogVerbose("Used bytes: %u", stats.total.usedBytes);
    ToonLogVerbose("Unused bytes: %u", stats.total.unusedBytes);

    ToonBenchmarkEnd();
    return true;
}

TOON_VULKAN_API
void VulkanGraphicsDriver::Terminate()
{
    ToonBenchmarkStart();

    if (_vkDevice) {
        vkDeviceWaitIdle(_vkDevice);
    }

    TermSyncObjects();

    TermSwapChain();

    for (auto& primitive : _primitiveList) {
        primitive->Terminate();
    }

    _primitiveList.clear();

    // TODO: Move
    _shaderGlobalsBuffer->Terminate();

    for (auto& shader : _shaderList) {
        shader->Terminate();
    }

    _shaderList.clear();

    TermAllocator();
    TermLogicalDevice();
    TermSurface();

    #if defined(TOON_BUILD_DEBUG)

        TermDebugUtilsMessenger();

    #endif

    TermInstance();

    SDL2GraphicsDriver::Terminate();

    ToonBenchmarkEnd();
}

void VulkanGraphicsDriver::InitializeRenderContext()
{
    _renderContext.reset(new VulkanRenderContext());
    SDL2GraphicsDriver::InitializeRenderContext();
}

TOON_VULKAN_API
void VulkanGraphicsDriver::SetBackbufferCount(unsigned backbufferCount)
{
    if (backbufferCount != GetBackbufferCount()) {
        ResetSwapChain();
    }

    SDL2GraphicsDriver::SetBackbufferCount(backbufferCount);
}

TOON_VULKAN_API
void VulkanGraphicsDriver::Render()
{
    VkResult vkResult;

    SDL2GraphicsDriver::Render();

    VulkanRenderContext * vkRenderCtx = TOON_VULKAN_RENDER_CONTEXT(_renderContext.get());
    vkRenderCtx->SetVkCommandBuffer(nullptr);
    
    Scene * scene = GetCurrentScene();
    if (scene) {
        scene->Render(_renderContext.get());
    }

    vmaSetCurrentFrameIndex(_vmaAllocator, _currentFrame);

    vkWaitForFences(_vkDevice, 1, &_vkInFlightFences[_currentFrame], VK_TRUE, UINT64_MAX);

    uint32_t imageIndex = 0;
    vkResult = vkAcquireNextImageKHR(_vkDevice, _vkSwapChain, UINT64_MAX, _vkImageAvailableSemaphores[_currentFrame], VK_NULL_HANDLE, &imageIndex);
    if (vkResult == VK_ERROR_OUT_OF_DATE_KHR) {
        if (!ResetSwapChain()) {
            ToonLogFatal("Failed to resize swap chain");
        }
    }
    else if (vkResult != VK_SUCCESS && vkResult != VK_SUBOPTIMAL_KHR) {
        ToonLogFatal("vkAcquireNextImageKHR() failed");
    }

    if (_vkImagesInFlight[imageIndex] != VK_NULL_HANDLE) {
        vkWaitForFences(_vkDevice, 1, &_vkImagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
    }
    _vkImagesInFlight[imageIndex] = _vkInFlightFences[_currentFrame];

    // "Present Complete"
    VkSemaphore waitSemaphores[] = { _vkImageAvailableSemaphores[_currentFrame] };
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

    // "Render Complete"
    VkSemaphore signalSemaphores[] = { _vkRenderingFinishedSemaphores[_currentFrame] };

    VkSubmitInfo submitInfo = {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .pNext = nullptr,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = waitSemaphores,
        .pWaitDstStageMask = waitStages,
        .commandBufferCount = 1,
        .pCommandBuffers = &_vkCommandBuffers[imageIndex],
        .signalSemaphoreCount = 1,
        .pSignalSemaphores = signalSemaphores,
    };

    vkResetFences(_vkDevice, 1, &_vkInFlightFences[_currentFrame]);

    vkResult = vkQueueSubmit(_vkGraphicsQueue, 1, &submitInfo, _vkInFlightFences[_currentFrame]);
    if (vkResult != VK_SUCCESS) {
        ToonLogFatal("vkQueueSubmit() failed");
    }

    VkSwapchainKHR swapChains[] = { _vkSwapChain };

    VkPresentInfoKHR presentInfo = {
        .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .pNext = 0,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = signalSemaphores,
        .swapchainCount = 1,
        .pSwapchains = swapChains,
        .pImageIndices = &imageIndex,
    };

    vkResult = vkQueuePresentKHR(_vkPresentQueue, &presentInfo);

    if (vkResult == VK_ERROR_OUT_OF_DATE_KHR || vkResult == VK_SUBOPTIMAL_KHR) {
        if (!ResetSwapChain()) {
            ToonLogFatal("Failed to resize swap chain");
        }
    }
    else if (vkResult != VK_SUCCESS) {
        ToonLogFatal("vkQueuePresentKHR() failed");
    }

    _currentFrame = (_currentFrame + 1) % GetBackbufferCount();
}

TOON_VULKAN_API
std::shared_ptr<Buffer> VulkanGraphicsDriver::CreateBuffer()
{
    return std::shared_ptr<Buffer>(new VulkanBuffer());
}


TOON_VULKAN_API
std::shared_ptr<Pipeline> VulkanGraphicsDriver::CreatePipeline(std::shared_ptr<Shader> shader)
{
    auto ptr = std::shared_ptr<Pipeline>(new VulkanPipeline());
    ptr->Initialize(shader);
    _pipelineList.push_back(ptr);
    return ptr;
}

TOON_VULKAN_API
std::shared_ptr<Texture> VulkanGraphicsDriver::CreateTexture()
{
    return std::shared_ptr<Texture>(new VulkanTexture());
}

TOON_VULKAN_API
std::shared_ptr<Shader> VulkanGraphicsDriver::CreateShader()
{
    auto ptr = std::shared_ptr<Shader>(new VulkanShader());
    // ptr->Initialize();
    _shaderList.push_back(ptr);
    return ptr;
}

TOON_VULKAN_API
std::shared_ptr<Mesh> VulkanGraphicsDriver::CreateMesh()
{
    auto ptr = std::shared_ptr<Mesh>(new VulkanMesh());
    ptr->Initialize();
    _meshList.push_back(ptr);

    // Reset swap chain?
    return ptr;
}

TOON_VULKAN_API
std::shared_ptr<Primitive> VulkanGraphicsDriver::CreatePrimitive()
{
    auto ptr = std::shared_ptr<Primitive>(new VulkanPrimitive());
    _primitiveList.push_back(ptr);
    return ptr;
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
        ToonLogError("Failed to allocate command buffer");
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
        ToonLogError("Failed to build command buffer");
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

    vkFreeCommandBuffers(_vkDevice, _vkCommandPool, 1, &commandBuffer);

    return true;
}

TOON_VULKAN_API
bool VulkanGraphicsDriver::CreateDescriptorSet(VkDescriptorSet * descriptorSet)
{
    VkResult vkResult;

    VkDescriptorSetLayout setLayouts[] = { _vkDescriptorSetLayout };

    VkDescriptorSetAllocateInfo allocateInfo = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        .pNext = nullptr,
        .descriptorPool = _vkDescriptorPool,
        .descriptorSetCount = 1,
        .pSetLayouts = setLayouts,
    };

    vkResult = vkAllocateDescriptorSets(_vkDevice, &allocateInfo, descriptorSet);
    if (vkResult != VK_SUCCESS) {
        ToonLogError("vkAllocateDescriptorSets() failed");
        return false;
    }

    VulkanBuffer * vkShaderGlobalsBuffer = TOON_VULKAN_BUFFER(GetShaderGlobalsBuffer().get());

    VkDescriptorBufferInfo bufferInfo = {
        .buffer = vkShaderGlobalsBuffer->GetVkBuffer(),
        .offset = 0,
        .range = VK_WHOLE_SIZE,
    };

    VkWriteDescriptorSet writeDescriptorSet = {
        .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
        .dstSet = *descriptorSet,
        .dstBinding = TOON_SHADER_GLOBALS_BINDING,
        .dstArrayElement = 0,
        .descriptorCount = 1,
        .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .pBufferInfo = &bufferInfo,
    };

    vkUpdateDescriptorSets(_vkDevice, 1, &writeDescriptorSet, 0, nullptr);

    return true;
}

TOON_VULKAN_API
bool VulkanGraphicsDriver::IsDeviceSuitable(const VkPhysicalDevice device)
{
    vkGetPhysicalDeviceProperties(device, &_vkPhysicalDeviceProperties);
    vkGetPhysicalDeviceFeatures(device, &_vkPhysicalDeviceFeatures);

    // TODO:
    // * Queue Families
    // * Device Extensions
    // * Swap Chain Support

    return _vkPhysicalDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU 
        && _vkPhysicalDeviceFeatures.geometryShader;
}

TOON_VULKAN_API
std::vector<const char *> VulkanGraphicsDriver::GetRequiredLayers()
{
    std::vector<const char *> requiredLayers = { };

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
    std::vector<const char *> requiredExtensions = {
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

    std::vector<const char *> requiredExtensions(requiredExtensionCount);
    sdlResult = SDL_Vulkan_GetInstanceExtensions(GetSDL2Window(), &requiredExtensionCount, requiredExtensions.data());
    if (!sdlResult) {
        ToonLogError("SDL_Vulkan_GetInstanceExtensions() failed, %s", SDL_GetError());
        return std::vector<const char *>();
    }

    #if defined(TOON_BUILD_DEBUG)

        auto it = _vkAvailableInstanceExtensions.find(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        if (it != _vkAvailableInstanceExtensions.end()) {
            requiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }
        else {
            ToonLogWarn("Vulkan Extension '%s' not available", VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

    #endif

    return requiredExtensions;
}

static VKAPI_ATTR VkBool32 VKAPI_CALL _VulkanDebugMessageCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT * callbackData,
    void * userData)
{
    // if (callbackData->messageIdNumber == 0) { // Loader Message
    //     return VK_FALSE;
    // }

    LogLevel level = LogLevel::Info;

    if ((messageType & VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT) > 0) {
        level = LogLevel::Performance;
    }
    else {
        if ((messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) > 0) {
            level = LogLevel::Error;
        }
        else if ((messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) > 0) {
            level = LogLevel::Warning;
        }
        else if ((messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) > 0) {
            level = LogLevel::Info;
        }
        else if ((messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) > 0) {
            level = LogLevel::Verbose;

            /*if (!IsVerboseLoggingEnabled()) {
                return VK_FALSE;
            }*/
        }
    }

    const char * prefix;

    switch (level) {
    case LogLevel::Error:
        prefix = "ERRO";
        break;
    case LogLevel::Warning:
        prefix = "WARN";
        break;
    case LogLevel::Info:
        prefix = "INFO";
        break;
    case LogLevel::Performance:
        prefix = "PERF";
        break;
    case LogLevel::Verbose:
        prefix = "VERB";
        break;
    default: ;
    }

    Log(level, "[%s](VkDebugUtilsMessenger) %s %s\n", 
        prefix, 
        callbackData->pMessageIdName, 
        callbackData->pMessage);

    if (callbackData->objectCount > 0) {
        for (uint32_t i = 0; i < callbackData->objectCount; ++i) {
            const char * name = callbackData->pObjects[i].pObjectName;
            if (name) {
                Log(level, "\t\tObject #%d: Type %d, Value %p, Name '%s'\n",
                    i,
                    callbackData->pObjects[i].objectType,
                    callbackData->pObjects[i].objectHandle,
                    callbackData->pObjects[i].pObjectName);
            }
        }
    }

    if (callbackData->cmdBufLabelCount > 0) {
        for (uint32_t i = 0; i < callbackData->cmdBufLabelCount; ++i) {
            const char * name = callbackData->pCmdBufLabels[i].pLabelName;
            if (name) {
                Log(level, "\t\tLabel #%d: %s\n", name);
            }
        }
    }

    return VK_FALSE;
}

TOON_VULKAN_API
bool VulkanGraphicsDriver::InitInstance()
{
    VkResult vkResult;

    int vkVersion = gladLoaderLoadVulkan(nullptr, nullptr, nullptr);
    if (vkVersion == 0) {
        ToonLogError("gladLoaderLoadVulkan() failed");
        return false;
    }
    
    ToonLogVerbose("Vulkan %d.%d", GLAD_VERSION_MAJOR(vkVersion), GLAD_VERSION_MINOR(vkVersion));

    uint32_t availableLayerCount = 0;
    vkEnumerateInstanceLayerProperties(&availableLayerCount, nullptr);

    if (availableLayerCount == 0) {
        ToonLogError("vkEnumerateInstanceLayerProperties() failed, no available layers");
        return false;
    }

    std::vector<VkLayerProperties> availableLayers(availableLayerCount);
    vkResult = vkEnumerateInstanceLayerProperties(&availableLayerCount, availableLayers.data());
    if (vkResult != VK_SUCCESS) {
        ToonLogError("vkEnumerateInstanceLayerProperties() failed");
        return false;
    }

    ToonLogVerbose("Available Vulkan Layers:");
    for (const auto& layer : availableLayers) {
        ToonLogVerbose("\t%s: %s", layer.layerName, layer.description);
        _vkAvailableLayers.emplace(layer.layerName, layer);
    }

    const auto& requiredLayers = GetRequiredLayers();

    ToonLogVerbose("Required Vulkan Device Layers:");
    for (const auto& layer : requiredLayers) {
        ToonLogVerbose("\t%s", layer);
    }

    uint32_t availableExtensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, nullptr);

    if (availableExtensionCount == 0) {
        ToonLogError("vkEnumerateInstanceExtensionProperties() failed, no extensions available");
        return false;
    }

    std::vector<VkExtensionProperties> availableExtensions(availableExtensionCount);
    vkResult = vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, availableExtensions.data());
    if (vkResult != VK_SUCCESS) {
        ToonLogError("vkEnumerateInstanceExtensionProperties() failed");
        return false;
    }

    ToonLogVerbose("Available Vulkan Instance Extensions:");
    for (const auto& extension : availableExtensions) {
        ToonLogVerbose("\t%s", extension.extensionName);
        _vkAvailableInstanceExtensions.emplace(extension.extensionName, extension);
    }

    const auto& requiredExtensions = GetRequiredInstanceExtensions();
    if (requiredExtensions.empty()) {
        ToonLogError("Failed to get Required Instance Extensions");
        return false;
    }

    ToonLogVerbose("Required Vulkan Instance Extensions:");
    for (const auto& extension : requiredExtensions) {
        ToonLogVerbose("\t%s", extension);
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


    VkInstanceCreateInfo instanceCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .pApplicationInfo = &applicationInfo,
        .enabledLayerCount = static_cast<uint32_t>(requiredLayers.size()),
        .ppEnabledLayerNames = requiredLayers.data(),
        .enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size()),
        .ppEnabledExtensionNames = requiredExtensions.data(),
    };

    VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfo;

    auto it = _vkAvailableLayers.find("VK_LAYER_KHRONOS_validation");
    if (it != _vkAvailableLayers.end()) {
        VkDebugUtilsMessageSeverityFlagsEXT messageSeverity = 
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT;

        #if defined(TOON_BUILD_DEBUG)
            messageSeverity |= VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT;
        #endif

        VkDebugUtilsMessageTypeFlagsEXT messageType =
            VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | 
            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | 
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

        debugUtilsMessengerCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
            .pNext = nullptr,
            .flags = 0,
            .messageSeverity = messageSeverity,
            .messageType = messageType,
            .pfnUserCallback = _VulkanDebugMessageCallback,
            .pUserData = nullptr,
        };

        instanceCreateInfo.pNext = &debugUtilsMessengerCreateInfo;
    }

    vkResult = vkCreateInstance(&instanceCreateInfo, nullptr, &_vkInstance);
    if (vkResult != VK_SUCCESS) {
        ToonLogError("vkCreateInstance() failed");
        return false;
    }

    // Reload glad to load instance pointers and populate available extensions
    vkVersion = gladLoaderLoadVulkan(_vkInstance, nullptr, nullptr);
    if (vkVersion == 0) {
        ToonLogError("gladLoaderLoadVulkan() failed, unable to reload symbols with VkInstance");
        return false;
    }

    return true;
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
bool VulkanGraphicsDriver::InitDebugUtilsMessenger()
{
    VkResult vkResult;

    auto it = _vkAvailableLayers.find("VK_LAYER_KHRONOS_validation");
    if (it == _vkAvailableLayers.end()) {
        ToonLogWarn("Unable to find VK_LAYER_KHRONOS_validation");
        return false;
    }

    VkDebugUtilsMessageSeverityFlagsEXT messageSeverity = 
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT;

    #if defined(TOON_BUILD_DEBUG)
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
        .pUserData = nullptr,
    };

    if (!vkCreateDebugUtilsMessengerEXT) {
        ToonLogWarn("vkCreateDebugUtilsMessengerEXT() is not bound");
        return false;
    }

    vkResult = vkCreateDebugUtilsMessengerEXT(_vkInstance, &debugUtilsMessengerCreateInfo, nullptr, &_vkDebugMessenger);
    if (vkResult != VK_SUCCESS) {
        ToonLogError("vkCreateDebugUtilsMessengerEXT() failed");
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
        ToonLogWarn("vkDestroyDebugUtilsMessengerEXT() is not bound");
        return;
    }

    vkDestroyDebugUtilsMessengerEXT(_vkInstance, _vkDebugMessenger, nullptr);
}

TOON_VULKAN_API
bool VulkanGraphicsDriver::InitSurface()
{
    SDL_bool sdlResult;

    sdlResult = SDL_Vulkan_CreateSurface(GetSDL2Window(), _vkInstance, &_vkSurface);
    if (!sdlResult) {
        ToonLogError("SDL_Vulkan_CreateSurface() failed, %s", SDL_GetError());
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
        ToonLogError("vkEnumeratePhysicalDevices() failed, no devices found");
        return false;
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkResult = vkEnumeratePhysicalDevices(_vkInstance, &deviceCount, devices.data());
    if (vkResult != VK_SUCCESS) {
        ToonLogError("vkEnumeratePhysicalDevices() failed");
        return false;
    }

    for (const auto& device : devices) {
        if (IsDeviceSuitable(device)) {
            _vkPhysicalDevice = device;
            break;
        }
    }

    if (_vkPhysicalDevice == VK_NULL_HANDLE) {
        ToonLogError("No suitable graphics device found");
        return false;
    }

    ToonLogVerbose("Physical Device Name: %s", _vkPhysicalDeviceProperties.deviceName);

    // Reload glad to load instance pointers and populate available extensions
    int vkVersion = gladLoaderLoadVulkan(_vkInstance, _vkPhysicalDevice, nullptr);
    if (vkVersion == 0) {
        ToonLogError("gladLoaderLoadVulkan() failed, unable to reload symbols with VkInstance and VkPhysicalDevice");
        return false;
    }

    uint32_t availableExtensionCount = 0;
    vkEnumerateDeviceExtensionProperties(_vkPhysicalDevice, nullptr, &availableExtensionCount, nullptr);

    if (availableExtensionCount == 0) {
        ToonLogError("vkEnumerateDeviceExtensionProperties() failed, no extensions available");
        return false;
    }

    std::vector<VkExtensionProperties> availableExtensions(availableExtensionCount);
    vkResult = vkEnumerateDeviceExtensionProperties(_vkPhysicalDevice, nullptr, &availableExtensionCount, availableExtensions.data());
    if (vkResult != VK_SUCCESS) {
        ToonLogError("vkEnumerateDeviceExtensionProperties() failed");
        return false;
    }

    ToonLogVerbose("Available Vulkan Device Extensions:");
    for (const auto& extension : availableExtensions) {
        ToonLogVerbose("\t%s", extension.extensionName);
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
        ToonLogError("vkGetPhysicalDeviceQueueFamilyProperties(), no queues found");
        return false;
    }

    std::vector<VkQueueFamilyProperties> queueFamilyProps(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(_vkPhysicalDevice, &queueFamilyCount, queueFamilyProps.data());

    _vkGraphicsQueueFamilyIndex = UINT32_MAX;
    _vkPresentQueueFamilyIndex = UINT32_MAX;
    
    for (const auto& prop : queueFamilyProps) {
        string types;
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

        ToonLogVerbose("Queue #%d: %s", prop.queueCount, types);
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
        ToonLogError("No suitable graphics queue found");
        return false;
    }

    if (_vkPresentQueueFamilyIndex == UINT32_MAX) {
        ToonLogError("No suitable present queue found");
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

    VkPhysicalDeviceFeatures requiredDeviceFeatures = {
        // TODO
    };

    const auto& requiredLayers = GetRequiredLayers();

    const auto& requiredExtensions = GetRequiredDeviceExtensions();
    
    ToonLogVerbose("Required Vulkan Device Extensions:");
    for (const auto& extension : requiredExtensions) {
        ToonLogVerbose("\t%s", extension);
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
        ToonLogError("vkCreateDevice() failed");
        return false;
    }

    // Reload glad to load instance pointers and populate available extensions
    int vkVersion = gladLoaderLoadVulkan(_vkInstance, _vkPhysicalDevice, _vkDevice);
    if (vkVersion == 0) {
        ToonLogError("gladLoaderLoadVulkan() failed, unable to reload symbols with VkInstance, VkPhysicalDevice, and VkDevice");
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
        // .preferredLargeHeapBlockSize = ,
        // .pAllocationCallbacks = ,
        // .pDeviceMemoryCallbacks = ,
        .instance = _vkInstance,
        .vulkanApiVersion = VK_API_VERSION_1_1,
    };

    vkResult = vmaCreateAllocator(&allocatorCreateInfo, &_vmaAllocator);
    if (vkResult != VK_SUCCESS) {
        ToonLogError("vmaCreateAllocator() failed");
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
    // VK_FORMAT_B8G8R8A8_SRGB

    _vkSwapChainImageFormat = formats[0];
    for (const auto& format : formats) {
        // TODO: Investigate
        if (format.format == VK_FORMAT_R8G8B8A8_UNORM && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            _vkSwapChainImageFormat = format;
            break;
        }
    }

    // TODO: Investigate
    // VK_PRESENT_MODE_FIFO_KHR = Queue of presentation requests, wait for vsync, required to be supported
    // VK_PRESENT_MODE_MAILBOX_KHR = Queue of presentation requests, wait for vsync, replaces entries if the queue is full
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

    uint32_t backbufferCount = std::clamp(
        GetBackbufferCount(),
        surfaceCapabilities.minImageCount,
        surfaceCapabilities.maxImageCount
    );

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

    // TODO: Refactor
    if (_vkGraphicsQueueFamilyIndex != _vkPresentQueueFamilyIndex) {
        swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swapChainCreateInfo.queueFamilyIndexCount = 2;
        swapChainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
    }

    vkResult = vkCreateSwapchainKHR(_vkDevice, &swapChainCreateInfo, nullptr, &_vkSwapChain);
    if (vkResult != VK_SUCCESS) {
        ToonLogError("vkCreateSwapchainKHR() failed");
        return false;
    }

    vkGetSwapchainImagesKHR(_vkDevice, _vkSwapChain, &backbufferCount, nullptr);

    _vkSwapChainImages.resize(backbufferCount);
    vkGetSwapchainImagesKHR(_vkDevice, _vkSwapChain, &backbufferCount, _vkSwapChainImages.data());

    _vkSwapChainImageViews.resize(backbufferCount);

    for (size_t i = 0; i < _vkSwapChainImages.size(); ++i) {
        VkImageViewCreateInfo imageViewCreateInfo = {
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

        vkResult = vkCreateImageView(_vkDevice, &imageViewCreateInfo, nullptr, &_vkSwapChainImageViews[i]);
        if (vkResult != VK_SUCCESS) {
            ToonLogError("vkCreateImageView() failed");
            return false;
        }
    }

    GraphicsDriver::SetBackbufferCount(backbufferCount);

    if (!InitDepthBuffer()) {
        return false;
    }

    if (!InitRenderPass()) {
        return false;
    }

    if (!InitDescriptorPool()) {
        return false;
    }

    for (const auto& pipeline : _pipelineList) {
        VulkanPipeline * vkPipeline = TOON_VULKAN_PIPELINE(pipeline.get());
        vkPipeline->Create();
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

    return true;
}

TOON_VULKAN_API
void VulkanGraphicsDriver::TermSwapChain(bool resetting /*= false*/)
{
    TermCommandBuffers();
    TermCommandPool();
    TermFramebuffers();

    if (resetting) {
        for (const auto& pipeline : _pipelineList) {
            VulkanPipeline * vkPipeline = TOON_VULKAN_PIPELINE(pipeline.get());
            vkPipeline->Destroy();
        }

        for (const auto& mesh : _meshList) {
            VulkanMesh * vkMesh = TOON_VULKAN_MESH(mesh.get());
            vkMesh->Destroy();
        }
    }
    else {
        for (const auto& pipeline : _pipelineList) {
            pipeline->Terminate();
        }
        _pipelineList.clear();

        for (const auto& mesh : _meshList) {
            mesh->Terminate();
        }
        _meshList.clear();
    }

    TermDescriptorPool();
    TermRenderPass();
    TermDepthBuffer();

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
}

TOON_VULKAN_API
bool VulkanGraphicsDriver::ResetSwapChain()
{
    ToonBenchmarkStart();

    vkDeviceWaitIdle(_vkDevice);

    TermSwapChain(true);

    if (!InitSwapChain()) {
        return false;
    }

    ToonBenchmarkEnd();
    return true;
}

TOON_VULKAN_API
bool VulkanGraphicsDriver::InitDepthBuffer()
{
    VkResult vkResult;

    std::vector<VkFormat> potentialDepthFormats = {
        VK_FORMAT_D32_SFLOAT,
        VK_FORMAT_D32_SFLOAT_S8_UINT,
        VK_FORMAT_D24_UNORM_S8_UINT,
    };

    _vkDepthImageFormat = VK_FORMAT_UNDEFINED;

    for (VkFormat format : potentialDepthFormats) {
        VkFormatProperties formatProperties;
        vkGetPhysicalDeviceFormatProperties(_vkPhysicalDevice, format, &formatProperties);

        VkFormatFeatureFlags features = (formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
        if (features == VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) {
            _vkDepthImageFormat = format;
            break;
        }
    }

    if (_vkDepthImageFormat == VK_FORMAT_UNDEFINED) {
        ToonLogError("Unable to find suitable depth buffer image format");
        return false;
    }

    VkImageCreateInfo imageCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .imageType = VK_IMAGE_TYPE_2D,
        .format = _vkDepthImageFormat,
        .extent = {
            .width = _vkSwapChainExtent.width,
            .height = _vkSwapChainExtent.height,
            .depth = 1,
        },
        .mipLevels = 1,
        .arrayLayers = 1,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .tiling = VK_IMAGE_TILING_OPTIMAL,
        .usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
    };

    VmaAllocationCreateInfo allocationCreateInfo = {
        .flags = 0,
        .usage = VMA_MEMORY_USAGE_GPU_ONLY,
    };

    vkResult = vmaCreateImage(
        _vmaAllocator,
        &imageCreateInfo,
        &allocationCreateInfo,
        &_vkDepthImage,
        &_vmaDepthImageAllocation,
        nullptr
    );

    if (vkResult != VK_SUCCESS) {
        ToonLogError("vmaCreateImage() failed, unable to create depth buffer image");
        return false;
    }

    VkImageViewCreateInfo imageViewCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .image = _vkDepthImage,
        .viewType = VK_IMAGE_VIEW_TYPE_2D,
        .format = VK_FORMAT_D32_SFLOAT,
        // .components
        .subresourceRange = {
            .aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT,
            .baseMipLevel = 0,
            .levelCount = 1,
            .baseArrayLayer = 0,
            .layerCount = 1,
        },
    };

    if (vkCreateImageView(_vkDevice, &imageViewCreateInfo, nullptr, &_vkDepthImageView) != VK_SUCCESS) {
        ToonLogError("Failed to create depth buffer image view");
        return false;
    }

    return true;
}

TOON_VULKAN_API
void VulkanGraphicsDriver::TermDepthBuffer()
{
    if (_vkDepthImageView) {
        vkDestroyImageView(_vkDevice, _vkDepthImageView, nullptr);
        _vkDepthImageView = nullptr;
    }

    if (_vkDepthImage) {
        vkDestroyImage(_vkDevice, _vkDepthImage, nullptr);
        _vkDepthImage = nullptr;
    }

    if (_vmaDepthImageAllocation) {
        vmaFreeMemory(_vmaAllocator, _vmaDepthImageAllocation);
        _vmaDepthImageAllocation = nullptr;
    }

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
        ToonLogError("vkCreateRenderPass() failed");
        return false;
    }

    return true;
}

TOON_VULKAN_API
void VulkanGraphicsDriver::TermRenderPass()
{
    if (_vkRenderPass) {
        vkDestroyRenderPass(_vkDevice, _vkRenderPass, nullptr);
        _vkRenderPass = nullptr;
    }
}

TOON_VULKAN_API
bool VulkanGraphicsDriver::InitDescriptorPool()
{
    VkResult vkResult;

    std::array<VkDescriptorPoolSize, 1> descriptorPoolSizeList = {
        VkDescriptorPoolSize {
            .type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            .descriptorCount = static_cast<uint32_t>(1 + _meshList.size()), // Can never be 0
        },
        // VkDescriptorPoolSize {
        //     .type = VK_DESCRIPTOR_TYPE_SAMPLER,
        //     .descriptorCount = 0,
        // },
    };

    VkDescriptorPoolCreateInfo descriptorPoolCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
        .maxSets = static_cast<uint32_t>(_vkSwapChainImages.size()),
        .poolSizeCount = descriptorPoolSizeList.size(),
        .pPoolSizes = descriptorPoolSizeList.data(),
    };

    vkResult = vkCreateDescriptorPool(_vkDevice, &descriptorPoolCreateInfo, nullptr, &_vkDescriptorPool);
    if (vkResult != VK_SUCCESS) {
        ToonLogError("vkCreateDescriptorPool() failed");
        return false;
    }

    std::array<VkDescriptorSetLayoutBinding, 2> descriptorSetLayoutBindingList = {
        VkDescriptorSetLayoutBinding {
            .binding = TOON_SHADER_GLOBALS_BINDING,
            .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            .descriptorCount = 1,
            .stageFlags = VK_SHADER_STAGE_ALL_GRAPHICS,
            .pImmutableSamplers = nullptr,
        },
        VkDescriptorSetLayoutBinding {
            .binding = TOON_SHADER_TRANSFORM_BINDING,
            .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            .descriptorCount = 1,
            .stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
            .pImmutableSamplers = nullptr,
        },
    };

    VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .bindingCount = static_cast<uint32_t>(descriptorSetLayoutBindingList.size()),
        .pBindings = descriptorSetLayoutBindingList.data(),
    };

    vkResult = vkCreateDescriptorSetLayout(_vkDevice, &descriptorSetLayoutCreateInfo, nullptr, &_vkDescriptorSetLayout);
    if (vkResult != VK_SUCCESS) {
        ToonLogError("vkCreateDescriptorSetLayout() failed");
        return false;
    }

    for (auto& mesh : _meshList) {
        VulkanMesh * vkMesh = TOON_VULKAN_MESH(mesh.get());
        if (!vkMesh->Create()) {
            return false;
        }
    }

    std::array<VkDescriptorSetLayout, 1> setLayouts = {
        _vkDescriptorSetLayout,
    };

    // TODO: Move into Pipeline
    VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .setLayoutCount = static_cast<uint32_t>(setLayouts.size()),
        .pSetLayouts = setLayouts.data(),
        .pushConstantRangeCount = 0,
        .pPushConstantRanges = nullptr,
    };

    vkResult = vkCreatePipelineLayout(_vkDevice, &pipelineLayoutCreateInfo, nullptr, &_vkPipelineLayout);
    if (vkResult != VK_SUCCESS) {
        ToonLogFatal("vkCreatePipelineLayout() failed");
    }

    return true;
}

TOON_VULKAN_API
void VulkanGraphicsDriver::TermDescriptorPool()
{
    if (_vkPipelineLayout) {
        vkDestroyPipelineLayout(_vkDevice, _vkPipelineLayout, nullptr);
        _vkPipelineLayout = nullptr;
    }

    if (_vkDescriptorSetLayout) {
        vkDestroyDescriptorSetLayout(_vkDevice, _vkDescriptorSetLayout, nullptr);
        _vkDescriptorSetLayout = nullptr;
    }

    if (_vkDescriptorPool) {
        vkResetDescriptorPool(_vkDevice, _vkDescriptorPool, 0); // ?
        vkDestroyDescriptorPool(_vkDevice, _vkDescriptorPool, nullptr);
        _vkDescriptorPool = nullptr;
    }
}

TOON_VULKAN_API
bool VulkanGraphicsDriver::InitFramebuffers()
{
    VkResult vkResult;

    _vkFramebuffers.resize(_vkSwapChainImageViews.size());

    for (size_t i = 0; i < _vkSwapChainImageViews.size(); ++i) {
        std::array<VkImageView, 2> attachments = {
            _vkSwapChainImageViews[i],
            _vkDepthImageView,
        };

        VkFramebufferCreateInfo framebufferCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .renderPass = _vkRenderPass,
            .attachmentCount = static_cast<uint32_t>(attachments.size()),
            .pAttachments = attachments.data(),
            .width = _vkSwapChainExtent.width,
            .height = _vkSwapChainExtent.height,
            .layers = 1,
        };

        vkResult = vkCreateFramebuffer(_vkDevice, &framebufferCreateInfo, nullptr, &_vkFramebuffers[i]);
        if (vkResult != VK_SUCCESS) {
            ToonLogError("vkCreateFramebuffer() failed");
            return false;
        }
    }

    return true;
}

TOON_VULKAN_API
void VulkanGraphicsDriver::TermFramebuffers()
{
    for (auto& framebuffer : _vkFramebuffers) {
        if (framebuffer) {
            vkDestroyFramebuffer(_vkDevice, framebuffer, nullptr);
        }
    }
    _vkFramebuffers.clear();
}

TOON_VULKAN_API
bool VulkanGraphicsDriver::InitCommandPool()
{
    VkResult vkResult;

    VkCommandPoolCreateInfo commandPoolCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .queueFamilyIndex = _vkGraphicsQueueFamilyIndex,
    };

    vkResult = vkCreateCommandPool(_vkDevice, &commandPoolCreateInfo, nullptr, &_vkCommandPool);
    if (vkResult != VK_SUCCESS) {
        ToonLogError("vkCreateCommandPool() failed");
        return false;
    }

    return true;
}

TOON_VULKAN_API
void VulkanGraphicsDriver::TermCommandPool()
{
    if (_vkCommandPool) {
        vkDestroyCommandPool(_vkDevice, _vkCommandPool, nullptr);
        _vkCommandPool = nullptr;
    }
}

TOON_VULKAN_API
bool VulkanGraphicsDriver::InitCommandBuffers()
{
    VkResult vkResult;

    VkCommandBufferAllocateInfo commandBufferAllocateInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .pNext = nullptr,
        .commandPool = _vkCommandPool,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = static_cast<uint32_t>(_vkFramebuffers.size()),
    };

    _vkCommandBuffers.resize(_vkFramebuffers.size());

    vkResult = vkAllocateCommandBuffers(_vkDevice, &commandBufferAllocateInfo, _vkCommandBuffers.data());
    if (vkResult != VK_SUCCESS) {
        ToonLogError("vkAllocateCommandBuffers() failed");
        return false;
    }

    if (!FillCommandBuffers()) {
        return false;
    }

    return true;
}

TOON_VULKAN_API
void VulkanGraphicsDriver::TermCommandBuffers()
{
    if (!_vkCommandBuffers.empty()) {
        vkFreeCommandBuffers(_vkDevice, _vkCommandPool, static_cast<size_t>(_vkCommandBuffers.size()), _vkCommandBuffers.data());
        _vkCommandBuffers.clear();
    }
}

TOON_VULKAN_API
bool VulkanGraphicsDriver::InitSyncObjects()
{
    VkResult vkResult;

    _vkImageAvailableSemaphores.resize(2);
    _vkRenderingFinishedSemaphores.resize(2);
    _vkInFlightFences.resize(2);
    _vkImagesInFlight.resize(_vkSwapChainImages.size(), VK_NULL_HANDLE);

    VkSemaphoreCreateInfo semaphoreCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
    };

    VkFenceCreateInfo fenceCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        .pNext = nullptr,
        .flags = VK_FENCE_CREATE_SIGNALED_BIT,
    };

    for (size_t i = 0; i < 2; ++i) {
        vkResult = vkCreateSemaphore(_vkDevice, &semaphoreCreateInfo, nullptr, &_vkImageAvailableSemaphores[i]);
        if (vkResult != VK_SUCCESS) {
            ToonLogError("vkCreateSemaphore() failed");
            return false;
        }
        else {
            vkResult = vkCreateSemaphore(_vkDevice, &semaphoreCreateInfo, nullptr, &_vkRenderingFinishedSemaphores[i]);
            if (vkResult != VK_SUCCESS) {
                ToonLogError("vkCreateSemaphore() failed");
                return false;
            }
            else {
                vkResult = vkCreateFence(_vkDevice, &fenceCreateInfo, nullptr, &_vkInFlightFences[i]);
                if (vkResult != VK_SUCCESS) {
                    ToonLogError("vkCreateFence() failed");
                    return false;
                }   
            }
        }
    }

    return true;
}

TOON_VULKAN_API
void VulkanGraphicsDriver::TermSyncObjects()
{
    for (auto fence : _vkInFlightFences) {
        vkDestroyFence(_vkDevice, fence, nullptr);
        fence = nullptr;
    }

    for (auto semaphore : _vkRenderingFinishedSemaphores) {
        vkDestroySemaphore(_vkDevice, semaphore, nullptr);
        semaphore = nullptr;
    }

    for (auto semaphore : _vkImageAvailableSemaphores) {
        vkDestroySemaphore(_vkDevice, semaphore, nullptr);
        semaphore = nullptr;
    }
}

TOON_VULKAN_API
bool VulkanGraphicsDriver::FillCommandBuffers()
{
    VkResult vkResult;

    for (size_t i = 0; i < _vkCommandBuffers.size(); ++i) {

        VkCommandBufferBeginInfo commandBufferBeginInfo = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .pNext = nullptr,
            .flags = 0,
            .pInheritanceInfo = nullptr,
        };

        // Requires VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT
        // vkResetCommandBuffer(_vkCommandBuffers[i], 0);

        vkResult = vkBeginCommandBuffer(_vkCommandBuffers[i], &commandBufferBeginInfo);
        if (vkResult != VK_SUCCESS) {
            ToonLogError("vkBeginCommandBuffer() failed");
            return false;
        }

        VulkanRenderContext * vkRenderContext = TOON_VULKAN_RENDER_CONTEXT(_renderContext.get());
        vkRenderContext->SetVkCommandBuffer(_vkCommandBuffers[i]);

        std::array<VkClearValue, 2> clearValues = { };

        const glm::vec4& cc = GetClearColor();
        clearValues[0].color = {
            .float32 = { cc.r, cc.g, cc.b, cc.a }
        };
        
        clearValues[1].depthStencil = { 
            .depth = 1.0f, 
            .stencil = 0,
        };

        VkRenderPassBeginInfo renderPassBeginInfo = {
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
            .renderPass = _vkRenderPass,
            .framebuffer = _vkFramebuffers[i],
            .renderArea = {
                .offset = { 0, 0 },
                .extent = _vkSwapChainExtent,
            },
            .clearValueCount = static_cast<uint32_t>(clearValues.size()),
            .pClearValues = clearValues.data(),
        };

        vkCmdBeginRenderPass(_vkCommandBuffers[i], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

        Scene * scene = GetCurrentScene();
        if (scene) {
            scene->Render(_renderContext.get());
        }


        vkCmdEndRenderPass(_vkCommandBuffers[i]);

        vkResult = vkEndCommandBuffer(_vkCommandBuffers[i]);
        if (vkResult != VK_SUCCESS) {
            ToonLogError("vkEndCommandBuffer() failed");
            return false;
        }
    }

    return true;
}

} // namespace Toon::Vulkan
