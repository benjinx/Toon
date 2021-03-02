#ifndef TOON_OPENGL_TEXTURE_HPP
#define TOON_OPENGL_TEXTURE_HPP

#include <Toon/OpenGL/OpenGLConfig.hpp>
#include <Toon/Texture.hpp>

namespace Toon::OpenGL {

#define TOON_OPENGL_TEXTURE(x) (dynamic_cast<Toon::OpenGL::OpenGLTexture *>(x))

class TOON_OPENGL_API OpenGLTexture : public Texture
{
public:

    DISALLOW_COPY_AND_ASSIGN(OpenGLTexture)

    OpenGLTexture() = default;

    virtual ~OpenGLTexture();

    bool Load(const std::unique_ptr<TextureData>& data, Options opts = Options()) override;

    void Bind();

private:

    GLenum GetGLDataFormat(int components);
    
    GLenum GetGLDataType(const TextureDataType& type);

    GLenum GetGLWrapType(const TextureWrapType& type);

    GLenum GetGLFilterType(const TextureFilterType& type);

    GLuint _glID = 0;
    
}; // class OpenGLTexture

} // namespace Toon::OpenGL

#endif // TOON_OPENGL_TEXTURE_HPP