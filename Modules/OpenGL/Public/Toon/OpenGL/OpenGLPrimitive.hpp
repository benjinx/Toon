#ifndef TOON_OPENGL_PRIMITIVE_HPP
#define TOON_OPENGL_PRIMITIVE_HPP

#include <Toon/OpenGL/OpenGLConfig.hpp>

#include <Toon/Primitive.hpp>
#include <Toon/PrimitiveData.hpp>

namespace Toon::OpenGL {

#define TOON_OPENGL_PRIMITIVE(x) (dynamic_cast<Toon::OpenGL::OpenGLPrimitive *>(x))

class TOON_OPENGL_API OpenGLPrimitive : public Primitive
{
public:

    DISALLOW_COPY_AND_ASSIGN(OpenGLPrimitive)

    OpenGLPrimitive() = default;

    virtual ~OpenGLPrimitive() = default;

    void Render();

    bool Load(const std::unique_ptr<PrimitiveData>& data) override;

private:

    GLuint _glVAO;

    GLenum _glMode;

    GLsizei _glCount;

    bool _indexed;
}; // class OpenGLPrimitive

inline GLenum GetGLPrimitiveTopology(PrimitiveTopology primitiveTopology)
{
    switch (primitiveTopology) {
    case PrimitiveTopology::PointList:
        return GL_POINTS;
    case PrimitiveTopology::LineList:
        return GL_LINES;
    case PrimitiveTopology::LineStrip:
        return GL_LINE_STRIP;
    case PrimitiveTopology::TriangleList:
        return GL_TRIANGLES;
    case PrimitiveTopology::TriangleStrip:
        return GL_TRIANGLE_STRIP;
    default: ;
    }

    return GL_INVALID_ENUM;
}

} // namespace Toon::OpenGL

#endif // TOON_OPENGL_PRIMITIVE_HPP