#ifndef MATERIAL_H
#define MATERIAL_H

#include "Config.hpp"

#include <memory>

class Shader;
class Texture;

class Material
{
public:

    enum TextureID {
        AMBIENT = 0,
        DIFFUSE = 1,
        SPECULAR = 2,
        NORMAL = 3,
        ALPHA = 4,
        DISPLACEMENT = 5,
        METALLIC_ROUGHNESS = 6,
        SHEEN = 7,
        EMISSIVE = 8,
    };

    Material(glm::vec3 ambient,
             glm::vec3 diffuse,
             glm::vec3 specular,
             float shininess,
             std::shared_ptr<Texture> ambientMap,
             std::shared_ptr<Texture> diffuseMap,
             std::shared_ptr<Texture> specularMap,
             std::shared_ptr<Texture> normalMap);
    virtual ~Material() = default;

    void Bind(Shader* shader);

    bool AmbientMapExists() { return (_mAmbientMap != nullptr); }
    bool DiffuseMapExists() { return (_mDiffuseMap != nullptr); }
    bool SpecularMapExists() { return (_mSpecularMap != nullptr); }
    bool NormalMapExists() { return (_mNormalMap != nullptr); }
    bool AlphaMapExists() { return (_mAlphaMap != nullptr); }
    bool DisplacementMapExists() { return (_mDisplacementMap != nullptr); }
    bool MetallicRoughnessMapExists() { return (_mMetallicRoughnessMap != nullptr); }
    bool SheenMapExists() { return (_mSheenMap != nullptr); }
    bool EmissiveMapExists() { return (_mEmissiveMap != nullptr); }

private:
    glm::vec3   _mAmbient = glm::vec3(0),
                _mDiffuse = glm::vec3(0),
                _mSpecular = glm::vec3(0),
                _mEmissive = glm::vec3(0);

    float       _mRoughness = 0.0f,
                _mMetallic = 0.0f,
                _mShininess = 1.0f,
                _mDissolve = 1.0f,
                _mSheen = 0.0f,
                _mClearcoatThickness = 0.0f,
                _mClearcoatRoughness = 0.0f,
                _mAnisotropy = 0.0f,
                _mAnisotropyRotation = 0.0f;

    std::shared_ptr<Texture>    _mAmbientMap,
                                _mDiffuseMap,
                                _mSpecularMap,
                                _mNormalMap,
                                _mAlphaMap,
                                _mDisplacementMap,
                                _mMetallicRoughnessMap,
                                _mSheenMap,
                                _mEmissiveMap;
};

#endif // MATERIAL_H
