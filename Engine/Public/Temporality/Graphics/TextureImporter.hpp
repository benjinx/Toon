#ifndef TEMPORALITY_TEXTURE_IMPORTER_HPP
#define TEMPORALITY_TEXTURE_IMPORTER_HPP

#include <Temporality/Config.hpp>
#include <Temporality/Math.hpp>

#include <memory>
#include <string>

namespace Temporality {

class TEMPORALITY_ENGINE_API TextureData
{
public:

    DISALLOW_COPY_AND_ASSIGN(TextureData)

    explicit TextureData() = default;

    virtual ~TextureData() = default;

    // Pointer to pixel data Buffer
    virtual uint8_t* GetData() const = 0;

    // Width and Height in pixels
    virtual glm::uvec2 GetSize() const = 0;

    // Number of components(R, RG, RGB, RGBA)
    virtual int GetComponents() const = 0;
};

class TEMPORALITY_ENGINE_API TextureImporter
{
public:

    DISALLOW_COPY_AND_ASSIGN(TextureImporter)

    explicit TextureImporter() = default;

    virtual ~TextureImporter() = default;

    virtual std::unique_ptr<TextureData> LoadFromFile(const std::string& filename) = 0;

    virtual std::unique_ptr<TextureData> LoadFromMemory(uint8_t * buffer, size_t length) = 0;
};

} // namespace Temporality

#endif // TEMPORALITY_TEXTURE_IMPORTER_HPP