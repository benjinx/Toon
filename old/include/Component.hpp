#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include <Macros.hpp>

class GameObject;

class Component 
{
public:

    DISALLOW_COPY_AND_ASSIGN(Component)

    Component() = default;
    virtual ~Component() = default;

    virtual void Update(const float dt) {}
    virtual void Render() {}

    GameObject* GetGameObject() { return _mGameObject; }
    void SetGameObject(GameObject* value) { _mGameObject = value; }

private:
    GameObject* _mGameObject;
};

#endif // COMPONENT_HPP