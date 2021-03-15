#ifndef TOON_OPENGL_MATERIAL_HPP
#define TOON_OPENGL_MATERIAL_HPP

#include <Toon/OpenGL/OpenGLConfig.hpp>

#include <Toon/Material.hpp>

namespace Toon::OpenGL {

#define TOON_OPENGL_MATERIAL(x) (dynamic_cast<Toon::OpenGL::OpenGLMaterial *>(x))

class TOON_OPENGL_API OpenGLMaterial : public Material
{
public:

    DISALLOW_COPY_AND_ASSIGN(OpenGLMaterial)

    OpenGLMaterial() = default;

    virtual ~OpenGLMaterial() = default;

    void Bind();

private:

}; // class OpenGLMaterial

} // namespace Toon::OpenGL

#endif // TOON_OPENGL_MATERIAL_HPP