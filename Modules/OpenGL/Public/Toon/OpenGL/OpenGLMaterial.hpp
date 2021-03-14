#ifndef TOON_OPENGL_MATERIAL_HPP
#define TOON_OPENGL_MATERIAL_HPP

#include <Toon/OpenGL/OpenGLConfig.hpp>

#include <Toon/Material.hpp>

namespace Toon::OpenGL {

class TOON_OPENGL_API OpenGLMaterial : public Material
{
public:

    DISALLOW_COPY_AND_ASSIGN(OpenGLMaterial)

    OpenGLMaterial() = default;

    virtual ~OpenGLMaterial() = default;

private:

};

} // namespace Toon::OpenGL

#endif // TOON_OPENGL_MATERIAL_HPP