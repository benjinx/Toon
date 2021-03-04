#include <Toon/OpenGL/OpenGLMesh.hpp>

#include <Toon/Log.hpp>
#include <Toon/OpenGL/OpenGLPrimitive.hpp>
#include <Toon/OpenGL/OpenGLPipeline.hpp>
#include <Toon/OpenGL/OpenGLGraphicsDriver.hpp>

namespace Toon::OpenGL {

TOON_OPENGL_API
void OpenGLMesh::Render(RenderContext * ctx)
{
    Mesh::Render(ctx);

    OpenGLPipeline * glPipeline = TOON_OPENGL_PIPELINE(_pipeline.get());
    glPipeline->Bind();

    OpenGLTexture * glTexture = TOON_OPENGL_TEXTURE(_texture.get());
    if (glTexture)
    {
        glActiveTexture(GL_TEXTURE0);
        glTexture->Bind();
    }

    OpenGLBuffer * glTransformBuffer = TOON_OPENGL_BUFFER(_shaderTransformBuffer.get());
    glBindBufferBase(GL_UNIFORM_BUFFER, TOON_SHADER_TRANSFORM_BINDING, glTransformBuffer->GetGLID());

    for (const auto& primitive : _primitiveList) {
        OpenGLPrimitive * glPrimitive = TOON_OPENGL_PRIMITIVE(primitive.get());
        glPrimitive->Render();
    }
}

} // namespace Toon::OpenGL