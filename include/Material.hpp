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
    };

    Material() = default;
    virtual ~Material() = default;

    void Bind(Shader* shader);

    void SetDiffuse(glm::vec3 value) { _mDiffuse = value; }
    void SetSpecular(glm::vec3 value) { _mSpecular = value; }
    void SetAmbient(glm::vec3 value) { _mAmbient = value; }
    void SetEmissive(glm::vec3 value) { _mEmissive = value; }
    void SetTransparent(glm::vec3 value) { _mTransparent = value; }
    void SetReflective(glm::vec3 value) { _mReflective = value; }

    void SetMap(Material::TextureID id, std::unique_ptr<Texture> texture) {
        _mTextures.emplace(id, std::move(texture));
    }

private:
    glm::vec3   _mDiffuse = glm::vec3(1.0f, 1.0f, 1.0f),
                _mSpecular = glm::vec3(0),
                _mAmbient = glm::vec3(0),
                _mEmissive = glm::vec3(0),
                _mTransparent = glm::vec3(0),
                _mReflective = glm::vec3(0);

    std::unordered_map<Material::TextureID, std::unique_ptr<Texture>> _mTextures;

    static std::string GetMapVariableName(Material::TextureID id);
    static std::string GetHasMapVariableName(Material::TextureID id);
};

#endif // MATERIAL_H
