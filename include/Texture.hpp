#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <Config.hpp>
#include <Math.hpp>
#include <OpenGL.hpp>

class Texture
{
public:
    Texture() = default;
    Texture(const std::string& filename);
    Texture(unsigned char* data, glm::ivec2 size, int comp = 4);
    Texture(GLuint&& id, glm::ivec2 size);
    Texture(Texture&& rhs);

    ~Texture();

    // Load from a file
    bool Load(const std::string& filename);

    // Load from a buffer
    bool Load(unsigned char* buffer, glm::ivec2 size, int comp = 4);

    void Bind();

private:
    bool _mLoaded = false;

    GLuint _mglID = 0;

    glm::ivec2 _mSize;
};

#endif //TEXTURE_HPP