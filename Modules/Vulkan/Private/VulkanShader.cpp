#include <Toon/Vulkan/VulkanShader.hpp>

#include <Toon/Log.hpp>
#include <Toon/Benchmark.hpp>
#include <Toon/Vulkan/VulkanGraphicsDriver.hpp>

#include <fstream>

namespace Toon::Vulkan {

TOON_VULKAN_API
void VulkanShader::Terminate()
{
    auto gfx = TOON_VULKAN_GRAPHICS_DRIVER(GetGraphicsDriver());

    for (auto& shaderModule : _shaderModuleList) {
        vkDestroyShaderModule(gfx->GetDevice(), shaderModule, nullptr);
    }
    _shaderModuleList.clear();
}

TOON_VULKAN_API
bool VulkanShader::LoadFromFiles(const std::vector<string>& filenames, bool useAssetPath /*= true*/)
{
    ToonBenchmarkStart();

    for (const auto& filename : filenames) {
        if (!LoadSPV(filename, useAssetPath)) {
            if (!LoadSPV(filename + ".spv", useAssetPath)) {
                ToonLogError("Failed to load '%s'", filename);
                return false;
            }
        }
    }

    ToonBenchmarkEnd();
    return true;
}

TOON_VULKAN_API
bool VulkanShader::LoadSPV(const string& filename, bool useAssetPath)
{
    VulkanGraphicsDriver * gfx = TOON_VULKAN_GRAPHICS_DRIVER(GetGraphicsDriver());

    const auto& assetPaths = GetAssetPathList();

    std::ifstream file;

    if (useAssetPath) {
        for (const auto& path : assetPaths) {
            Path fullPath = path / "Shaders" / filename;
            ToonLogVerbose("Checking '%s'", fullPath);

            file.open(fullPath.ToString(), std::ios::binary);
            if (file.is_open()) {
                break;
            }
        }
    }
    else {
        file.open(filename, std::ios::binary);
    }

    if (!file.is_open()) {
        return false;
    }

    file.unsetf(std::ios::skipws);

    ToonLogLoad("Loading SPIR-V shader '%s'", filename);

    std::vector<uint8_t> data(
        (std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>()
    );

    VkShaderStageFlagBits type = GetVkShaderType(filename);
    if (type == VK_SHADER_STAGE_ALL) {
        ToonLogError("Failed to determine shader type of '%s'", filename);
        return false;
    }

    VkShaderModuleCreateInfo shaderModuleCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .codeSize = data.size(),
        .pCode = reinterpret_cast<const uint32_t *>(data.data()),
    };

    VkShaderModule shaderModule;

    if (vkCreateShaderModule(gfx->GetDevice(), &shaderModuleCreateInfo, nullptr, &shaderModule) != VK_SUCCESS) {
        ToonLogError("Failed to create shader module");
        return false;
    }

    VkPipelineShaderStageCreateInfo shaderStageCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .stage = type,
        .module = shaderModule, // This conflicts with the `module` keyword in C++20
        .pName = "main", // TODO: Update
        .pSpecializationInfo = nullptr,
    };

    _shaderModuleList.push_back(shaderModule);
    _shaderStageList.push_back(shaderStageCreateInfo);

    return true;
}

TOON_VULKAN_API
VkShaderStageFlagBits VulkanShader::GetVkShaderType(const string& filename)
{
    string ext = GetExtension(filename);
    size_t pivot = filename.find_last_of('.');
    if (pivot == string::npos) {
        return VK_SHADER_STAGE_ALL; // Invalid
    }
    ext = GetExtension(filename.substr(0, pivot));

    if (ext == "vert" || ext == "vertex") {
        return VK_SHADER_STAGE_VERTEX_BIT;
    }
    else if (ext == "frag" || ext == "fragment") {
        return VK_SHADER_STAGE_FRAGMENT_BIT;
    }
    else if (ext == "tesc" || ext == "tesscontrol") {
        return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
    }
    else if (ext == "tese" || ext == "tesseval") {
        return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
    }
    else if (ext == "comp" || ext == "compute") {
        return VK_SHADER_STAGE_COMPUTE_BIT;
    }

    return VK_SHADER_STAGE_ALL; // Invalid
}

} // namespace Toon::Vulkan