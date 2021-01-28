#ifndef TOON_TEXTURE_IMPORTER_HPP
#define TOON_TEXTURE_IMPORTER_HPP

#include <Toon/Config.hpp>
#include <Toon/Math.hpp>

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace Toon {

class TOON_ENGINE_API TextureData
{
public:

    DISALLOW_COPY_AND_ASSIGN(TextureData)

    TextureData() = default;

    virtual ~TextureData() = default;

    // Pointer to pixel data Buffer
    virtual uint8_t* GetData() const = 0;

    // Width and Height in pixels
    virtual glm::uvec2 GetSize() const = 0;

    // Number of components(R, RG, RGB, RGBA)
    virtual int GetComponents() const = 0;
}; // class TextureData

class TOON_ENGINE_API TextureImporter
{
public:

    DISALLOW_COPY_AND_ASSIGN(TextureImporter)

    TextureImporter() = default;

    virtual ~TextureImporter() = default;

    virtual std::unique_ptr<TextureData> LoadFromFile(const std::string& filename) = 0;

    virtual std::unique_ptr<TextureData> LoadFromMemory(const uint8_t * buffer, size_t length) = 0;

}; // class TextureImporter

TOON_ENGINE_API
void AddTextureImporter(const std::string& id, std::unique_ptr<TextureImporter> importer);

TOON_ENGINE_API
void RemoveTextureImporter(const std::string& id);

TOON_ENGINE_API
const std::vector<TextureImporter *>& GetAllTextureImporters();

} // namespace Toon

#endif // TOON_TEXTURE_IMPORTER_HPP