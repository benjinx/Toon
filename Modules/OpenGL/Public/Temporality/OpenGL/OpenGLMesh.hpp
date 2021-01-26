#ifndef TEMPORALITY_OPENGL_MESH_HPP
#define TEMPORALITY_OPENGL_MESH_HPP

#include <Temporality/OpenGL/OpenGLConfig.hpp>
#include <Temporality/Mesh.hpp>

namespace Temporality::OpenGL {

class TEMPORALITY_OPENGL_API OpenGLMesh : public Mesh
{
public:
    DISALLOW_COPY_AND_ASSIGN(OpenGLMesh)

    OpenGLMesh() = default;
    
    virtual ~OpenGLMesh() = default;

    bool Load(const std::vector<std::unique_ptr<PrimitiveData>>& data) override;

    void Render(RenderContext * ctx) override;

private:

}; // class OpenGLMesh

} // namespace Temporality::OpenGL

#endif // TEMPORALITY_OPENGL_MESH_HPP