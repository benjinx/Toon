#include "Material.hpp"

#include <Shader.hpp>
#include <Texture.hpp>
#include <Log.hpp>

void Material::Bind(Shader* shader)
{
    shader->Use();

    // Set Materials
    shader->SetVec3("material.diffuse", _mDiffuse);
    shader->SetVec3("material.ambient", _mAmbient);
    shader->SetVec3("material.emissive", _mEmissive);
    shader->SetVec3("material.transparent", _mTransparent);
    shader->SetVec3("material.reflective", _mReflective);

    // Set Textures

    for (auto& tex : _mTextures)
    {
        shader->SetBool("material." + GetHasMapVariableName(tex.first), true);
        shader->SetInt("material." + GetMapVariableName(tex.first), tex.first);
        glActiveTexture(GL_TEXTURE0 + tex.first);
        tex.second->Bind();
    }
}

std::string Material::GetMapVariableName(Material::TextureID id)
{
    switch (id) {
        case DIFFUSE:           return "diffuseMap";
        case SPECULAR:          return "specularMap";
        case AMBIENT:           return "ambientMap";
        case EMISSIVE:          return "emissiveMap";
        case HEIGHT:            return "heightMap";
        case NORMAL:            return "normalMap";
        case SHININESS:         return "shininessMap";
        case OPACITY:           return "opacityMap";
        case DISPLACEMENT:      return "displacementMap";
        case LIGHT_MAP:         return "lightMap";
        case REFLECTION:        return "reflectionMap";
        case BASE_COLOR:        return "baseColorMap";
        case NORMAL_CAMERA:     return "normalCameraMap";
        case EMISSION_COLOR:    return "emissionColorMap";
        case METALNESS:         return "metalnessMap";
        case DIFFUSE_ROUGHNESS: return "diffuseRoughnessMap";
        case AMBIENT_OCCLUSION: return "ambientOcclusionMap";
    }
    return "";
}

std::string Material::GetHasMapVariableName(Material::TextureID id)
{
    switch (id) {
    case DIFFUSE:           return "hasDiffuseMap";
    case SPECULAR:          return "hasSpecularMap";
    case AMBIENT:           return "hasAmbientMap";
    case EMISSIVE:          return "hasEmissiveMap";
    case HEIGHT:            return "hasHeightMap";
    case NORMAL:            return "hasNormalMap";
    case SHININESS:         return "hasShininessMap";
    case OPACITY:           return "hasOpacityMap";
    case DISPLACEMENT:      return "hasDisplacementMap";
    case LIGHT_MAP:         return "hasLightMap";
    case REFLECTION:        return "hasReflectionMap";
    case BASE_COLOR:        return "hasBaseColorMap";
    case NORMAL_CAMERA:     return "hasNormalCameraMap";
    case EMISSION_COLOR:    return "hasEmissionColorMap";
    case METALNESS:         return "hasMetalnessMap";
    case DIFFUSE_ROUGHNESS: return "hasDiffuseRoughnessMap";
    case AMBIENT_OCCLUSION: return "hasAmbientOcclusionMap";
    }
    return "";
}