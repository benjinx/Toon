#ifndef TOON_MATERIAL_HPP
#define TOON_MATERIAL_HPP

#include <Toon/Config.hpp>
#include <Toon/String.hpp>
#include <Toon/Texture.hpp>

#include <Toon/ShaderMaterial.hpp>

#include <memory>

namespace Toon {

class TOON_ENGINE_API Material
{
public:

    DISALLOW_COPY_AND_ASSIGN(Material)

    Material() = default;

    virtual ~Material() = default;

private:

    glm::vec4 _baseColorFactor  = glm::vec4(1.0f);
    glm::vec3 _emissiveFactor   = glm::vec3(1.0f);

    float _metallicFactor       = 1.0f;
    float _roughnessFactor      = 1.0f;
    float _occlusionStrength    = 1.0f;
    float _normalScale          = 1.0f;

    std::shared_ptr<Texture> _baseColorMap;
    std::shared_ptr<Texture> _normalMap;
    std::shared_ptr<Texture> _metallicRoughnessMap;
    std::shared_ptr<Texture> _emissiveMap;
    std::shared_ptr<Texture> _occlusionMap;

};

} // namespace Toon


    // Lambert (Phong/BlinnPhong)
    // - Ambient
    // - Diffuse
    // - Specular*
    // - Shininess*
    // * (not exactly used in Lambert due to removing the 'matte' effect that is lambert)

    // Principled BSDF (Ref: Blender)
    // - Subsurface
    // - Subsurface Radius
    // - Subsurface Color
    // - Base Color
    // - Metallic
    // - Specular
    // - Specular Tint
    // - Roughness
    // - Anisotropic
    // - Anisotropic Rotation
    // - Sheen
    // - Sheen Tint
    // - Clearcoat
    // - Clearcoat Roughness
    // - IOR
    // - Transmission
    // - Transmission Roughness
    // - Emission
    // - Alpha
    // - Normal
    // - Clearcoat Normal
    // - Tangent

    // PBR (Ref: Unreal Engine)
    // - Base Color
    // - Metallic
    // - Specular
    // - Roughness
    // - Anisotropy
    // - Emissive Color
    // - Opacity
    // - Opacity Mask
    // - Normal
    // - Tangent
    // - World Position Offset
    // - World Displacement
    // - Tessellation Multiplier
    // - Subsurface Color
    // - Custom Data 0?
    // - Custom Data 1?
    // - Ambient Occlusion
    // - Refraction
    // - Pixel Depth Offset
    // - Shading Model

    // Bare Minimum:
    // - Base Color
    // - Metallic
    // - Roughness
    // - Emissive
    // - Ambient Occlusion
    // - Normal


#endif // TOON_MATERIAL_HPP