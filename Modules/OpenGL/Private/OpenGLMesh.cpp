#include <Temporality/OpenGL/OpenGLMesh.hpp>

#include <Temporality/Log.hpp>
#include <Temporality/OpenGL/OpenGLPrimitive.hpp>
#include <Temporality/OpenGL/OpenGLPipeline.hpp>
#include <Temporality/OpenGL/OpenGLGraphicsDriver.hpp>

namespace Temporality::OpenGL {

TEMPORALITY_OPENGL_API
bool OpenGLMesh::Load(const std::vector<std::unique_ptr<PrimitiveData>>& data)
{
   for (const auto& primitiveData : data) {
        std::unique_ptr<Primitive> primitive = std::unique_ptr<Primitive>(new OpenGLPrimitive());
        if (!primitive->Load(primitiveData)) {
            return false;
        }

        _primitiveList.push_back(std::move(primitive));
    }

    return true;
}

TEMPORALITY_OPENGL_API
void OpenGLMesh::Render(RenderContext * ctx)
{
    OpenGLPipeline * glPipeline = TEMPORALITY_OPENGL_PIPELINE(_pipeline.get());
    glPipeline->Bind();

    for (const auto& primitive : _primitiveList) {
        OpenGLPrimitive * glPrimitive = TEMPORALITY_OPENGL_PRIMITIVE(primitive.get());
        glPrimitive->Render();
    }
}

} // namespace Temporality::OpenGL