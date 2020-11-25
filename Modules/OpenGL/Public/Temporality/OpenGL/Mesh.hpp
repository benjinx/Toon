#ifndef TEMPORALITY_OPENGL_MESH_HPP
#define TEMPORALITY_OPENGL_MESH_HPP

#include <Temporality/OpenGL/Config.hpp>
#include <Temporality/Graphics/Mesh.hpp>

namespace Temporality::OpenGL {

class TEMPORALITY_OPENGL_API Mesh : public Temporality::Mesh
{
public:
    DISALLOW_COPY_AND_ASSIGN(Mesh)

    Mesh() = default;

    void Render() override;

    bool Load(const MeshData* data) override;

private:

    GLenum GetGLMode(const MeshData::Mode& mode);

    GLuint _mglVAO;
    GLenum _mglMode;
    GLsizei _mglCount;

    bool _mIndexed;
}; // class Mesh

} // namespace Temporality::OpenGL

#endif // TEMPORALITY_OPENGL_MESH_HPP