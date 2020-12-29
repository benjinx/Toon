#ifndef TEMPORALITY_OPENGL_TEXTURE_HPP
#define TEMPORALITY_OPENGL_TEXTURE_HPP

#include <Temporality/OpenGL/OpenGLConfig.hpp>
#include <Temporality/Graphics/Texture.hpp>

namespace Temporality::OpenGL {

class TEMPORALITY_OPENGL_API OpenGLTexture : public Texture
{
public:

    DISALLOW_COPY_AND_ASSIGN(OpenGLTexture)

    OpenGLTexture() = default;

    virtual ~OpenGLTexture();

    bool Load(const TextureData * data) override;

    void Bind() override;

private:
    GLuint _mGLid = 0;
    
}; // class OpenGLTexture

} // namespace Temporality::OpenGL

#endif // TEMPORALITY_OPENGL_TEXTURE_HPP