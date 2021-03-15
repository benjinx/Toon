#ifndef TOON_MATERIAL_HPP
#define TOON_MATERIAL_HPP

#include <Toon/Config.hpp>
#include <Toon/Math.hpp>
#include <Toon/Buffer.hpp>
#include <Toon/Texture.hpp>
#include <Toon/ShaderMaterial.hpp>

namespace Toon {

class TOON_ENGINE_API Material
{
public:

    DISALLOW_COPY_AND_ASSIGN(Material)

    Material() = default;

    virtual ~Material() = default;

    virtual void Initialize();

    virtual void Terminate();

    virtual inline glm::vec4 GetBaseColorFactor() const {
        return _shaderMaterial.BaseColorFactor;
    }

    virtual inline glm::vec3 GetEmissiveFactor() const {
        return _shaderMaterial.EmissiveFactor;
    }

    virtual inline float GetMetallicFactor() const {
        return _shaderMaterial.MetallicFactor;
    }

    virtual inline float GetRoughnessFactor() const {
        return _shaderMaterial.RoughnessFactor;
    }

    virtual inline float GetOcclusionStrength() const {
        return _shaderMaterial.OcclusionStrength;
    }

    virtual inline float GetNormalScale() const {
        return _shaderMaterial.NormalScale;
    }

    virtual inline void SetBaseColorFactor(glm::vec4 factor) {
        _shaderMaterial.BaseColorFactor = factor;
        UpdateBuffer();
    }

    virtual inline void SetEmissiveFactor(glm::vec3 factor) {
        _shaderMaterial.EmissiveFactor = factor;
        UpdateBuffer();
    }

    virtual inline void SetMetallicFactor(float factor) {
        _shaderMaterial.MetallicFactor = factor;
        UpdateBuffer();
    }

    virtual inline void SetRoughnessFactor(float factor) {
        _shaderMaterial.RoughnessFactor = factor;
        UpdateBuffer();
    }

    virtual inline void SetOcclusionStrength(float strength) {
        _shaderMaterial.OcclusionStrength = strength;
        UpdateBuffer();
    }

    virtual inline void SetNormalScale(float scale) {
        _shaderMaterial.NormalScale = scale;
        UpdateBuffer();
    }

    virtual inline void SetBaseColorMap(std::shared_ptr<Texture> map) {
        _baseColorMap = map;
    }

    virtual inline void SetNormalMap(std::shared_ptr<Texture> map) {
        _normalMap = map;
    }

    virtual inline void SetMetallicRoughnessMap(std::shared_ptr<Texture> map) {
        _metallicRoughnessMap = map;
    }

    virtual inline void SetEmissiveMap(std::shared_ptr<Texture> map) {
        _emissiveMap = map;
    }

    virtual inline void SetOcclusionMap(std::shared_ptr<Texture> map) {
        _occlusionMap = map;
    }

    virtual inline std::shared_ptr<Texture> GetBaseColorMap() {
        return _baseColorMap;
    }

    virtual inline std::shared_ptr<Texture> GetNormalMap() {
        return _normalMap;
    }

    virtual inline std::shared_ptr<Texture> GetMetallicRoughnessMap() {
        return _metallicRoughnessMap;
    }

    virtual inline std::shared_ptr<Texture> GetEmissiveMap() {
        return _emissiveMap;
    }

    virtual inline std::shared_ptr<Texture> GetOcclusionMap() {
        return _occlusionMap;
    }

    // TODO: Fix buffer thrashing when setting multiple variables
    virtual void UpdateBuffer();

protected:

    std::shared_ptr<Buffer> _shaderMaterialBuffer;

    ShaderMaterial _shaderMaterial;

    std::shared_ptr<Texture> _baseColorMap;

    std::shared_ptr<Texture> _normalMap;

    std::shared_ptr<Texture> _metallicRoughnessMap;

    std::shared_ptr<Texture> _emissiveMap;

    std::shared_ptr<Texture> _occlusionMap;

}; // class Material

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