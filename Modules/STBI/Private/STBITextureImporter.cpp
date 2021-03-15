#include <Toon/STBI/STBITextureImporter.hpp>

#include <Toon/Log.hpp>
#include <Toon/Math.hpp>
#include <Toon/Benchmark.hpp>
#include <Toon/GraphicsDriver.hpp>
#include <Toon/STBI/STBITextureData.hpp>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-function"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#pragma clang diagnostic pop

#pragma GCC diagnostic pop

namespace Toon::STBI {

TOON_STBI_API
std::unique_ptr<TextureData> STBITextureImporter::LoadFromFile(const std::string& filename, bool useAssetPath)
{
    ToonBenchmarkStart();

    glm::ivec2 size;
    int components;
    uint8_t* data = nullptr;

    if (useAssetPath) {
        const auto& assetPathList = GetAssetPathList();

        for (const auto& path : assetPathList) {
            Path fullPath = path / "Textures" / filename;
            data = stbi_load(fullPath.ToCString(), &size.x, &size.y, &components, STBI_rgb_alpha);
            if (data) {
                break;
            }
        }
    }
    else {
        data = stbi_load(filename.c_str(), &size.x, &size.y, &components, STBI_rgb_alpha);
    }

    if (!data) {
        return nullptr;
    }

    ToonLogLoad("Loaded texture from '%s'", filename);

    STBITextureData * textureData = new STBITextureData();
    textureData->Data = data;
    textureData->Size = size;

    ToonBenchmarkEnd();
    return std::unique_ptr<TextureData>(textureData);
}

TOON_STBI_API
std::unique_ptr<TextureData> STBITextureImporter::LoadFromMemory(const uint8_t * buffer, size_t length)
{
    ToonBenchmarkStart();

    glm::ivec2 size;
    int components;

    uint8_t * data = stbi_load_from_memory(buffer, length, &size.x, &size.y, &components, STBI_rgb_alpha);
    if (!data) {
        return nullptr;
    }

    STBITextureData * textureData = new STBITextureData();
    textureData->Data = data;
    textureData->Size = size;
    
    ToonBenchmarkEnd();
    return std::unique_ptr<TextureData>(textureData);
}

} // namespace Toon::STBI