#include "Material.hpp"
#include <Temporality.hpp>
#include "Shader.hpp"
#include "Texture.hpp"

Material::Material(glm::vec3 ambient,
                   glm::vec3 diffuse,
                   glm::vec3 specular,
                   float shininess,
                   std::shared_ptr<Texture> ambientMap,
                   std::shared_ptr<Texture> diffuseMap,
                   std::shared_ptr<Texture> specularMap,
                   std::shared_ptr<Texture> normalMap)
                   : _mAmbient(ambient)
                   , _mDiffuse(diffuse)
                   , _mSpecular(specular)
                   , _mShininess(shininess)
                   , _mAmbientMap(ambientMap)
                   , _mDiffuseMap(diffuseMap)
                   , _mSpecularMap(specularMap)
                   , _mNormalMap(normalMap) {}

void Material::Bind(Shader* shader)
{
    shader->Use();

    shader->SetVec3("material.ambient", _mAmbient);
    shader->SetVec3("material.diffuse", _mDiffuse);
    shader->SetVec3("material.specular", _mSpecular);
    shader->SetVec3("material.emissive", _mEmissive);

    shader->SetFloat("material.roughness", _mRoughness);
    shader->SetFloat("material.metallic", _mMetallic);
    shader->SetFloat("material.shininess", _mShininess);
    shader->SetFloat("material.dissolve", _mDissolve);
    shader->SetFloat("material.sheen", _mSheen);
    shader->SetFloat("material.clearcoatThickness", _mClearcoatThickness);
    shader->SetFloat("material.clearcoatRoughness", _mClearcoatRoughness);
    shader->SetFloat("material.anisotropy", _mAnisotropy);
    shader->SetFloat("material.anisotropyRotation", _mAnisotropyRotation);

    if (_mAmbientMap)
    {
        shader->SetInt("material.ambientMap", Material::TextureID::AMBIENT);
        glActiveTexture(GL_TEXTURE0 + Material::TextureID::AMBIENT);
        _mAmbientMap->Bind();
    }
    
    if (_mDiffuseMap)
    {
        shader->SetInt("material.diffuseMap", Material::TextureID::DIFFUSE);
        glActiveTexture(GL_TEXTURE0 + Material::TextureID::DIFFUSE);
        _mDiffuseMap->Bind();
    }

    if (_mSpecularMap)
    {
        shader->SetInt("material.specularMap", Material::TextureID::SPECULAR);
        glActiveTexture(GL_TEXTURE0 + Material::TextureID::SPECULAR);
        _mSpecularMap->Bind();
    }

    if (_mNormalMap)
    {
        shader->SetInt("material.normalMap", Material::TextureID::NORMAL);
        glActiveTexture(GL_TEXTURE0 + Material::TextureID::NORMAL);
        _mNormalMap->Bind();
    }

    if (_mAlphaMap)
    {
        shader->SetInt("material.alphaMap", Material::TextureID::ALPHA);
        glActiveTexture(GL_TEXTURE0 + Material::TextureID::ALPHA);
        _mAlphaMap->Bind();
    }

    if (_mDisplacementMap)
    {
        shader->SetInt("material.displacementMap", Material::TextureID::DISPLACEMENT);
        glActiveTexture(GL_TEXTURE0 + Material::TextureID::DISPLACEMENT);
        _mDisplacementMap->Bind();
    }

    if (_mMetallicRoughnessMap)
    {
        shader->SetInt("material.metallicRoughnessMap", Material::TextureID::METALLIC_ROUGHNESS);
        glActiveTexture(GL_TEXTURE0 + Material::TextureID::METALLIC_ROUGHNESS);
        _mMetallicRoughnessMap->Bind();
    }

    if (_mSheenMap)
    {
        shader->SetInt("material.sheenMap", Material::TextureID::SHEEN);
        glActiveTexture(GL_TEXTURE0 + Material::TextureID::SHEEN);
        _mSheenMap->Bind();
    }

    if (_mEmissiveMap)
    {
        shader->SetInt("material.emissiveMap", Material::TextureID::EMISSIVE);
        glActiveTexture(GL_TEXTURE0 + Material::TextureID::EMISSIVE);
        _mEmissiveMap->Bind();
    }
}
