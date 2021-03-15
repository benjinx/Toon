#ifndef TOON_SHADER_MATERIAL_HPP
#define TOON_SHADER_MATERIAL_HPP

#include <Toon/Config.hpp>

namespace Toon {

#define TOON_SHADER_MATERIAL_BINDING 2

struct TOON_ENGINE_API ShaderMaterial
{
public:
    alignas(16) glm::vec4 BaseColorFactor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    alignas(16) glm::vec3 EmissiveFactor = vec3(1.0f, 1.0f, 1.0f);
    alignas(4) float NormalScale = 1.0f;
    alignas(4) float MetallicFactor = 1.0f;
    alignas(4) float RoughnessFactor = 1.0f;
    alignas(4) float OcclusionStrength = 1.0f;

}; // struct ShaderMaterial

enum class MaterialTexture
{
    BaseColorMap,
    NormalMap,
    MetallicRoughnessMap,
    EmissiveMap,
    OcclusionMap,

}; // enum class MaterialTexture

inline string MaterialTextureToString(MaterialTexture materialTexture)
{
    switch (materialTexture) {
    case MaterialTexture::BaseColorMap:
        return "BaseColorMap";
    case MaterialTexture::NormalMap:
        return "NormalMap";
    case MaterialTexture::MetallicRoughnessMap:
        return "MetallicRoughnessMap";
    case MaterialTexture::EmissiveMap:
        return "EmissiveMap";
    case MaterialTexture::OcclusionMap:
        return "OcclusionMap";
    }

    return "Unknown";
}

inline uint32_t GetMaterialTextureBinding(MaterialTexture materialTexture)
{
    switch (materialTexture) {
    case MaterialTexture::BaseColorMap:
        return 0;
    case MaterialTexture::NormalMap:
        return 1;
    case MaterialTexture::MetallicRoughnessMap:
        return 2;
    case MaterialTexture::EmissiveMap:
        return 3;
    case MaterialTexture::OcclusionMap:
        return 4;
    }

    return UINT32_MAX;
}

} // namespace Toon

#endif // TOON_SHADER_MATERIAL_HPP