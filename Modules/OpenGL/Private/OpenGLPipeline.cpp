#include <Toon/OpenGL/OpenGLPipeline.hpp>

#include <Toon/GraphicsDriver.hpp>
#include <Toon/OpenGL/OpenGLShader.hpp>

#include <cassert>

namespace Toon::OpenGL {

TOON_OPENGL_API
bool OpenGLPipeline::Bind()
{
    auto gfx = GetGraphicsDriver();
    glm::ivec2 windowSize = gfx->GetWindowSize();

    OpenGLShader * shader = TOON_OPENGL_SHADER(_shader.get());
    if (shader) {
        shader->Bind();
    }

    glViewport(0, 0, windowSize.x, windowSize.y);

    glFrontFace(GetGLFrontFace(_frontFace));

    if (_cullMode == CullMode::None) {
        glDisable(GL_CULL_FACE);
    }
    else {
        glEnable(GL_CULL_FACE);
        glCullFace(GetGLCullMode(_cullMode));
    }

    glPolygonMode(GL_FRONT_AND_BACK, GetGLFillMode(_fillMode));

    switch (_depthBiasMode) {
        case DepthBiasMode::Clamp:
            glDisable(GL_DEPTH_TEST);
            glEnable(GL_POLYGON_OFFSET_FILL);
            glPolygonOffset(_depthBiasSlopeFactor, _depthBiasFactor);
            break;
        case DepthBiasMode::Clip:
            glEnable(GL_DEPTH_TEST);
            glDisable(GL_POLYGON_OFFSET_FILL);
            glDepthFunc(GetGLCompareOperation(_depthCompareOperation));
        break;
    }

    if (_blendEnabled) {
        glEnable(GL_BLEND);

        glBlendFuncSeparate(
            GetGLBlendFactor(_srcColorFactor),
            GetGLBlendFactor(_dstColorFactor),
            GetGLBlendFactor(_srcAlphaFactor),
            GetGLBlendFactor(_dstAlphaFactor)
        );

        glBlendEquationSeparate(
            GetGLBlendOperation(_colorBlendOp),
            GetGLBlendOperation(_alphaBlendOp)
        );
    }
    else {
        glDisable(GL_BLEND);
    }

    return true;
}

} // namespace Toon::OpenGL