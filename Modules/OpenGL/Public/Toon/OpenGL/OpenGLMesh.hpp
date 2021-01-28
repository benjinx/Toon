#ifndef TOON_OPENGL_MESH_HPP
#define TOON_OPENGL_MESH_HPP

#include <Toon/OpenGL/OpenGLConfig.hpp>
#include <Toon/Mesh.hpp>

namespace Toon::OpenGL {

class TOON_OPENGL_API OpenGLMesh : public Mesh
{
public:
    DISALLOW_COPY_AND_ASSIGN(OpenGLMesh)

    OpenGLMesh() = default;
    
    virtual ~OpenGLMesh() = default;

    bool Load(const std::vector<std::unique_ptr<PrimitiveData>>& data) override;

    void Render(RenderContext * ctx) override;

private:

}; // class OpenGLMesh

} // namespace Toon::OpenGL

#endif // TOON_OPENGL_MESH_HPP