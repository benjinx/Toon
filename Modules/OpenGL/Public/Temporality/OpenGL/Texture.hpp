#ifndef TEMPORALITY_OPENGL_TEXTURE_HPP
#define TEMPORALITY_OPENGL_TEXTURE_HPP

#include <Temporality/OpenGL/Config.hpp>
#include <Temporality/Graphics/Texture.hpp>

namespace Temporality::OpenGL {

class TEMPORALITY_OPENGL_API Texture : public Temporality::Texture
{
public:

    DISALLOW_COPY_AND_ASSIGN(Texture)

    Texture() = default;

    virtual ~Texture();

    bool Load(const TextureData * data) override;

    void Bind() override;

private:
    GLuint _mGLid = 0;
    
};

}
#endif // TEMPORALITY_OPENGL_TEXTURE_HPP