#ifndef TOON_VULKAN_SHADER_HPP
#define TOON_VULKAN_SHADER_HPP

#include <Toon/Vulkan/VulkanConfig.hpp>

#include <Toon/Shader.hpp>
#include <vector>

namespace Toon::Vulkan {

#define TOON_VULKAN_SHADER(x) (dynamic_cast<Toon::Vulkan::VulkanShader *>(x))

class TOON_VULKAN_API VulkanShader : public Toon::Shader
{
public:

    DISALLOW_COPY_AND_ASSIGN(VulkanShader)

    VulkanShader() = default;

    virtual ~VulkanShader() = default;

    void Terminate() override;

    bool LoadFromFiles(const std::vector<std::string>& filenames, bool useAssetPath = true) override;

    inline std::vector<VkPipelineShaderStageCreateInfo>& GetStageList() {
        return _shaderStageList;
    }

private:

    bool LoadSPV(const std::string& filename, bool useAssetPath);

    VkShaderStageFlagBits GetVkShaderType(const std::string& filename);

    std::vector<VkShaderModule> _shaderModuleList;

    std::vector<VkPipelineShaderStageCreateInfo> _shaderStageList;
    
}; // class VulkanShader

} // namespace Toon::Vulkan

#endif // TOON_VULKAN_SHADER_HPP