#include "GameObject.hpp"

#include <App.hpp>
#include <Camera.hpp>
#include <Light.hpp>
#include <Log.hpp>
#include <Mesh.hpp>
#include <MeshComponent.hpp>
#include <Utils.hpp>

GameObject::GameObject()
{
}

GameObject::GameObject(glm::vec3 position)
{
    SetPosition(position);
}

GameObject::~GameObject()
{
    delete _mSceneAxis;
    _mChildren.clear();
}

void GameObject::Update(const float dt)
{
    // Check if enabled
    if (!_mEnabled)
    {
        return;
    }

    // Update
    for (const auto& comp : _mComponents)
    {
        comp->Update(dt);
    }

    // Update all children
    for (const auto& child : _mChildren)
    {
        child->Update(dt);
    }
}

void GameObject::Render()
{
    // Check for visibility
    if (!_mVisibility)
    {
        return;
    }

    // Render
    for (const auto& comp : _mComponents)
    {
        comp->Render();
    }

    // Call render on all children
    for (const auto& child : _mChildren)
    {
        child->Render();
    }
}

void GameObject::RenderAxis()
{
    if (_mSceneAxis == nullptr)
    {
        _mSceneAxis = new Axis();
    }

    // Render the gobjs axis
    _mSceneAxis->Render(GetWorldTransform());

    // Render it for our children
    for (auto& gobj : _mChildren)
    {
        gobj->RenderAxis();
    }
}

GameObject* GameObject::FindGameObject(std::string name)
{
    for (auto& gobj : _mChildren)
    {
        if (gobj->GetName() == name)
        {
            return gobj.get();
        }
    }

    for (auto& gobj : _mChildren)
    {
        auto tmp = gobj->FindGameObject(name);
        if (tmp)
        {
            return tmp;
        }
    }

    return nullptr;
}

void GameObject::AddChild(std::unique_ptr<GameObject>&& child)
{
    child->SetParent(this);
    _mChildren.push_back(std::move(child));
}

void GameObject::SetTransform(glm::vec3 position, glm::quat rotation, glm::vec3 scale)
{
    _mPosition = position;
    _mRotation = rotation;
    _mScale = scale;
}

glm::mat4 GameObject::GetTransform() const {
    glm::mat4 transform = glm::mat4(1);
    transform = glm::translate(transform, _mPosition);
    transform *= glm::mat4_cast(_mRotation);
    transform = glm::scale(transform, _mScale);
    return transform;
}

glm::mat4 GameObject::GetWorldTransform() const
{
    if (GetParent())
    {
        return GetParent()->GetTransform() * GetTransform();
    }

    return GetTransform();
}

glm::vec3 GameObject::GetWorldPosition() const
{
    if (GetParent())
    {
        return GetParent()->GetPosition() + GetPosition();
    }

    return GetPosition();
}

glm::quat GameObject::GetWorldRotation() const
{
    if (GetParent())
    {
        return GetParent()->GetRotation() * GetRotation();
    }

    return GetRotation();
}

glm::vec3 GameObject::GetWorldScale() const
{
    if (GetParent())
    {
        return GetParent()->GetScale() * GetScale();
    }

    return GetScale();
}
