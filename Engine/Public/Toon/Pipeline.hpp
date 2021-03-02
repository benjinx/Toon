#ifndef TOON_PIPELINE_HPP
#define TOON_PIPELINE_HPP

#include <Toon/Config.hpp>

#include <Toon/Shader.hpp>
#include <Toon/Primitive.hpp>

#include <memory>

namespace Toon {

enum class FrontFace
{
    Clockwise,
    CounterClockwise,
}; // enum class FrontFace

enum class CullMode
{
    None,
    Front,
    Back,
}; // enum class CullMode

enum class FillMode
{
    Fill,
    Line,
}; // enum class FillMode

enum class DepthBiasMode
{
    Clamp,
    Clip,
}; // enum class DepthBiasMode

enum class BlendOperation
{
    Add,
    Subtract,
    ReverseSubtract,
    Min,
    Max,
}; // enum class BlendOperation

enum class BlendFactor
{
    Zero,
    One,
    SrcColor,
    OneMinusSrcColor,
    SrcAlpha,
    OneMinusSrcAlpha,
    DstColor,
    OneMinusDstColor,
    DstAlpha,
    OneMinusDstAlpha,
    ConstantColor,
    OneMinusConstantColor,
    ConstantAlpha,
    OneMinusConstantAlpha,
    SrcAlphaSaturated, // TODO: Investigate
}; // enum class BlendFactor

enum class CompareOperation
{
    Never,
    Less,
    LessOrEqual,
    Greater,
    GreaterOrEqual,
    Equal,
    NotEqual,
    Always,
}; // enum class CompareOperation

class TOON_ENGINE_API Pipeline
{
public:

    DISALLOW_COPY_AND_ASSIGN(Pipeline)

    Pipeline() = default;

    virtual ~Pipeline() = default;

    inline virtual bool Initialize(std::shared_ptr<Shader> shader) {
        _shader = shader;
        return true;
    }

    inline virtual void Terminate() {
        _shader.reset();
    }

    inline void SetShader(std::shared_ptr<Shader> shader) {
        _shader = shader;
        // Reset?
    }

    inline void SetPrimitiveTopology(PrimitiveTopology primitiveTopology) {
        _primitiveTopology = primitiveTopology;
    }

    inline void SetFrontFace(FrontFace frontFace) {
        _frontFace = frontFace;
    }

    inline void SetCullMode(CullMode mode) {
        _cullMode = mode;
    }

    inline void SetFillMode(FillMode mode) {
        _fillMode = mode;
    }

    inline void SetDepthBiasFactor(float factor) {
        _depthBiasFactor = factor;
    }

    inline void SetDepthBiasSlopeFactor(float factor) {
        _depthBiasSlopeFactor = factor;
    }

    inline void SetDepthBiasClamp(float clamp) {
        _depthBiasClamp = clamp;
    }

    inline void SetDepthBiasMode(DepthBiasMode mode) {
        _depthBiasMode = mode;
    }

    inline void SetDepthCompareOperation(CompareOperation op) {
        _depthCompareOperation = op;
    }

protected:

    std::shared_ptr<Shader> _shader = nullptr;

    PrimitiveTopology _primitiveTopology = PrimitiveTopology::TriangleList;

    FrontFace _frontFace = FrontFace::CounterClockwise;

    CullMode _cullMode = CullMode::Back;

    FillMode _fillMode = FillMode::Fill;

    float _depthBiasFactor = 0.0f;

    float _depthBiasSlopeFactor = 0.0f;

    float _depthBiasClamp = 0.0f;

    DepthBiasMode _depthBiasMode = DepthBiasMode::Clip;

    CompareOperation _depthCompareOperation = CompareOperation::Less;

    bool _blendEnabled = false;

    BlendOperation _colorBlendOp = BlendOperation::Add;

    BlendOperation _alphaBlendOp = BlendOperation::Add;

    BlendFactor _srcColorFactor = BlendFactor::One;

    BlendFactor _dstColorFactor = BlendFactor::One;

    BlendFactor _srcAlphaFactor = BlendFactor::One;

    BlendFactor _dstAlphaFactor = BlendFactor::One;
    
}; // class Pipeline

inline string FrontFaceToString(FrontFace frontFace)
{
    switch (frontFace) {
    case FrontFace::Clockwise:
        return "Clockwise";
    case FrontFace::CounterClockwise:
        return "CounterClockwise";
    }

    return "Unknown";
}

inline string CullModeToString(CullMode cullMode)
{
    switch (cullMode) {
    case CullMode::None:
        return "None";
    case CullMode::Front:
        return "Front";
    case CullMode::Back:
        return "Back";
    }

    return "Unknown";
}

inline string CullModeToString(FillMode fillMode)
{
    switch (fillMode) {
    case FillMode::Fill:
        return "Fill";
    case FillMode::Line:
        return "Line";
    }

    return "Unknown";
}

inline string DepthBiasModeToString(DepthBiasMode depthBiasMode)
{
    switch (depthBiasMode) {
    case DepthBiasMode::Clamp:
        return "Clamp";
    case DepthBiasMode::Clip:
        return "Clip";
    }

    return "Unknown";
}

inline string BlendOperationToString(BlendOperation blendOperation)
{
    switch (blendOperation) {
    case BlendOperation::Add:
        return "Add";
    case BlendOperation::Subtract:
        return "Subtract";
    case BlendOperation::ReverseSubtract:
        return "ReverseSubtract";
    case BlendOperation::Min:
        return "Min";
    case BlendOperation::Max:
        return "Max";
    }

    return "Unknown";
}

inline string BlendFactorToString(BlendFactor blendFactor)
{
    switch (blendFactor) {
        case BlendFactor::Zero:
            return "Zero";
        case BlendFactor::One:
            return "One";
        case BlendFactor::SrcColor:
            return "SrcColor";
        case BlendFactor::OneMinusSrcColor:
            return "OneMinusSrcColor";
        case BlendFactor::SrcAlpha:
            return "SrcAlpha";
        case BlendFactor::OneMinusSrcAlpha:
            return "OneMinusSrcAlpha";
        case BlendFactor::DstColor:
            return "DstColor";
        case BlendFactor::OneMinusDstColor:
            return "OneMinusDstColor";
        case BlendFactor::DstAlpha:
            return "DstAlpha";
        case BlendFactor::OneMinusDstAlpha:
            return "OneMinusDstAlpha";
        case BlendFactor::ConstantColor:
            return "ConstantColor";
        case BlendFactor::OneMinusConstantColor:
            return "OneMinusConstantColor";
        case BlendFactor::ConstantAlpha:
            return "ConstantAlpha";
        case BlendFactor::OneMinusConstantAlpha:
            return "OneMinusConstantAlpha";
        case BlendFactor::SrcAlphaSaturated:
            return "SrcAlphaSaturated";
    }

    return "Unknown";
}

inline string CompareOperationToString(CompareOperation compareOperation)
{
    switch (compareOperation) {
        case CompareOperation::Never:
            return "Never";
        case CompareOperation::Less:
            return "Less";
        case CompareOperation::LessOrEqual:
            return "LessOrEqual";
        case CompareOperation::Greater:
            return "Greater";
        case CompareOperation::GreaterOrEqual:
            return "GreaterOrEqual";
        case CompareOperation::Equal:
            return "Equal";
        case CompareOperation::NotEqual:
            return "NotEqual";
        case CompareOperation::Always:
            return "Always";
    }

    return "Unknown";
}

} // namespace Toon

#endif // TOON_PIPELINE_HPP