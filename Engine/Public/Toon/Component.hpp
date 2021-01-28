#ifndef TOON_COMPONENT_HPP
#define TOON_COMPONENT_HPP

#include <Toon/Config.hpp>
#include <Toon/UpdateContext.hpp>
#include <Toon/RenderContext.hpp>

namespace Toon {

class Entity;

class TOON_ENGINE_API Component
{
public:
    DISALLOW_COPY_AND_ASSIGN(Component)

    Component() = default;

    virtual ~Component() = default;

    virtual inline void Attach(Entity* entity) {
        _mEntity = entity;
    }

    virtual inline void Detach() {
        _mEntity = nullptr;
    }

    virtual inline bool IsAttached() const {
        return (_mEntity != nullptr);
    }

    virtual Entity * GetEntity() const {
        return _mEntity;
    }

    virtual inline void Update(UpdateContext * ctx) { }

    virtual inline void Render(RenderContext * ctx) { }

private:

    Entity* _mEntity = nullptr;

}; // class Component

} // namespacee Toon

#endif // TOON_COMPONENT_HPP