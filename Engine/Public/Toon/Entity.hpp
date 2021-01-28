#ifndef TOON_ENTITY_HPP
#define TOON_ENTITY_HPP

#include <Toon/Config.hpp>
#include <Toon/Axis.hpp>
#include <Toon/Math.hpp>
//#include <Toon/OpenGL.hpp>
#include <Toon/Component.hpp>
//#include <Toon/Shader.hpp>
//#include <Toon/Material.hpp>
#include <Toon/Macros.hpp>

#include <vector>
#include <memory>
#include <unordered_map>
#include <typeindex>
#include <typeinfo>

namespace Toon {

TOON_ENGINE_API
inline glm::vec3 GetWorldUp() {
    return { 0.0f, 1.0f, 0.0f };
}

TOON_ENGINE_API
inline glm::vec3 GetWorldForward() {
    return { 0.0f, 0.0f, -1.0f };
}

class TOON_ENGINE_API Entity
{
public:

    DISALLOW_COPY_AND_ASSIGN(Entity)

    Entity();
    Entity(glm::vec3 position);
    virtual ~Entity();

    virtual void Update(UpdateContext * ctx);
    virtual void Render(RenderContext * ctx);

    void SetParent(Entity* parent) { 
        _mParent = parent;
    }

    Entity* GetParent() const { 
        return _mParent;
    }

    Entity* FindEntity(std::string name);

    void AddChild(std::unique_ptr<Entity>&& child);

    void RenderAxis();

    void SetName(std::string name) { _mName = name; }
    std::string GetName() { return _mName; }

    //bool Load(std::string filename);

    // Local Transform
    void SetTransform(glm::vec3 position, glm::quat rotation, glm::vec3 scale);

    glm::mat4 GetTransform() const;
    glm::vec3 GetPosition() const { return _position; }
    glm::quat GetOrientation() const { return _orientation; }
    glm::vec3 GetScale() const { return _scale; }

    // Remember matrix order is Translate (Position), Orientate, Scale
    void SetPosition(glm::vec3 position) {
        _position = position;
    }

    // Remember matrix order is Translate (Position), Orientate, Scale
    void SetOrientation(glm::quat orientation) {
        _orientation = orientation;
    }

    // Remember matrix order is Translate (Position), Orientate, Scale
    void SetScale(glm::vec3 scale) {
        _scale = scale;
    }

    // World Transforms
    glm::mat4 GetWorldTransform() const;
    glm::vec3 GetWorldPosition() const;
    glm::quat GetWorldOrientation() const;
    glm::vec3 GetWorldScale() const;

    // Component
    Component * AddComponent(std::unique_ptr<Component>&& component);
    
    template <typename T>
    std::vector<T*> FindComponentsOfType();

    // Enabled/disabled
    void SetEnabled(bool value) { _mEnabled = value; }
    bool GetEnabled() { return _mEnabled; }

    // Visibility
    void SetVisibility(bool value) { _mVisibility = value; }
    bool GetVisibility() { return _mVisibility; }
    
private:

    // Children
    std::vector<std::unique_ptr<Entity>> _mChildren;
    
    // Object name
    std::string _mName;

    // Parent
    Entity* _mParent = nullptr;

    // Axis of the object
    Axis* _mSceneAxis = nullptr;

    // Pos, rot, scale
    glm::vec3 _position = glm::vec3(0.0f),
              _scale = glm::vec3(1.0f);
    glm::quat _orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

    // Components
    std::vector<std::unique_ptr<Component>> _componentList;

    std::vector<Component *> _componentPtrs;

    //std::unordered_map<std::type_index, std::vector<Component*>> _componentListByType;

    // Enabled
    bool _mEnabled = true;

    // Visibility
    bool _mVisibility = true;

};

// template <typename T>
// std::vector<T*> Entity::FindComponentsOfType()
// {
//     auto id = std::type_index(typeid(T));
//     auto list = std::vector<T*>();
//     if (_componentListByType.find(id) != _componentListByType.end())
//     {
//         for (auto c : _componentListByType[id])
//         {
//             list.push_back(static_cast<T*>(c));
//         }
//     }

//     return list;
// }

} // namespace Toon

#endif // TOON_ENTITY_HPP