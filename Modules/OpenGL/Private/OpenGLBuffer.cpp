#include <Toon/OpenGL/OpenGLBuffer.hpp>

#include <Toon/Log.hpp>

#include <Toon/String.hpp>

namespace Toon::OpenGL {

TOON_OPENGL_API
bool OpenGLBuffer::Initialize(size_t size, uint8_t * data, BufferUsage bufferUsage, MemoryUsage memoryUsage)
{
    _bufferUsage = bufferUsage;
    _memoryUsage = memoryUsage;
    _size = size;

    if (!data && _memoryUsage == MemoryUsage::GPU) {
        ToonLogError("Attempting to create an empty buffer with MemoryUsage GPU");
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

    // TODO: Limit to only mappable memory usages
    _mappedBufferMemory = glMapBufferRange(_glTarget, 0, _size, flags);
    if (!_mappedBufferMemory) {
        ToonLogError("glMapBuffer() failed");
        return false;
    }

    glBindBuffer(_glTarget, 0);

    return true;
}

TOON_OPENGL_API
void OpenGLBuffer::Terminate()
{
    if (_mappedBufferMemory) {
        glBindBuffer(_glTarget, _glID);
        glUnmapBuffer(_glTarget);
        glBindBuffer(_glTarget, 0);
        
        _mappedBufferMemory = nullptr;
    }

    glDeleteBuffers(1, &_glID);
}

TOON_OPENGL_API
void OpenGLBuffer::Bind()
{
    glBindBuffer(_glTarget, _glID);
}

} // namespace Toon::OpenGL