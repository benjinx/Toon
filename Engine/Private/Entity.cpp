#include <Temporality/Scene/Entity.hpp>

//#include <Temporality/App.hpp>
#include <Temporality/Camera.hpp>
#include <Temporality/Light.hpp>
#include <Temporality/Log.hpp>
#include <Temporality/Graphics/Mesh.hpp>
//#include <Temporality/StaticMeshComponent.hpp>
#include <Temporality/Utils.hpp>

namespace Temporality {

TEMPORALITY_ENGINE_API
Entity::Entity()
{
    SetTransform(glm::vec3(0), glm::vec3(0), glm::vec3(1));
}

TEMPORALITY_ENGINE_API
Entity::Entity(glm::vec3 position)
{
    SetPosition(position);
}

TEMPORALITY_ENGINE_API
Entity::~Entity()
{
    delete _mSceneAxis;
    _mChildren.clear();
}

TEMPORALITY_ENGINE_API
void Entity::Update(UpdateContext * ctx)
{
    // Check if enabled
    if (!_mEnabled)
    {
        return;
    }

    // Update
    for (const auto& comp : _mComponents)
    {
        comp->Update(ctx);
    }

    // Update all children
    for (const auto& child : _mChildren)
    {
        child->Update(ctx);
    }
}

TEMPORALITY_ENGINE_API
void Entity::Render(RenderContext * ctx)
{
    // Check for visibility
    if (!_mVisibility)
    {
        return;
    }

    // Render
    for (const auto& comp : _mComponents)
    {
        comp->Render(ctx);
    }

    // Call render on all children
    for (const auto& child : _mChildren)
    {
        child->Render(ctx);
    }
}

TEMPORALITY_ENGINE_API
void Entity::RenderAxis()
{
    if (_mSceneAxis == nullptr)
    {
        _mSceneAxis = new Axis();
    }

    // Render the entitys axis
    //_mSceneAxis->Render(GetWorldTransform());

    // Render it for our children
    for (auto& entity : _mChildren)
    {
        entity->RenderAxis();
    }
}

TEMPORALITY_ENGINE_API
Entity* Entity::FindEntity(std::string name)
{
    for (auto& entity : _mChildren)
    {
        if (entity->GetName() == name)
        {
            return entity.get();
        }
    }

    for (auto& entity : _mChildren)
    {
        auto tmp = entity->FindEntity(name);
        if (tmp)
        {
            return tmp;
        }
    }

    return nullptr;
}

TEMPORALITY_ENGINE_API
void Entity::AddChild(std::unique_ptr<Entity>&& child)
{
    child->SetParent(this);
    _mChildren.push_back(std::move(child));
}

TEMPORALITY_ENGINE_API
void Entity::SetTransform(glm::vec3 position, glm::quat rotation, glm::vec3 scale)
{
    _mPosition = position;
    _mRotation = rotation;
    _mScale = scale;
}

TEMPORALITY_ENGINE_API
glm::mat4 Entity::GetTransform() const {
    glm::mat4 transform = glm::mat4(1);
    transform = glm::translate(transform, _mPosition);
    transform *= glm::mat4_cast(_mRotation);
    transform = glm::scale(transform, _mScale);
    return transform;
}

TEMPORALITY_ENGINE_API
glm::mat4 Entity::GetWorldTransform() const
{
    if (GetParent())
    {
        return GetParent()->GetTransform() * GetTransform();
    }

    return GetTransform();
}

TEMPORALITY_ENGINE_API
glm::vec3 Entity::GetWorldPosition() const
{
    if (GetParent())
    {
        return GetParent()->GetPosition() + GetPosition();
    }

    return GetPosition();
}

TEMPORALITY_ENGINE_API
glm::quat Entity::GetWorldRotation() const
{
    if (GetParent())
    {
        return GetParent()->GetRotation() * GetRotation();
    }

    return GetRotation();
}

TEMPORALITY_ENGINE_API
glm::vec3 Entity::GetWorldScale() const
{
    if (GetParent())
    {
        return GetParent()->GetScale() * GetScale();
    }

    return GetScale();
}

} // namespace Temporality