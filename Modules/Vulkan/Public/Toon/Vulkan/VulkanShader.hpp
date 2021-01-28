#ifndef TOON_VULKAN_SHADER_HPP
#define TOON_VULKAN_SHADER_HPP

#include <Toon/Vulkan/VulkanConfig.hpp>

#include <Toon/Shader.hpp>

namespace Toon::Vulkan {

#define TOON_VULKAN_SHADER(x) (dynamic_cast<Toon::Vulkan::VulkanShader *>(x))

class TOON_VULKAN_API VulkanShader : public Toon::Shader
{
public:

    DISALLOW_COPY_AND_ASSIGN(VulkanShader)

    VulkanShader() = default;

    virtual ~VulkanShader() = default;

    bool LoadFromFiles(const std::vector<std::string>& filenames) override;

    inline std::vector<VkPipelineShaderStageCreateInfo>& GetStages() {
        return _shaderStages;
    }

private:

    bool LoadSPV(const std::string& filename);

    VkShaderStageFlagBits GetVkShaderType(const std::string& filename);

    std::vector<VkPipelineShaderStageCreateInfo> _shaderStages;
    
}; // class VulkanShader

} // namespace Toon::Vulkan

#endif // TOON_VULKAN_SHADER_HPP