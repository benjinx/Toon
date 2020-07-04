#ifndef MATERIAL_H
#define MATERIAL_H

#include <Config.hpp>
#include <Math.hpp>
#include <Texture.hpp>

class Shader;

#include <memory>
#include <unordered_map>

class Material
{
public:

    enum TextureID {
        DIFFUSE = 0,
        SPECULAR = 1,
        AMBIENT = 2,
        EMISSIVE = 3,
        HEIGHT = 4,
        NORMAL = 5,
        SHININESS = 6,
        OPACITY = 7,
        DISPLACEMENT = 8,
        LIGHT_MAP = 9,
        REFLECTION = 10,
        BASE_COLOR = 11,
        NORMAL_CAMERA = 12,
        EMISSION_COLOR = 13,
        METALNESS = 14,
        DIFFUSE_ROUGHNESS = 15,
        AMBIENT_OCCLUSION = 16,
        METALLIC_ROUGHNESS = 17,
    };

    Material() = default;
    virtual ~Material() = default;

    void Bind(Shader* shader);

    void SetDiffuse(glm::vec3 value)        { _mDiffuse = value; }
    void SetSpecular(glm::vec3 value)       { _mSpecular = value; }
    void SetAmbient(glm::vec3 value)        { _mAmbient = value; }
    void SetEmissive(glm::vec3 value)       { _mEmissive = value; }
    void SetTransparent(glm::vec3 value)    { _mTransparent = value; }
    void SetReflective(glm::vec3 value)     { _mReflective = value; }

    glm::vec3 GetDiffuse()                  { return _mDiffuse; }
    glm::vec3 GetSpecular()                 { return _mSpecular; }
    glm::vec3 GetAmbient()                  { return _mAmbient; }
    glm::vec3 GetEmissive()                 { return _mEmissive; }
    glm::vec3 GetTransparent()              { return _mTransparent; }
    glm::vec3 GetReflective()               { return _mReflective; }

    void SetMetallicFactor(float value)     { _mMetallicFactor = value; }
    void SetRoughnessFactor(float value)    { _mRoughnessFactor = value; }
    void SetOcclusionStrength(float value)  { _mOcclusionStrength = value; }
    void SetNormalScale(float value)        { _mNormalScale = value; }

    void SetBaseColorFactor(glm::vec4 value) { _mBaseColorFactor = value; }
    void SetEmissiveFactor(glm::vec3 value) { _mEmissiveFactor = value; }

    glm::vec4 GetBaseColorFactor() { return _mBaseColorFactor; }
    glm::vec3 GetEmissiveFactor() { return _mEmissiveFactor; }

    void SetMap(Material::TextureID id, std::shared_ptr<Texture> texture) {
        _mTextures.emplace(id, std::move(texture));
    }

private:
    glm::vec3   _mDiffuse = glm::vec3(1.0f, 1.0f, 1.0f),
                _mSpecular = glm::vec3(0),
                _mAmbient = glm::vec3(0),
                _mEmissive = glm::vec3(0),
                _mTransparent = glm::vec3(0),
                _mReflective = glm::vec3(0);

    glm::vec4 _mBaseColorFactor = glm::vec4(1.0f);
    glm::vec3 _mEmissiveFactor = glm::vec3(1.0f);

    float       _mMetallicFactor = 1.0f,
                _mRoughnessFactor = 1.0f,
                _mOcclusionStrength = 1.0f,
                _mNormalScale = 1.0f;

    std::unordered_map<Material::TextureID, std::shared_ptr<Texture>> _mTextures;

    static std::string GetMapVariableName(Material::TextureID id);
    static std::string GetHasMapVariableName(Material::TextureID id);
};

#endif // MATERIAL_H
