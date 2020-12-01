#ifndef TEMPORALITY_COMPONENT_HPP
#define TEMPORALITY_COMPONENT_HPP

#include <Temporality/Config.hpp>
// #include <Temporality/UpdateContext.hpp>
// #include <Temporality/RenderContext.hpp>

namespace Temporality {

class GameObject;

class TEMPORALITY_ENGINE_API Component
{
public:
    DISALLOW_COPY_AND_ASSIGN(Component)

    Component() = default;

    virtual ~Component() = default;

    virtual inline void Attach(GameObject* gameObject) {
        _mGameObject = gameObject;
    }

    virtual inline void Detach() {
        _mGameObject = nullptr;
    }

    virtual inline bool IsAttached() const {
        return (_mGameObject != nullptr);
    }

    virtual GameObject * GetEntity() const {
        return _mGameObject;
    }

    virtual inline void Update(UpdateContext * ctx) { }

    virtual inline void Render(RenderContext * ctx) { }

private:

    GameObject* _mGameObject = nullptr;

} // class Component

}; // namespacee Temporality

#endif // TEMPORALITY_COMPONENT_HPP