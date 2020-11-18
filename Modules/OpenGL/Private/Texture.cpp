#include <Temporality/OpenGL/Texture.hpp>
#include <Temporality/Log.hpp>

namespace Temporality::OpenGL {

TEMPORALITY_OPENGL_API
Texture::~Texture()
{
    glDeleteTextures(1, &_mGLid);
}

TEMPORALITY_OPENGL_API
bool Texture::Load(const TextureData * data)
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

    LogVerbose(GL_TEXTURE_2D, 0);

    return true;
}

TEMPORALITY_OPENGL_API
void Texture::Bind()
{
    glBindTexture(GL_TEXTURE_2D, _mGLid);
}

}