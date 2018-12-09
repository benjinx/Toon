#ifndef MATERIAL_H
#define MATERIAL_H

#include "Config.hpp"

class Material
{
public:

    enum TexID {
        AMBIENT = 0,
        DIFFUSE,
        SPECULAR,
        NORMAL,
    };

    Material(float       ambient[3],
             float       diffuse[3],
             float       specular[3],
             float       shininess,
             std::string ambientFile,
             std::string diffuseFile,
             std::string specularFile,
             std::string normalFile);
    virtual ~Material();

    void Bind();

	glm::vec3 GetAmbient() { return _mAmbient; }
	glm::vec3 GetDiffuse() { return _mDiffuse; }
	glm::vec3 GetSpecular() { return _mSpecular; }
	float GetShininess() { return _mShininess; }

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

    bool NormalTexExists()
    {
        if (_mNormalTex != 0)
            return true;
        else
            return false;
    }

	GLuint GetAmbientTex() { return _mAmbientTex; }
	GLuint GetDiffuseTex() { return _mDiffuseTex; }
	GLuint GetSpecularTex() { return _mSpecularTex; }
	GLuint GetNormalTex() { return _mNormalTex; }

private:
    glm::vec3 _mAmbient, _mDiffuse, _mSpecular;

    float _mShininess;

    GLuint _mAmbientTex, _mDiffuseTex, _mSpecularTex, _mNormalTex;
};

#endif // MATERIAL_H
