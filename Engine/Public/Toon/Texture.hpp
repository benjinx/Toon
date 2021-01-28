#ifndef TOON_TEXTURE_HPP
#define TOON_TEXTURE_HPP

#include <Toon/Config.hpp>
#include <Toon/TextureImporter.hpp>

namespace Toon {

class TOON_ENGINE_API Texture
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

} // namespace Toon

#endif // TOON_TEXTURE_HPP