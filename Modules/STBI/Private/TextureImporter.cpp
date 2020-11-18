#include <Temporality/STBI/TextureImporter.hpp>

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
    glm::ivec2 size;
    int components;

    uint8_t* data = stbi_load(filename.c_str(), &size.x, &size.y, &components, 0);
    if (!data) {
        return nullptr;
    }

    auto tex = std::make_unique<TextureData>();
    tex->Data = data;
    tex->Size = size;
    tex->Components = components;

    return tex;
}

TEMPORALITY_STBI_API
std::unique_ptr<Temporality::TextureData> TextureImporter::LoadFromMemory(uint8_t * buffer, size_t length)
{
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
    
    return tex;
}

}