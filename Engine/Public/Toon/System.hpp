#ifndef TOON_SYSTEM_HPP
#define TOON_SYSTEM_HPP

#include <Toon/Config.hpp>
#include <Toon/UpdateContext.hpp>
#include <Toon/RenderContext.hpp>

namespace Toon {

class TOON_ENGINE_API System
{
public:

    DISALLOW_COPY_AND_ASSIGN(System)

    System() = default;

    virtual ~System() = default;

    virtual inline void Update(UpdateContext * ctx) { }

    virtual inline void Render(RenderContext * ctx) { }

}; // class System

} // namespace Toon

#endif // TOON_SYSTEM_HPP