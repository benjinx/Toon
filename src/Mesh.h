#ifndef MESH_H
#define MESH_H

#include "Common.h"

class Shader;
class Material;

enum attributeName
{
    // defaults
    ATTRIB_POSITION,
    ATTRIB_BLEND_WEIGHTS,
    ATTRIB_NORMAL,
    ATTRIB_COLOR,
    ATTRIB_COLOR_SECONDARY,
    ATTRIB_FOG_COORD,
    ATTRIB_POINT_SIZE,
    ATTRIB_BLEND_INDICES,
    ATTRIB_TEXCOORD0,
    ATTRIB_TEXCOORD1,
    ATTRIB_TEXCOORD2,
    ATTRIB_TEXCOORD3,
    ATTRIB_TEXCOORD4,
    ATTRIB_TEXCOORD5,
    ATTRIB_TEXCOORD6,
    ATTRIB_TEXCOORD7,

    // alternatives
    ATTRIB_TEXCOORD = ATTRIB_TEXCOORD0,
    ATTRIB_TEXCOORD_SECONDARY,
    ATTRIB_TANGENT,
    ATTRIB_BITANGENT,
    ATTRIB_POSITION_SECONDARY,
    ATTRIB_NORMAL_SECONDARY,
    ATTRIB_TANGENT_SECONDARY,
    ATTRIB_BITANGENT_SECONDARY,
};

class Mesh
{
public:
    /* Mesh Data */

    /* Functions */
    Mesh(std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<glm::vec2> texCoords);
    void Render(GLuint programNum, Shader * shader);
    glm::mat4 CalcTBN(std::vector<glm::vec3> vertices, std::vector<glm::vec2> texCoords);

    GLuint GetVAO() { return _mVAO; }
    void SetMaterial(Material* material) { _mMaterial = material; }

    // TEMP
    float* GetRotSpeed() { return &_mRotSpeed; }
    void SetSel(float value) { _mSel = value; }

private:
    Material* _mMaterial;

    /* Render Data */
    GLuint       _mVAO;
    glm::mat4    _mModelMat;
    unsigned int _mVertCount;

    // TEMP
    float _mRotSpeed;
    float _mSel;
};

#endif // MESH_H
