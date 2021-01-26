#ifndef TEMPORALITY_OPENGL_PIPELINE_HPP
#define TEMPORALITY_OPENGL_PIPELINE_HPP

#include <Temporality/OpenGL/OpenGLConfig.hpp>

#include <Temporality/Pipeline.hpp>

namespace Temporality::OpenGL {

#define TEMPORALITY_OPENGL_PIPELINE(x) (dynamic_cast<Temporality::OpenGL::OpenGLPipeline *>(x))

class TEMPORALITY_OPENGL_API OpenGLPipeline : public Pipeline
{
public:

    DISALLOW_COPY_AND_ASSIGN(OpenGLPipeline)

    OpenGLPipeline() = default;

    virtual ~OpenGLPipeline() = default;

    bool Initialize() override;

    void Terminate() override;

    bool Bind();

private:

}; // class OpenGLPipeline

inline GLenum GetGLFrontFace(FrontFace frontFace)
{
    switch (frontFace) {
    case FrontFace::Clockwise:
        return GL_CW;
    case FrontFace::CounterClockwise:
        return GL_CCW;
    default: ;
    }

    return GL_INVALID_ENUM;
}

inline GLenum GetGLCullMode(CullMode cullMode)
{
    switch (cullMode) {
    case CullMode::Front:
        return GL_FRONT;
    case CullMode::Back:
        return GL_BACK;
    default: ;
    }
    
    return GL_INVALID_ENUM;
}

inline GLenum GetGLFillMode(FillMode fillMode)
{
    switch (fillMode) {
    case FillMode::Fill:
        return GL_FILL;
    case FillMode::Line:
        return GL_LINE;
    default: ;
    }
    
    return GL_INVALID_ENUM;
}

inline GLenum GetGLBlendFactor(BlendFactor factor)
{
    switch (factor) {
    case BlendFactor::Zero:
        return GL_ZERO;
    case BlendFactor::One:
        return GL_ONE;
    case BlendFactor::SrcColor:
        return GL_SRC_COLOR;
    case BlendFactor::OneMinusSrcColor:
        return GL_ONE_MINUS_SRC_COLOR;
    case BlendFactor::SrcAlpha:
        return GL_SRC_ALPHA;
    case BlendFactor::OneMinusSrcAlpha:
        return GL_ONE_MINUS_SRC_ALPHA;
    case BlendFactor::DstColor:
        return GL_DST_COLOR;
    case BlendFactor::OneMinusDstColor:
        return GL_ONE_MINUS_DST_COLOR;
    case BlendFactor::DstAlpha:
        return GL_DST_ALPHA;
    case BlendFactor::OneMinusDstAlpha:
        return GL_ONE_MINUS_DST_ALPHA;
    case BlendFactor::ConstantColor:
        return GL_CONSTANT_COLOR;
    case BlendFactor::OneMinusConstantColor:
        return GL_ONE_MINUS_CONSTANT_COLOR;
    case BlendFactor::ConstantAlpha:
        return GL_CONSTANT_ALPHA;
    case BlendFactor::OneMinusConstantAlpha:
        return GL_ONE_MINUS_CONSTANT_ALPHA;
    case BlendFactor::SrcAlphaSaturated:
        return GL_SRC_ALPHA_SATURATE;
    }
    
    return GL_INVALID_ENUM;
}

inline GLenum GetGLBlendOperation(BlendOperation op)
{
    switch (op) {
    case BlendOperation::Add:
        return GL_FUNC_ADD;
    case BlendOperation::Subtract:
        return GL_FUNC_SUBTRACT;
    case BlendOperation::ReverseSubtract:
        return GL_FUNC_REVERSE_SUBTRACT;
    case BlendOperation::Min:
        return GL_MIN;
    case BlendOperation::Max:
        return GL_MAX;
    }
    
    return GL_INVALID_ENUM;
}

inline GLenum GetGLCompareOperation(CompareOperation op)
{
    switch (op) {
    case CompareOperation::Never:
        return GL_NEVER;
    case CompareOperation::Less:
        return GL_LESS;
    case CompareOperation::Equal:
        return GL_EQUAL;
    case CompareOperation::LessOrEqual:
        return GL_LEQUAL;
    case CompareOperation::Greater:
        return GL_GREATER;
    case CompareOperation::NotEqual:
        return GL_NOTEQUAL;
    case CompareOperation::GreaterOrEqual:
        return GL_GEQUAL;
    case CompareOperation::Always:
        return GL_ALWAYS;
    }

    return GL_INVALID_ENUM;
}

} // namespace Temporality::OpenGL

#endif // TEMPORALITY_OPENGL_PIPELINE_HPP