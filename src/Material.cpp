#include "Material.h"
#include "Utils.h"

Material::Material(float       ambient[3],
                   float       diffuse[3],
                   float       specular[3],
                   float       dissolve,
                   float       shininess,
                   std::string ambientFile,
                   std::string diffuseFile,
                   std::string specularFile,
                   std::string bumpFile)
{
    _mAmbient  = {ambient[0], ambient[1], ambient[2]};
    _mDiffuse  = {diffuse[0], diffuse[1], diffuse[2]};
    _mSpecular = {specular[0], specular[1], specular[2]};

    _mDissolve  = dissolve;
    _mShininess = shininess;

    if (ambientFile != "")
    {
        _mAmbientTex = Utils::LoadTexture(ambientFile);
    }

    if (diffuseFile != "")
    {
        _mDiffuseTex = Utils::LoadTexture(diffuseFile);
    }

    if (specularFile != "")
    {
        _mSpecularTex = Utils::LoadTexture(specularFile);
    }

    if (bumpFile != "")
    {
        _mBumpTex = Utils::LoadTexture(bumpFile);
    }
}

void Material::Bind()
{
    if (_mAmbientTex != 0)
    {
        glActiveTexture(GL_TEXTURE0 + TextureID::AMBIENT);
        glBindTexture(GL_TEXTURE_2D, _mAmbientTex);
    }

    if (_mDiffuseTex != 0)
    {
        glActiveTexture(GL_TEXTURE0 + TextureID::DIFFUSE);
        glBindTexture(GL_TEXTURE_2D, _mDiffuseTex);
    }

    if (_mSpecularTex != 0)
    {
        glActiveTexture(GL_TEXTURE0 + TextureID::SPECULAR);
        glBindTexture(GL_TEXTURE_2D, _mSpecularTex);
    }

    if (_mBumpTex != 0)
    {
        glActiveTexture(GL_TEXTURE0 + TextureID::BUMP);
        glBindTexture(GL_TEXTURE_2D, _mBumpTex);
    }
}
