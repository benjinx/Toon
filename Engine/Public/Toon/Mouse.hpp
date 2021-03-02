#ifndef TOON_MOUSE_HPP
#define TOON_MOUSE_HPP

#include <Toon/Config.hpp>

#include <Toon/String.hpp>

namespace Toon {

enum class MouseButton
{
    Unknown = -1,

    Left,
    Right,
    Middle,

}; // enum MouseButton

static inline string GetMouseButtonName(MouseButton button)
{
    switch(button) {
    case MouseButton::Left:
        return "Left";
    case MouseButton::Right:
        return "Right";
    case MouseButton::Middle:
        return "Middle";
    default: ;
    }

    return string();
};

} // namespace Toon

#endif // TOON_MOUSE_HPP
