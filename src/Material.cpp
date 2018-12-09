#include "Material.hpp"
#include "Utils.hpp"

Material::Material(float       ambient[3],
                   float       diffuse[3],
                   float       specular[3],
                   float       shininess,
                   std::string ambientFile,
                   std::string diffuseFile,
                   std::string specularFile,
                   std::string normalFile)
{
    _mAmbient  = {ambient[0], ambient[1], ambient[2]};
    _mDiffuse  = {diffuse[0], diffuse[1], diffuse[2]};
    _mSpecular = {specular[0], specular[1], specular[2]};

    _mShininess = shininess;

    if (ambientFile != "")
    {
        _mAmbientTex = Utils::LoadTexture(ambientFile);
    }
	else
	{
		_mAmbientTex = 0;
	}

    if (diffuseFile != "")
    {
        _mDiffuseTex = Utils::LoadTexture(diffuseFile);
    }
	else
	{
		_mDiffuseTex = 0;
	}

    if (specularFile != "")
    {
        _mSpecularTex = Utils::LoadTexture(specularFile);
    }
	else
	{
		_mSpecularTex = 0;
	}

    if (normalFile != "")
    {
		_mNormalTex = Utils::LoadTexture(normalFile);
    }
	else
	{
		_mNormalTex = 0;
	}
}

Material::~Material() {
    if (_mAmbientTex != 0) {
        glDeleteTextures(1, &_mAmbientTex);
        _mAmbientTex = 0;
    }
    if (_mDiffuseTex != 0) {
        glDeleteTextures(1, &_mDiffuseTex);
        _mDiffuseTex = 0;
    }
    if (_mSpecularTex != 0) {
        glDeleteTextures(1, &_mSpecularTex);
        _mSpecularTex = 0;
    }
    if (_mNormalTex != 0) {
        glDeleteTextures(1, &_mNormalTex);
        _mNormalTex = 0;
    }
}

void Material::Bind()
{
    if (_mAmbientTex != 0)
    {
        glActiveTexture(GL_TEXTURE0 + TexID::AMBIENT);
        glBindTexture(GL_TEXTURE_2D, _mAmbientTex);
    }

    if (_mDiffuseTex != 0)
    {
        glActiveTexture(GL_TEXTURE0 + TexID::DIFFUSE);
        glBindTexture(GL_TEXTURE_2D, _mDiffuseTex);
    }

    if (_mSpecularTex != 0)
    {
        glActiveTexture(GL_TEXTURE0 + TexID::SPECULAR);
        glBindTexture(GL_TEXTURE_2D, _mSpecularTex);
    }

    if (_mNormalTex != 0)
    {
        glActiveTexture(GL_TEXTURE0 + TexID::NORMAL);
        glBindTexture(GL_TEXTURE_2D, _mNormalTex);
    }
}
