#include <Toon/OpenGL/OpenGLBuffer.hpp>

#include <Toon/Log.hpp>

namespace Toon::OpenGL {

TOON_OPENGL_API
bool OpenGLBuffer::Initialize(size_t size, uint8_t * data, BufferUsage bufferUsage, MemoryUsage memoryUsage)
{
    _bufferUsage = bufferUsage;
    _memoryUsage = memoryUsage;
    _size = size;

    if (!data && _memoryUsage == MemoryUsage::GPU) {
        LogError("Attempting to create an empty buffer with MemoryUsage GPU");
        return false;
    }

    _glTarget = GetGLBufferUsage(_bufferUsage);
    if (_glTarget == GL_INVALID_ENUM) {
        return false;
    }

    GLbitfield flags = GetGLMemoryUsage(_memoryUsage);

    glGenBuffers(1, &_glID);
    glBindBuffer(_glTarget, _glID);
    glBufferStorage(_glTarget, _size, data, flags);
    glBindBuffer(_glTarget, 0);

    return true;
}

TOON_OPENGL_API
void OpenGLBuffer::Terminate()
{
    glDeleteBuffers(1, &_glID);
}

TOON_OPENGL_API
bool OpenGLBuffer::ReadFrom(size_t offset, size_t length, uint8_t * data)
{
    if (_memoryUsage != MemoryUsage::Download) {
        LogError("Unable to read data from buffer with MemoryUsage: %s",
            MemoryUsageToString(_memoryUsage));
        return false;
    }

    GLbitfield flags = GetGLMemoryUsage(_memoryUsage);

    glBindBuffer(_glTarget, _glID);

    void * ptr;
    ptr = glMapBufferRange(_glTarget, offset, length, flags);
    if (!ptr) {
        LogError("glMapBufferRange() failed");
        return false;
    }

    memcpy(data, ptr, length);

    glUnmapBuffer(_glTarget);

    glBindBuffer(_glTarget, 0);

    return true;
}

TOON_OPENGL_API
bool OpenGLBuffer::WriteTo(size_t offset, size_t length, uint8_t * data)
{
    if (_memoryUsage != MemoryUsage::UploadOnce && _memoryUsage != MemoryUsage::UploadOften) {
        LogError("Unable to write data to buffer with MemoryUsage: %s",
            MemoryUsageToString(_memoryUsage));
        return false;
    }

    GLbitfield flags = GetGLMemoryUsage(_memoryUsage);
    
    glBindBuffer(_glTarget, _glID);

    void * ptr;
    ptr = glMapBufferRange(_glTarget, offset, length, flags);
    if (!ptr) {
        LogError("glMapBufferRange() failed");
        return false;
    }

    memcpy(ptr, data, length);

    glUnmapBuffer(_glTarget);

    glBindBuffer(_glTarget, 0);

    return true;
}

TOON_OPENGL_API
void OpenGLBuffer::Bind()
{
    glBindBuffer(_glTarget, _glID);
}

} // namespace Toon::OpenGL