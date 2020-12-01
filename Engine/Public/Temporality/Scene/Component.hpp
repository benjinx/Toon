#ifndef TEMPORALITY_COMPONENT_HPP
#define TEMPORALITY_COMPONENT_HPP

#include <Temporality/Config.hpp>
#include <Temporality/UpdateContext.hpp>
#include <Temporality/RenderContext.hpp>

namespace Temporality {

class Entity;

class TEMPORALITY_ENGINE_API Component
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

} // namespacee Temporality

#endif // TEMPORALITY_COMPONENT_HPP