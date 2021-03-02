#ifndef TOON_ENTITY_HPP
#define TOON_ENTITY_HPP

#include <Toon/Config.hpp>
#include <Toon/Math.hpp>
#include <Toon/Component.hpp>
#include <Toon/UpdateContext.hpp>
#include <Toon/RenderContext.hpp>
#include <Toon/Macros.hpp>
#include <Toon/String.hpp>

#include <vector>
#include <memory>

namespace Toon {

TOON_ENGINE_API
constexpr glm::vec3 GetWorldUp() {
    return { 0.0f, 1.0f, 0.0f };
}

TOON_ENGINE_API
constexpr glm::vec3 GetWorldForward() {
    return { 0.0f, 0.0f, -1.0f };
}

class TOON_ENGINE_API Entity
{
public:

    DISALLOW_COPY_AND_ASSIGN(Entity)

    Entity() = default;
    
    virtual ~Entity() = default;

    bool LoadFromFile(const string& filename);

    void SetParent(Entity* parent);

    inline Entity* GetParent() const { 
        return _parent;
    }

    Entity * AddChild(std::unique_ptr<Entity>&& child);

    Entity* FindEntity(string name);

    std::vector<Entity *> GetChildren() const;

    Component * AddComponent(std::unique_ptr<Component>&& component);

    std::vector<Component *> GetComponents() const;

    void SetName(const string& name);

    inline string GetName() const {
        return _name;
    }

    // Remember matrix order is Translate (Position), Orientate, Scale
    void SetPosition(const glm::vec3& position);

    inline glm::vec3 GetPosition() const {
        return _position;
    }

    // Remember matrix order is Translate (Position), Orientate, Scale
    void SetOrientation(const glm::quat& orientation);

    inline glm::quat GetOrientation() const {
        return _orientation;
    }

    // Remember matrix order is Translate (Position), Orientate, Scale
    void SetScale(const glm::vec3& scale);

    inline glm::vec3 GetScale() const {
        return _scale;
    }

    glm::mat4 GetTransform() const;

    glm::vec3 GetWorldPosition() const;

    glm::quat GetWorldOrientation() const;

    glm::vec3 GetWorldScale() const;

    glm::mat4 GetWorldTransform() const;
    
    virtual void Update(UpdateContext * ctx);

    virtual void Render(RenderContext * ctx);

    // Enabled/disabled
    void SetEnabled(bool value) { _mEnabled = value; }
    bool GetEnabled() { return _mEnabled; }

    // Visibility
    void SetVisibility(bool value) { _mVisibility = value; }
    bool GetVisibility() { return _mVisibility; }

    
private:
    Entity* _parent = nullptr;

    string _name;

    std::vector<std::unique_ptr<Entity>> _children;

    std::vector<Entity *> _childPtrs;

    std::vector<std::unique_ptr<Component>> _components;
    
    std::vector<Component *> _componentPtrs;

    // Pos, rot, scale
    glm::vec3 _position = glm::vec3(0.0f);

    glm::quat _orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

    glm::vec3 _scale = glm::vec3(1.0f);

    // Enabled
    bool _mEnabled = true;

    // Visibility
    bool _mVisibility = true;

};

} // namespace Toon

#endif // TOON_ENTITY_HPP