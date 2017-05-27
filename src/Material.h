#ifndef MATERIAL_H
#define MATERIAL_H

#include "Common.h"

enum TextureID
{
	AMBIENT = 0,
	DIFFUSE,
	SPECULAR,
	BUMP,
};

class Material
{
public:
    Material(float       ambient[3],
             float       diffuse[3],
             float       specular[3],
             float       dissolve,
             float       shininess,
             std::string ambientFile,
             std::string diffuseFile,
             std::string specularFile,
             std::string bumpFile);
    ~Material(){};

    void Bind();

    bool AmbientTexExists()
    {
        if (_mAmbientTex != 0)
            return true;
        else
            return false;
    }

    bool DiffuseTexExists()
    {
        if (_mDiffuseTex != 0)
            return true;
        else
            return false;
    }

    bool SpecularTexExists()
    {
        if (_mSpecularTex != 0)
            return true;
        else
            return false;
    }

    bool BumpTexExists()
    {
        if (_mBumpTex != 0)
            return true;
        else
            return false;
    }

	GLuint GetAmbientTex() { return _mAmbientTex; }
	GLuint GetDiffuseTex() { return _mDiffuseTex; }
	GLuint GetSpecularTex() { return _mSpecularTex; }
	GLuint GetBumpTex() { return _mBumpTex; }

private:
    glm::vec3 _mAmbient, _mDiffuse, _mSpecular;

    float _mDissolve, _mShininess;

    GLuint _mAmbientTex, _mDiffuseTex, _mSpecularTex, _mBumpTex;
};

#endif // MATERIAL_H
