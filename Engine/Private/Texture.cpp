#include <Toon/Texture.hpp>
#include <Toon/Log.hpp>

namespace Toon {

TOON_ENGINE_API
bool Texture::LoadFromFile(const std::string& filename)
{
    const auto& importers = GetAllTextureImporters();
    for (const auto& importer : importers) {
        auto data = importer->LoadFromFile(filename);
        if (data && Load(data.get())) {
            return true;
        }
    }

    LogError("Unable to find an importer for '%s'", filename);
    return false;
}

TOON_ENGINE_API
bool Texture::LoadFromMemory(const uint8_t* buffer, size_t length)
{
    const auto& importers = GetAllTextureImporters();
    for (const auto& importer : importers) {
        auto data = importer->LoadFromMemory(buffer, length);
        if (data && Load(data.get())) {
            return true;
        }
    }

    return false;
}



}