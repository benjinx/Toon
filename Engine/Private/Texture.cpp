#include <Toon/Texture.hpp>
#include <Toon/TextureImporter.hpp>
#include <Toon/GraphicsDriver.hpp>
#include <Toon/Log.hpp>

namespace Toon {

TOON_ENGINE_API
std::shared_ptr<Texture> LoadTextureFromFile(const string& filename, bool useAssetPath /*= true*/, Texture::Options opts /*= Texture::Options()*/)
{
    GraphicsDriver * gfx = GetGraphicsDriver();

    const auto& importers = GetAllTextureImporters();
    for (const auto& importer : importers) {
        auto textureData = importer->LoadFromFile(filename, useAssetPath);
        if (!textureData) {
            continue;
        }

        auto texture = gfx->CreateTexture();

        if (!texture->Load(textureData, opts)) {
            break;
        }

        return texture;
    }

    ToonLogError("Failed to load texture '%s'", filename);
    return nullptr;
}

TOON_ENGINE_API
std::shared_ptr<Texture> LoadTextureFromMemory(const uint8_t* buffer, size_t length, Texture::Options opts /*= Texture::Options()*/)
{
    GraphicsDriver * gfx = GetGraphicsDriver();

    const auto& importers = GetAllTextureImporters();
    for (const auto& importer : importers) {
        auto textureData = importer->LoadFromMemory(buffer, length);
        if (!textureData) {
            continue;
        }

        auto texture = gfx->CreateTexture();

        if (!texture->Load(textureData, opts)) {
            break;
        }

        return texture;
    }

    ToonLogError("Failed to load texture from memory");
    return nullptr;
}

} // namespace Toon