#ifndef TEMPORALITY_VULKAN_PIPELINE_HPP
#define TEMPORALITY_VULKAN_PIPELINE_HPP

#include <Temporality/Vulkan/VulkanConfig.hpp>

#include <Temporality/Log.hpp>
#include <Temporality/Pipeline.hpp>

#include <optional>

namespace Temporality::Vulkan {

#define TEMPORALITY_VULKAN_PIPELINE(x) (dynamic_cast<Temporality::Vulkan::VulkanPipeline *>(x))

class TEMPORALITY_VULKAN_API VulkanPipeline : public Pipeline
{
public:

    DISALLOW_COPY_AND_ASSIGN(VulkanPipeline)

    VulkanPipeline() = default;

    virtual ~VulkanPipeline() = default;

    bool Initialize() override;

    void Terminate() override;

    void GenerateCommands(VkCommandBuffer vkCommandBuffer);

private:

    VkPipeline _vkPipeline;

}; // class VulkanPipeline

inline std::optional<VkFrontFace> GetVkFrontFace(FrontFace frontFace)
{
    switch (frontFace) {
    case FrontFace::Clockwise:
        return VK_FRONT_FACE_CLOCKWISE;
    case FrontFace::CounterClockwise:
        return VK_FRONT_FACE_COUNTER_CLOCKWISE;
    }
    
    return {};
}

inline std::optional<VkCullModeFlags> GetVkCullMode(CullMode cullMode)
{
    switch (cullMode) {
    case CullMode::None:
        return 0;
    case CullMode::Front:
        return VK_CULL_MODE_FRONT_BIT;
    case CullMode::Back:
        return VK_CULL_MODE_BACK_BIT;
    }
    
    return {};
}

inline std::optional<VkPolygonMode> GetVkPolygonMode(FillMode fillMode)
{
    switch (fillMode) {
    case FillMode::Line:
        return VK_POLYGON_MODE_LINE;
    case FillMode::Fill:
        return VK_POLYGON_MODE_FILL;
    }

    return {};
}

inline std::optional<VkBlendFactor> GetVkBlendFactor(BlendFactor factor)
{
    switch (factor) {
    case BlendFactor::Zero:
        return VK_BLEND_FACTOR_ZERO;
    case BlendFactor::One:
        return VK_BLEND_FACTOR_ONE;
    case BlendFactor::SrcColor:
        return VK_BLEND_FACTOR_SRC_COLOR;
    case BlendFactor::OneMinusSrcColor:
        return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
    case BlendFactor::SrcAlpha:
        return VK_BLEND_FACTOR_SRC_ALPHA;
    case BlendFactor::OneMinusSrcAlpha:
        return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    case BlendFactor::DstColor:
        return VK_BLEND_FACTOR_DST_COLOR;
    case BlendFactor::OneMinusDstColor:
        return VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
    case BlendFactor::DstAlpha:
        return VK_BLEND_FACTOR_DST_ALPHA;
    case BlendFactor::OneMinusDstAlpha:
        return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
    case BlendFactor::ConstantColor:
        return VK_BLEND_FACTOR_CONSTANT_COLOR;
    case BlendFactor::OneMinusConstantColor:
        return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR;
    case BlendFactor::ConstantAlpha:
        return VK_BLEND_FACTOR_CONSTANT_ALPHA;
    case BlendFactor::OneMinusConstantAlpha:
        return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA;
    case BlendFactor::SrcAlphaSaturated:
        return VK_BLEND_FACTOR_SRC_ALPHA_SATURATE;
    }
    
    return {};
}

inline std::optional<VkBlendOp> GetVkBlendOp(BlendOperation op)
{
    switch (op) {
    case BlendOperation::Add:
        return VK_BLEND_OP_ADD;
    case BlendOperation::Subtract:
        return VK_BLEND_OP_SUBTRACT;
    case BlendOperation::ReverseSubtract:
        return VK_BLEND_OP_REVERSE_SUBTRACT;
    case BlendOperation::Min:
        return VK_BLEND_OP_MIN;
    case BlendOperation::Max:
        return VK_BLEND_OP_MAX;
    }
    
    return {};
}

inline std::optional<VkCompareOp> GetVkCompareOp(CompareOperation op)
{
    switch (op) {
    case CompareOperation::Never:
        return VK_COMPARE_OP_NEVER;
    case CompareOperation::Less:
        return VK_COMPARE_OP_LESS;
    case CompareOperation::Equal:
        return VK_COMPARE_OP_EQUAL;
    case CompareOperation::LessOrEqual:
        return VK_COMPARE_OP_LESS_OR_EQUAL;
    case CompareOperation::Greater:
        return VK_COMPARE_OP_GREATER;
    case CompareOperation::NotEqual:
        return VK_COMPARE_OP_NOT_EQUAL;
    case CompareOperation::GreaterOrEqual:
        return VK_COMPARE_OP_GREATER_OR_EQUAL;
    case CompareOperation::Always:
        return VK_COMPARE_OP_ALWAYS;
    }

    return {};
}

} // namespace Temporality::Vulkan

#endif // TEMPORALITY_VULKAN_PIPELINE_HPP