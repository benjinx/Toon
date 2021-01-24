#ifndef TEMPORALITY_TEXTURE_HPP
#define TEMPORALITY_TEXTURE_HPP

#include <Temporality/Config.hpp>
#include <Temporality/TextureImporter.hpp>

namespace Temporality {

class TEMPORALITY_ENGINE_API Texture
{
public:

    DISALLOW_COPY_AND_ASSIGN(Texture)
    
    Texture() = default;

    virtual ~Texture() = default;

    virtual bool Load(const TextureData * data) = 0;

    virtual bool LoadFromFile(const std::string& filename);

    virtual bool LoadFromMemory(const uint8_t* buffer, size_t length);

    virtual void Bind() = 0;
}; // class Texture

} // namespace Temporality

#endif // TEMPORALITY_TEXTURE_HPP