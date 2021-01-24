#ifndef TEMPORALITY_VULKAN_SHADER_HPP
#define TEMPORALITY_VULKAN_SHADER_HPP

#include <Temporality/Vulkan/VulkanConfig.hpp>

#include <Temporality/Shader.hpp>

namespace Temporality::Vulkan {

#define TEMPORALITY_VULKAN_SHADER(x) (dynamic_cast<Temporality::Vulkan::VulkanShader *>(x))

class TEMPORALITY_VULKAN_API VulkanShader : public Temporality::Shader
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

} // namespace Temporality::Vulkan

#endif // TEMPORALITY_VULKAN_SHADER_HPP