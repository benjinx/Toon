#include "Material.hpp"

#include <Shader.hpp>
#include <Texture.hpp>

void Material::Bind(Shader* shader)
{
    shader->Use();

    shader->SetVec3("material.diffuse", _mDiffuse);
    shader->SetVec3("material.emissive", _mEmissive);

    shader->SetFloat("material.roughness", _mRoughness);
    shader->SetFloat("material.metallic", _mMetallic);
	shader->SetFloat("material.normalScale", _mNormalScale);
	shader->SetFloat("material.occlusionStength", _mOcclusionStrength);

    if (_mDiffuseMap)
    {
		shader->SetBool("material.hasDiffuseMap", true);
        shader->SetInt("material.diffuseMap", Material::TextureID::DIFFUSE);
        glActiveTexture(GL_TEXTURE0 + Material::TextureID::DIFFUSE);
        _mDiffuseMap->Bind();
    }
	else
	{
		shader->SetBool("material.hasDiffuseMap", false);
	}

	if (_mMetallicRoughnessMap)
	{
		shader->SetBool("material.hasMetallicRoughnessMap", true);
		shader->SetInt("material.metallicRoughnessMap", Material::TextureID::METALLIC_ROUGHNESS);
		glActiveTexture(GL_TEXTURE0 + Material::TextureID::METALLIC_ROUGHNESS);
		_mMetallicRoughnessMap->Bind();
	}
	else
	{
		shader->SetBool("material.hasMetallicRoughnessMap", false);
	}

    if (_mNormalMap)
    {
		shader->SetBool("material.hasNormalMap", true);
        shader->SetInt("material.normalMap", Material::TextureID::NORMAL);
        glActiveTexture(GL_TEXTURE0 + Material::TextureID::NORMAL);
        _mNormalMap->Bind();
    }
	else
	{
        shader->SetBool("material.hasNormalMap", false);
	}


    if (_mEmissiveMap)
    {
        shader->SetBool("material.hasEmissiveMap", true);
        shader->SetInt("material.emissiveMap", Material::TextureID::EMISSIVE);
        glActiveTexture(GL_TEXTURE0 + Material::TextureID::EMISSIVE);
        _mEmissiveMap->Bind();
    }
    else
    {
        shader->SetBool("material.hasEmissiveMap", false);
    }

	if (_mOcclusionMap)
	{
		shader->SetBool("material.hasOcclusionMap", true);
		shader->SetInt("material.occlusionMap", Material::TextureID::OCCLUSION);
		glActiveTexture(GL_TEXTURE0 + Material::TextureID::OCCLUSION);
		_mOcclusionMap->Bind();
	}
	else
	{
		shader->SetBool("material.hasOcclusionMap", false);
	}
}
