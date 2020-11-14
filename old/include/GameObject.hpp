#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include <Axis.hpp>
#include <Config.hpp>
#include <Math.hpp>
#include <OpenGL.hpp>
#include <Component.hpp>
#include <Shader.hpp>
#include <Material.hpp>
#include <Component.hpp>
#include <Macros.hpp>

#include <vector>
#include <memory>
#include <typeindex>
#include <typeinfo>

//
class GameObject
{
public:

    DISALLOW_COPY_AND_ASSIGN(GameObject)

    GameObject();
    GameObject(glm::vec3 position);
    virtual ~GameObject();

    virtual void Update(const float dt);
    virtual void Render();

    void SetParent(GameObject* parent) { 
        _mParent = parent;
    }

    GameObject* GetParent() const { 
        return _mParent;
    }

    GameObject* FindGameObject(std::string name);

    void AddChild(std::unique_ptr<GameObject>&& child);

    void RenderAxis();

    void SetName(std::string name) { _mName = name; }
    std::string GetName() { return _mName; }

    //bool Load(std::string filename);

    // Local Transform
    void SetTransform(glm::vec3 position, glm::quat rotation, glm::vec3 scale);

    glm::mat4 GetTransform() const;
    glm::vec3 GetPosition() const { return _mPosition; }
    glm::quat GetRotation() const { return _mRotation; }
    glm::vec3 GetScale() const { return _mScale; }

    // Remember matrix order is Translate (Position), Rotate, Scale
    void SetPosition(glm::vec3 position) {
        _mPosition = position;
    }

    // Remember matrix order is Translate (Position), Rotate, Scale
    void SetRotation(glm::quat rotation) {
        _mRotation = rotation;
    }

    // Remember matrix order is Translate (Position), Rotate, Scale
    void SetScale(glm::vec3 scale) {
        _mScale = scale;
    }

    // World Transforms
    glm::mat4 GetWorldTransform() const;
    glm::vec3 GetWorldPosition() const;
    glm::quat GetWorldRotation() const;
    glm::vec3 GetWorldScale() const;

    // Component
    template <typename T>
    T* AddComponent(std::unique_ptr<Component>&& component);
    
    template <typename T>
    std::vector<T*> FindComponentsOfType();

    // Enabled/disabled
    void SetEnabled(bool value) { _mEnabled = value; }
    bool GetEnabled() { return _mEnabled; }

    // Visibility
    void SetVisibility(bool value) { _mVisibility = value; }
    bool GetVisibility() { return _mVisibility; }


protected:

    // Children
    std::vector<std::unique_ptr<GameObject>> _mChildren;
    
private:
    // Object name
    std::string _mName;

    // Parent
    GameObject* _mParent = nullptr;

    // Axis of the object
    Axis* _mSceneAxis = nullptr;

    // Pos, rot, scale
    glm::vec3 _mPosition = glm::vec3(0.0f),
              _mScale = glm::vec3(1.0f);
    glm::quat _mRotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

    // Components
    std::vector<std::unique_ptr<Component>> _mComponents;

    std::unordered_map<std::type_index, std::vector<Component*>> _mComponentsByType;

    // Enabled
    bool _mEnabled = true;

    // Visibility
    bool _mVisibility = true;

};

template <typename T>
T* GameObject::AddComponent(std::unique_ptr<Component>&& component)
{
    auto id = std::type_index(typeid(T));

    Component* ptr = component.get();

    _mComponents.push_back(std::move(component));
    _mComponents.back()->SetGameObject(this);

    if (_mComponentsByType.find(id) == _mComponentsByType.end())
    {
        _mComponentsByType[id] = std::vector<Component *>();
    }

    _mComponentsByType[id].push_back(ptr);

    return (T*)ptr;
}

template <typename T>
std::vector<T*> GameObject::FindComponentsOfType()
{
    auto id = std::type_index(typeid(T));
    auto list = std::vector<T*>();
    if (_mComponentsByType.find(id) != _mComponentsByType.end())
    {
        for (auto c : _mComponentsByType[id])
        {
            list.push_back(static_cast<T*>(c));
        }
    }

    return list;
}

#endif // GAMEOBJECT_HPP
