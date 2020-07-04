#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <Config.hpp>
#include <Math.hpp>
#include <OpenGL.hpp>

class Texture
{
public:
    struct Options
    {
        Options()
            : WrapS(GL_REPEAT)
            , WrapT(GL_REPEAT)
            , MagFilter(GL_NEAREST)
            , MinFilter(GL_NEAREST)
            , Mipmap(true)
        { }

        GLenum WrapS;
        GLenum WrapT;

        GLenum MagFilter;
        GLenum MinFilter;

        bool Mipmap;
    };

    Texture() = default;
    Texture(const std::string& filename, Options opts = Options());
    Texture(unsigned char* data, glm::ivec2 size, int comp = 4, Options opts = Options());
    Texture(GLuint&& id, glm::ivec2 size);
    Texture(Texture&& rhs);

    ~Texture();

    // Load from a file
    bool Load(const std::string& filename, Options opts = Options());

    // Load from a buffer
    bool Load(unsigned char* buffer, glm::ivec2 size, int comp = 4, Options opts = Options());

    void Bind();

private:
    bool _mLoaded = false;

    GLuint _mglID = 0;

    glm::ivec2 _mSize;
};

#endif //TEXTURE_HPP