#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include <Config.hpp>
#include <Math.hpp>
#include <OpenGL.hpp>

#include <vector>
#include <memory>

class Shader;
class Material;
class Model;
class Texture;
namespace tinygltf { class Node; }
namespace tinygltf { class Model; }

//
class GameObject
{
public:

    GameObject();
    GameObject(glm::vec3 position);
    virtual ~GameObject();

    virtual void Update(const float dt);
    void Render();

    void SetShader(Shader* shader);

    void SetParent(GameObject* parent) { _mParent = parent; }
    GameObject* GetParent() const { return _mParent; }
    void AddChild(GameObject* child)
    { 
        child->SetParent(this);
        _mChildren.push_back(child);
    }
    std::vector<GameObject*> GetChildren() { return _mChildren; }

    //// LOCAL
    // Local Transform
    void SetTransform(glm::vec3 position, glm::quat rotation, glm::vec3 scale)
    {
        _mPosition = position;
        _mRotation = rotation;
        _mScale = scale;
    }

    glm::mat4 GetTransform() const
    { 
        glm::mat4 transform = glm::mat4(1);
        transform = glm::translate(transform, _mPosition);
        transform *= glm::mat4_cast(_mRotation);
        transform = glm::scale(transform, _mScale);
        return transform;
    }

    // Remember matrix order is Translate (Position), Rotate, Scale
    void SetPosition(glm::vec3 position) {
        _mPosition = position;
    }

    glm::vec3 GetPosition() const { return _mPosition; }

    // Remember matrix order is Translate (Position), Rotate, Scale
    void SetRotation(glm::quat rotation) {
        _mRotation = rotation;
    }

    glm::quat GetRotation() const { return _mRotation; }

    // Remember matrix order is Translate (Position), Rotate, Scale
    void SetScale(glm::vec3 scale) {
        _mScale = scale;
    }

    glm::vec3 GetScale() const { return _mScale; }
    //// END LOCAL

    //// WORLD
    // World Transform
    glm::mat4 GetWorldTransform() const
    {
        if (GetParent())
        {
            return GetParent()->GetTransform() * GetTransform();
        }

        return GetTransform();
    }

    glm::vec3 GetWorldPosition() const
    {
        if (GetParent())
        {
            return GetParent()->GetPosition() + GetPosition();
        }

        return GetPosition();
    }

    glm::quat GetWorldRotation() const
    {
        if (GetParent())
        {
            return GetParent()->GetRotation() * GetRotation();
        }

        return GetRotation();
    }

    glm::vec3 GetWorldScale() const
    {
        if (GetParent())
        {
            return GetParent()->GetScale() * GetScale();
        }

        return GetScale();
    }
    //// END WORLD

    void SetName(std::string name) { _mName = name; }
    std::string GetName() { return _mName; }

    void SetModel(Model* model) { _mModel = model; }

    bool Load(std::string filename);
    
private:
    // Pos, rot, scale
    glm::vec3 _mPosition = glm::vec3(0.0f),
              _mScale = glm::vec3(1.0f);
    glm::quat _mRotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

    // Gobjs Shader
    Shader* _mShader;

    // Model
    Model* _mModel = nullptr;

    // Parent
    GameObject* _mParent = nullptr;

    // Children
    std::vector<GameObject*> _mChildren;

    // Object name
    std::string _mName;

    // Loading
    tinygltf::Model* _mLoadedModel;
    std::vector<std::shared_ptr<Texture>> _mTextures;
    std::vector<std::shared_ptr<Material>> _mMaterials;
    std::vector<GLuint> _mVBOS;

    // Load Textures
    bool processTextures();

    // Load Materials
    bool processMaterials();

    // Part of loading function
    std::unique_ptr<GameObject> processNode(tinygltf::Node& node);
};
#endif // GAMEOBJECT_HPP
