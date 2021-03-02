#ifndef TOON_KEYBOARD_HPP
#define TOON_KEYBOARD_HPP

#include <Toon/Config.hpp>

#include <Toon/String.hpp>

namespace Toon {

enum class KeyboardKey
{
    Unknown = -1,

    A,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
    I,
    J,
    K,
    L,
    M,
    N,
    O,
    P,
    Q,
    R,
    S,
    T,
    U,
    V,
    W,
    X,
    Y,
    Z,
    Num0,
    Num1,
    Num2,
    Num3,
    Num4,
    Num5,
    Num6,
    Num7,
    Num8,
    Num9,
    Numpad0,
    Numpad1,
    Numpad2,
    Numpad3,
    Numpad4,
    Numpad5,
    Numpad6,
    Numpad7,
    Numpad8,
    Numpad9,
    NumpadPlus,
    NumpadMinus,
    NumpadMultiply,
    NumpadDivide,
    NumpadEnter,
    NumpadPeriod,
    F1,
    F2,
    F3,
    F4,
    F5,
    F6,
    F7,
    F8,
    F9,
    F10,
    F11,
    F12,
    F13,
    F14,
    F15,
    F16,
    F17,
    F18,
    F19,
    F20,
    F21,
    F22,
    F23,
    F24,
    Escape,
    Delete,
    Insert,
    Home,
    End,
    PageUp,
    PageDown,
    Backspace,
    Enter,
    Space,
    LeftShift,
    RightShift,
    LeftControl,
    RightControl,
    LeftAlt,
    RightAlt,
    PrintScreen,
    Break,
    Grave,
    Tab,
    CapsLock,
    NumLock,
    ScrollLock,
    LeftSuper,
    RightSuper,
    Menu,
    Left,
    Right,
    Up,
    Down,
    Minus,
    Equals,
    Semicolon,
    Quote,
    Slash,
    Backslash,
    Comma,
    Period,
    LeftBracket,
    RightBracket,
}; // enum KeyboardKey

static inline string GetKeyboardKeyName(KeyboardKey key)
{
    switch (key) {
    case KeyboardKey::A:
        return "A";
    case KeyboardKey::B:
        return "B";
    case KeyboardKey::C:
        return "C";
    case KeyboardKey::D:
        return "D";
    case KeyboardKey::E:
        return "E";
    case KeyboardKey::F:
        return "F";
    case KeyboardKey::G:
        return "G";
    case KeyboardKey::H:
        return "H";
    case KeyboardKey::I:
        return "I";
    case KeyboardKey::J:
        return "J";
    case KeyboardKey::K:
        return "K";
    case KeyboardKey::L:
        return "L";
    case KeyboardKey::M:
        return "M";
    case KeyboardKey::N:
        return "N";
    case KeyboardKey::O:
        return "O";
    case KeyboardKey::P:
        return "P";
    case KeyboardKey::Q:
        return "Q";
    case KeyboardKey::R:
        return "R";
    case KeyboardKey::S:
        return "S";
    case KeyboardKey::T:
        return "T";
    case KeyboardKey::U:
        return "U";
    case KeyboardKey::V:
        return "V";
    case KeyboardKey::W:
        return "W";
    case KeyboardKey::X:
        return "X";
    case KeyboardKey::Y:
        return "Y";
    case KeyboardKey::Z:
        return "Z";
    case KeyboardKey::Num0:
        return "0";
    case KeyboardKey::Num1:
        return "1";
    case KeyboardKey::Num2:
        return "2";
    case KeyboardKey::Num3:
        return "3";
    case KeyboardKey::Num4:
        return "4";
    case KeyboardKey::Num5:
        return "5";
    case KeyboardKey::Num6:
        return "6";
    case KeyboardKey::Num7:
        return "7";
    case KeyboardKey::Num8:
        return "8";
    case KeyboardKey::Num9:
        return "9";
    case KeyboardKey::Numpad0:
        return "Numpad 0";
    case KeyboardKey::Numpad1:
        return "Numpad 1";
    case KeyboardKey::Numpad2:
        return "Numpad 2";
    case KeyboardKey::Numpad3:
        return "Numpad 3";
    case KeyboardKey::Numpad4:
        return "Numpad 4";
    case KeyboardKey::Numpad5:
        return "Numpad 5";
    case KeyboardKey::Numpad6:
        return "Numpad 6";
    case KeyboardKey::Numpad7:
        return "Numpad 7";
    case KeyboardKey::Numpad8:
        return "Numpad 8";
    case KeyboardKey::Numpad9:
        return "Numpad 9";
    case KeyboardKey::NumpadPlus:
        return "Numpad +";
    case KeyboardKey::NumpadMinus:
        return "Numpad -";
    case KeyboardKey::NumpadMultiply:
        return "Numpad *";
    case KeyboardKey::NumpadDivide:
        return "Numpad /";
    case KeyboardKey::NumpadEnter:
        return "Numpad Enter";
    case KeyboardKey::NumpadPeriod:
        return "Numpad .";
    case KeyboardKey::F1:
        return "F1";
    case KeyboardKey::F2:
        return "F2";
    case KeyboardKey::F3:
        return "F3";
    case KeyboardKey::F4:
        return "F4";
    case KeyboardKey::F5:
        return "F5";
    case KeyboardKey::F6:
        return "F6";
    case KeyboardKey::F7:
        return "F7";
    case KeyboardKey::F8:
        return "F8";
    case KeyboardKey::F9:
        return "F9";
    case KeyboardKey::F10:
        return "F10";
    case KeyboardKey::F11:
        return "F11";
    case KeyboardKey::F12:
        return "F12";
    case KeyboardKey::F13:
        return "F13";
    case KeyboardKey::F14:
        return "F14";
    case KeyboardKey::F15:
        return "F15";
    case KeyboardKey::F16:
        return "F16";
    case KeyboardKey::F17:
        return "F17";
    case KeyboardKey::F18:
        return "F18";
    case KeyboardKey::F19:
        return "F19";
    case KeyboardKey::F20:
        return "F20";
    case KeyboardKey::F21:
        return "F21";
    case KeyboardKey::F22:
        return "F22";
    case KeyboardKey::F23:
        return "F23";
    case KeyboardKey::F24:
        return "F24";
    case KeyboardKey::Escape:
        return "Escape";
    case KeyboardKey::Delete:
        return "Delete";
    case KeyboardKey::Insert:
        return "Insert";
    case KeyboardKey::Home:
        return "Home";
    case KeyboardKey::End:
        return "End";
    case KeyboardKey::PageUp:
        return "Page Up";
    case KeyboardKey::PageDown:
        return "Page Down";
    case KeyboardKey::Backspace:
        return "Backspace";
    case KeyboardKey::Enter:
        return "Enter";
    case KeyboardKey::Space:
        return "Space";
    case KeyboardKey::LeftShift:
        return "Left Shift";
    case KeyboardKey::RightShift:
        return "Right Shift";
    case KeyboardKey::LeftControl:
        return "Left Control";
    case KeyboardKey::RightControl:
        return "Right Control";
    case KeyboardKey::LeftAlt:
        return "Left Alt";
    case KeyboardKey::RightAlt:
        return "Right Alt";
    case KeyboardKey::PrintScreen:
        return "Print Screen";
    case KeyboardKey::Break:
        return "Break";
    case KeyboardKey::Grave:
        return "`";
    case KeyboardKey::Tab:
        return "Tab";
    case KeyboardKey::CapsLock:
        return "Caps Lock";
    case KeyboardKey::NumLock:
        return "Num Lock";
    case KeyboardKey::ScrollLock:
        return "Scroll Lock";
    case KeyboardKey::LeftSuper:
        return "Left Super";
    case KeyboardKey::RightSuper:
        return "Right Super";
    case KeyboardKey::Menu:
        return "Menu";
    case KeyboardKey::Left:
        return "Left";
    case KeyboardKey::Right:
        return "Right";
    case KeyboardKey::Up:
        return "Up";
    case KeyboardKey::Down:
        return "Down";
    case KeyboardKey::Minus:
        return "-";
    case KeyboardKey::Equals:
        return "=";
    case KeyboardKey::Semicolon:
        return ";";
    case KeyboardKey::Quote:
        return "'";
    case KeyboardKey::Slash:
        return "/";
    case KeyboardKey::Backslash:
        return "\\";
    case KeyboardKey::Comma:
        return ",";
    case KeyboardKey::Period:
        return ".";
    case KeyboardKey::LeftBracket:
        return "[";
    case KeyboardKey::RightBracket:
        return "]";
    default: ;
    }

    return string();
};

} // namespace Toon

#endif // TOON_KEYBOARD_HPP
