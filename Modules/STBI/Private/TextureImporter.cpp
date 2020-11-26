#include <Temporality/STBI/TextureImporter.hpp>
#include <Temporality/Log.hpp>

#define STB_NO_HDR
#define STB_NO_PSD
#define STB_NO_PIC
#define STB_NO_PNM
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace Temporality::STBI {

TEMPORALITY_STBI_API
TextureData::~TextureData()
{
    stbi_image_free(Data);
}

TEMPORALITY_STBI_API
std::unique_ptr<Temporality::TextureData> TextureImporter::LoadFromFile(const std::string& filename)
{
    BenchmarkStart();
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

    auto tex = std::make_unique<TextureData>();
    tex->Data = data;
    tex->Size = size;
    tex->Components = components;

    LogInfo("Loaded '%s'", filename);

    BenchmarkEnd("STBI::TextureImporter::LoadFromFile");
    return tex;
}

TEMPORALITY_STBI_API
std::unique_ptr<Temporality::TextureData> TextureImporter::LoadFromMemory(const uint8_t * buffer, size_t length)
{
    BenchmarkStart();
    glm::ivec2 size;
    int components;

    uint8_t * data = stbi_load_from_memory(buffer, length, &size.x, &size.y, &components, 0);
    if (!data) {
        return nullptr;
    }

    auto tex = std::make_unique<TextureData>();
    tex->Data = data;
    tex->Size = size;
    tex->Components = components;
    
    BenchmarkEnd("STBI::TextureImporter::LoadFromMemory");
    return tex;
}

}