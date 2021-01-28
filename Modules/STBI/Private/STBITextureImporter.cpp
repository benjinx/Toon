#include <Toon/STBI/STBITextureImporter.hpp>
#include <Toon/Log.hpp>
#include <Toon/Benchmark.hpp>

#define STB_NO_HDR
#define STB_NO_PSD
#define STB_NO_PIC
#define STB_NO_PNM
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace Toon::STBI {

TOON_STBI_API
STBITextureData::~STBITextureData()
{
    stbi_image_free(Data);
}

TOON_STBI_API
std::unique_ptr<TextureData> STBITextureImporter::LoadFromFile(const std::string& filename)
{
    ToonBenchmarkStart();
    glm::ivec2 size;
    int components;
    uint8_t* data = nullptr;

    const auto& assetPaths = GetAssetPaths();

    for (const auto& path : assetPaths)
    {
        std::string fullPath = path + filename;
        data = stbi_load(fullPath.c_str(), &size.x, &size.y, &components, STBI_rgb_alpha);
        if (data) {
            break;
        }
    }

    if (!data) {
        return nullptr;
    }

    auto tex = std::make_unique<STBITextureData>();
    tex->Data = data;
    tex->Size = size;
    tex->Components = components;

    ToonLogInfo("Loaded '%s'", filename);

    ToonBenchmarkEnd();
    return tex;
}

TOON_STBI_API
std::unique_ptr<TextureData> STBITextureImporter::LoadFromMemory(const uint8_t * buffer, size_t length)
{
    ToonBenchmarkStart();
    glm::ivec2 size;
    int components;

    uint8_t * data = stbi_load_from_memory(buffer, length, &size.x, &size.y, &components, 0);
    if (!data) {
        return nullptr;
    }

    auto tex = std::make_unique<STBITextureData>();
    tex->Data = data;
    tex->Size = size;
    tex->Components = components;
    
    ToonBenchmarkEnd();
    return tex;
}

} // namespace Toon::STBI