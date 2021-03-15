#include <Toon/OpenGL/OpenGLTexture.hpp>

#include <Toon/Benchmark.hpp>
#include <Toon/Log.hpp>

namespace Toon::OpenGL {

TOON_OPENGL_API
OpenGLTexture::~OpenGLTexture()
{
    if (_glID) {
        glDeleteTextures(1, &_glID);
        _glID = 0;
    }
}

TOON_OPENGL_API
bool OpenGLTexture::Load(const std::unique_ptr<TextureData>& data, Options opts /*= Options()*/)
{
    ToonBenchmarkStart();

    if (_glID) {
        glDeleteTextures(1, &_glID);
        _glID = 0;
    }

    glGenTextures(1, &_glID);
    if (!_glID) {
        ToonLogError("Failed to create GL Texture");
        return false;
    }

    GLenum glType = GetGLDataType(data->GetDataType());

    glBindTexture(GL_TEXTURE_2D, _glID);

    const auto& size = data->GetSize();

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, glType, data->GetData());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GetGLWrapType(opts.WrapS));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GetGLWrapType(opts.WrapT));

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GetGLFilterType(opts.MagFilter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GetGLFilterType(opts.MinFilter));

    ToonLogVerbose("Bound texture to ID %d", _glID);

    glBindTexture(GL_TEXTURE_2D, 0);

    ToonBenchmarkEnd();
    return true;
}

TOON_OPENGL_API
GLenum OpenGLTexture::GetGLDataType(const TextureDataType& type)
{
    switch (type) {
    case TextureDataType::UnsignedByte:
        return GL_UNSIGNED_BYTE;
    case TextureDataType::Byte:
        return GL_BYTE;
    case TextureDataType::UnsignedShort:
        return GL_UNSIGNED_SHORT;
    case TextureDataType::Short:
        return GL_SHORT;
    case TextureDataType::UnsignedInt:
        return GL_UNSIGNED_INT;
    case TextureDataType::Int:
        return GL_INT;
    }

    return GL_INVALID_ENUM;
}

TOON_OPENGL_API
GLenum OpenGLTexture::GetGLWrapType(const TextureWrapType& type)
{
    switch (type) {
    case TextureWrapType::Repeat:
        return GL_REPEAT;
    case TextureWrapType::MirroredRepeat:
        return GL_MIRRORED_REPEAT;
    case TextureWrapType::ClampToEdge:
        return GL_CLAMP_TO_EDGE;
    case TextureWrapType::ClampToBorder:
        return GL_CLAMP_TO_BORDER;
    }

    return GL_INVALID_ENUM;
}

TOON_OPENGL_API
GLenum OpenGLTexture::GetGLFilterType(const TextureFilterType& type)
{
    switch (type) {
    case TextureFilterType::Nearest:
        return GL_NEAREST;
    case TextureFilterType::NearestMipmapLinear:
        // return GL_NEAREST_MIPMAP_LINEAR;
        return GL_NEAREST;
    case TextureFilterType::NearestMipmapNearest:
        // return GL_NEAREST_MIPMAP_NEAREST;
        return GL_NEAREST;
    case TextureFilterType::Linear:
        return GL_LINEAR;
    case TextureFilterType::LinearMipmapLinear:
        // return GL_LINEAR_MIPMAP_LINEAR;
        return GL_LINEAR;
    case TextureFilterType::LinearMipmapNearest:
        // return GL_LINEAR_MIPMAP_NEAREST;
        return GL_LINEAR;
    }

    return GL_INVALID_ENUM;
}

} // namespace Toon::OpenGL