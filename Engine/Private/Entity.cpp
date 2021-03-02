#include <Toon/Entity.hpp>

#include <Toon/SceneImporter.hpp>

namespace Toon {

TOON_ENGINE_API
bool Entity::LoadFromFile(const string& filename)
{
    const auto& importers = GetAllSceneImporters();
    for (const auto& importer : importers) {
        if (importer->LoadFromFile(this, filename)) {
            return true;
        }
    }
    return false;
}

TOON_ENGINE_API
void Entity::SetParent(Entity * parent)
{
    _parent = parent;
}

TOON_ENGINE_API
Entity * Entity::AddChild(std::unique_ptr<Entity> && child)
{
    child->SetParent(this);
    _childPtrs.push_back(child.get());
    _children.push_back(std::move(child));

    return _childPtrs.back();
}

TOON_ENGINE_API
std::vector<Entity *> Entity::GetChildren() const
{
    return _childPtrs;
}

TOON_ENGINE_API
Component * Entity::AddComponent(std::unique_ptr<Component> && component)
{
    component->Attach(this);
    _componentPtrs.push_back(component.get());
    _components.push_back(std::move(component));

    return _componentPtrs.back();
}

TOON_ENGINE_API
std::vector<Component *> Entity::GetComponents() const
{
    return _componentPtrs;
}

TOON_ENGINE_API
void Entity::SetName(const string& name)
{
    _name = name;
}

TOON_ENGINE_API
void Entity::SetPosition(const glm::vec3& position)
{
    _position = position;
}

TOON_ENGINE_API
void Entity::SetOrientation(const glm::quat& orientation)
{
    _orientation = orientation;
}

TOON_ENGINE_API
void Entity::SetScale(const glm::vec3& scale)
{
    _scale = scale;
}

TOON_ENGINE_API
glm::mat4 Entity::GetTransform() const
{
    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, GetPosition());
    transform *= glm::mat4_cast(GetOrientation());
    transform = glm::scale(transform, GetScale());
    return transform;
}

TOON_ENGINE_API
glm::vec3 Entity::GetWorldPosition() const
{
    if (_parent) {
        return _parent->GetWorldPosition() + GetPosition();
    }
    return GetPosition();
}

TOON_ENGINE_API
glm::quat Entity::GetWorldOrientation() const
{
    if (_parent) {
        return _parent->GetWorldOrientation() * GetOrientation();
    }
    return GetOrientation();
}

TOON_ENGINE_API
glm::vec3 Entity::GetWorldScale() const
{
    if (_parent) {
        return _parent->GetWorldScale() * GetScale();
    }
    return GetScale();
}

TOON_ENGINE_API
glm::mat4 Entity::GetWorldTransform() const
{
    if (_parent) {
        return _parent->GetWorldTransform() * GetTransform();
    }
    return GetTransform();
}

TOON_ENGINE_API
void Entity::Update(UpdateContext * ctx)
{
    // Check if enabled
    if (!_mEnabled) {
        return;
    }

    for (auto& comp : _components) {
        comp->Update(ctx);
    }

    for (auto& child : _children) {
        child->Update(ctx);
    }
}

TOON_ENGINE_API
void Entity::Render(RenderContext * ctx)
{
    // Check for visibility
    if (!_mVisibility) {
        return;
    }

    for (auto& comp : _components) {
        comp->Render(ctx);
    }
    
    for (auto& child : _children) {
        child->Render(ctx);
    }
}

TOON_ENGINE_API
Entity* Entity::FindEntity(std::string name)
{
    for (auto& entity : _children)
    {
        if (entity->GetName() == name)
        {
            return entity.get();
        }
    }

    for (auto& entity : _children)
    {
        auto tmp = entity->FindEntity(name);
        if (tmp)
        {
            return tmp;
        }
    }

    return nullptr;
}

} // namespace Toon