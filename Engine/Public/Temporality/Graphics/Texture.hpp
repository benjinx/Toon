#ifndef TEMPORALITY_TEXTURE_HPP
#define TEMPORALITY_TEXTURE_HPP

#include <Temporality/Config.hpp>
#include <Temporality/Graphics/TextureImporter.hpp>

namespace Temporality {

class TEMPORALITY_ENGINE_API Texture
{
public:

    DISALLOW_COPY_AND_ASSIGN(Texture)
    
    explicit Texture() = default;

    virtual ~Texture();

    virtual bool Load(const TextureData * data) = 0;

    virtual void Bind() = 0;
}; // class Texture

} // namespace Temporality

#endif // TEMPORALITY_TEXTURE_HPP