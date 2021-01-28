#ifndef TOON_OPENGL_TEXTURE_HPP
#define TOON_OPENGL_TEXTURE_HPP

#include <Toon/OpenGL/OpenGLConfig.hpp>
#include <Toon/Texture.hpp>

namespace Toon::OpenGL {

class TOON_OPENGL_API OpenGLTexture : public Texture
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

} // namespace Toon::OpenGL

#endif // TOON_OPENGL_TEXTURE_HPP