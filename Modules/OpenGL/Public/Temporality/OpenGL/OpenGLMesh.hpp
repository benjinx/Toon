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

    void Render() override;

    bool Load(const MeshData* data) override;

private:

    GLenum GetGLMode(const MeshData::Mode& mode);

    GLuint _mglVAO;
    GLenum _mglMode;
    GLsizei _mglCount;

    bool _mIndexed;
}; // class OpenGLMesh

} // namespace Temporality::OpenGL

#endif // TEMPORALITY_OPENGL_MESH_HPP