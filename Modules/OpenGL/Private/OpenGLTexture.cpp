#include <Toon/OpenGL/OpenGLTexture.hpp>
#include <Toon/Log.hpp>

namespace Toon::OpenGL {

TOON_OPENGL_API
OpenGLTexture::~OpenGLTexture()
{
    glDeleteTextures(1, &_mGLid);
}

TOON_OPENGL_API
bool OpenGLTexture::Load(const TextureData * data)
{
    if (_mGLid)
    {
        glDeleteTextures(1, &_mGLid);
        _mGLid = 0;
    }

    glGenTextures(1, &_mGLid);
    if (!_mGLid)
    {
        LogError("Failed to create GL Texture");
        return false;
    }

    glBindTexture(GL_TEXTURE_2D, _mGLid);

    LogVerbose("Binding texture to id %u", _mGLid);

    // Texture wrapping params
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, );
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, );

    // Texture filtering params
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, );
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, );

    // Create image
    const auto& size = data->GetSize();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data->GetData());

    /*if (data->GenerateMipmaps()) {
        glGenerateMipmap(GL_TEXTURE_2D);
    }*/

    glBindTexture(GL_TEXTURE_2D, 0);

    return true;
}

TOON_OPENGL_API
void OpenGLTexture::Bind()
{
    glBindTexture(GL_TEXTURE_2D, _mGLid);
}

} // namespace Toon::OpenGL