#include <Toon/OpenGL/OpenGLMesh.hpp>

#include <Toon/Log.hpp>
#include <Toon/OpenGL/OpenGLPrimitive.hpp>
#include <Toon/OpenGL/OpenGLPipeline.hpp>
#include <Toon/OpenGL/OpenGLGraphicsDriver.hpp>

namespace Toon::OpenGL {

TOON_OPENGL_API
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

TOON_OPENGL_API
void OpenGLMesh::Render(RenderContext * ctx)
{
    OpenGLPipeline * glPipeline = TOON_OPENGL_PIPELINE(_pipeline.get());
    glPipeline->Bind();

    for (const auto& primitive : _primitiveList) {
        OpenGLPrimitive * glPrimitive = TOON_OPENGL_PRIMITIVE(primitive.get());
        glPrimitive->Render();
    }
}

} // namespace Toon::OpenGL