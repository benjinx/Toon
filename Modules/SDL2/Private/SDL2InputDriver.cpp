#include <Toon/SDL2/SDL2InputDriver.hpp>

#include <Toon/Log.hpp>

namespace Toon::SDL2 {

void SDL2InputDriver::ProcessEvent(SDL_Event * event)
{
    switch (event->type) {
    case SDL_KEYDOWN:
        {
            KeyboardKey key = GetSDLKey(event->key.keysym.sym);
            ToonLogInfo("%s Pressed", GetKeyboardKeyName(key));
        }
        break;
    case SDL_KEYUP:
        {
            KeyboardKey key = GetSDLKey(event->key.keysym.sym);
            ToonLogInfo("%s Released", GetKeyboardKeyName(key));
        }
        break;
    case SDL_MOUSEBUTTONDOWN:
        {
            MouseButton button = GetSDLMouseButton(event->button.button);
            ToonLogInfo("Mouse %s Pressed", GetMouseButtonName(button));
        }
        break;
    case SDL_MOUSEBUTTONUP:
        {
            MouseButton button = GetSDLMouseButton(event->button.button);
            ToonLogInfo("Mouse %s Released", GetMouseButtonName(button));
        }
        break;
    case SDL_MOUSEMOTION:
        {
        }
        break;
    case SDL_CONTROLLERBUTTONDOWN:
        {
            ControllerButton button = GetSDLControllerButton(event->cbutton.button);
            ToonLogInfo("%s Pressed", GetControllerButtonName(button));
        }
        break;
    case SDL_CONTROLLERBUTTONUP:
        {
            ControllerButton button = GetSDLControllerButton(event->cbutton.button);
            ToonLogInfo("%s Released", GetControllerButtonName(button));
        }
        break;
    case SDL_CONTROLLERDEVICEADDED:
        {
        }
        break;
    case SDL_CONTROLLERDEVICEREMOVED:
        {
        }
        break;
    default: ;
    }
}

glm::ivec2 SDL2InputDriver::GetMouseCoordinates() const
{
    int x, y;
    SDL_GetMouseState(&x, &y);
    return { x, y };
}

KeyboardKey SDL2InputDriver::GetSDLKey(int sdlKey)
{
    switch (sdlKey) {
    case SDLK_a:
        return KeyboardKey::A;
    case SDLK_b:
        return KeyboardKey::B;
    case SDLK_c:
        return KeyboardKey::C;
    case SDLK_d:
        return KeyboardKey::D;
    case SDLK_e:
        return KeyboardKey::E;
    case SDLK_f:
        return KeyboardKey::F;
    case SDLK_g:
        return KeyboardKey::G;
    case SDLK_h:
        return KeyboardKey::H;
    case SDLK_i:
        return KeyboardKey::I;
    case SDLK_j:
        return KeyboardKey::J;
    case SDLK_k:
        return KeyboardKey::K;
    case SDLK_l:
        return KeyboardKey::L;
    case SDLK_m:
        return KeyboardKey::M;
    case SDLK_n:
        return KeyboardKey::N;
    case SDLK_o:
        return KeyboardKey::O;
    case SDLK_p:
        return KeyboardKey::P;
    case SDLK_q:
        return KeyboardKey::Q;
    case SDLK_r:
        return KeyboardKey::R;
    case SDLK_s:
        return KeyboardKey::S;
    case SDLK_t:
        return KeyboardKey::T;
    case SDLK_u:
        return KeyboardKey::U;
    case SDLK_v:
        return KeyboardKey::V;
    case SDLK_w:
        return KeyboardKey::W;
    case SDLK_x:
        return KeyboardKey::X;
    case SDLK_y:
        return KeyboardKey::Y;
    case SDLK_z:
        return KeyboardKey::Z;
    case SDLK_0:
        return KeyboardKey::Num0;
    case SDLK_1:
        return KeyboardKey::Num1;
    case SDLK_2:
        return KeyboardKey::Num2;
    case SDLK_3:
        return KeyboardKey::Num3;
    case SDLK_4:
        return KeyboardKey::Num4;
    case SDLK_5:
        return KeyboardKey::Num5;
    case SDLK_6:
        return KeyboardKey::Num6;
    case SDLK_7:
        return KeyboardKey::Num7;
    case SDLK_8:
        return KeyboardKey::Num8;
    case SDLK_9:
        return KeyboardKey::Num9;
    case SDLK_KP_0:
        return KeyboardKey::Numpad0;
    case SDLK_KP_1:
        return KeyboardKey::Numpad1;
    case SDLK_KP_2:
        return KeyboardKey::Numpad2;
    case SDLK_KP_3:
        return KeyboardKey::Numpad3;
    case SDLK_KP_4:
        return KeyboardKey::Numpad4;
    case SDLK_KP_5:
        return KeyboardKey::Numpad5;
    case SDLK_KP_6:
        return KeyboardKey::Numpad6;
    case SDLK_KP_7:
        return KeyboardKey::Numpad7;
    case SDLK_KP_8:
        return KeyboardKey::Numpad8;
    case SDLK_KP_9:
        return KeyboardKey::Numpad9;
    case SDLK_KP_PLUS:
        return KeyboardKey::NumpadPlus;
    case SDLK_KP_MINUS:
        return KeyboardKey::NumpadMinus;
    case SDLK_KP_MULTIPLY:
        return KeyboardKey::NumpadMultiply;
    case SDLK_KP_DIVIDE:
        return KeyboardKey::NumpadDivide;
    case SDLK_KP_ENTER:
        return KeyboardKey::NumpadEnter;
    case SDLK_KP_PERIOD:
        return KeyboardKey::NumpadPeriod;
    case SDLK_F1:
        return KeyboardKey::F1;
    case SDLK_F2:
        return KeyboardKey::F2;
    case SDLK_F3:
        return KeyboardKey::F3;
    case SDLK_F4:
        return KeyboardKey::F4;
    case SDLK_F5:
        return KeyboardKey::F5;
    case SDLK_F6:
        return KeyboardKey::F6;
    case SDLK_F7:
        return KeyboardKey::F7;
    case SDLK_F8:
        return KeyboardKey::F8;
    case SDLK_F9:
        return KeyboardKey::F9;
    case SDLK_F10:
        return KeyboardKey::F10;
    case SDLK_F11:
        return KeyboardKey::F11;
    case SDLK_F12:
        return KeyboardKey::F12;
    case SDLK_F13:
        return KeyboardKey::F13;
    case SDLK_F14:
        return KeyboardKey::F14;
    case SDLK_F15:
        return KeyboardKey::F15;
    case SDLK_F16:
        return KeyboardKey::F16;
    case SDLK_F17:
        return KeyboardKey::F17;
    case SDLK_F18:
        return KeyboardKey::F18;
    case SDLK_F19:
        return KeyboardKey::F19;
    case SDLK_F20:
        return KeyboardKey::F20;
    case SDLK_F21:
        return KeyboardKey::F21;
    case SDLK_F22:
        return KeyboardKey::F22;
    case SDLK_F23:
        return KeyboardKey::F23;
    case SDLK_F24:
        return KeyboardKey::F24;
    case SDLK_ESCAPE:
        return KeyboardKey::Escape;
    case SDLK_DELETE:
        return KeyboardKey::Delete;
    case SDLK_INSERT:
        return KeyboardKey::Insert;
    case SDLK_HOME:
        return KeyboardKey::Home;
    case SDLK_END:
        return KeyboardKey::End;
    case SDLK_PAGEUP:
        return KeyboardKey::PageUp;
    case SDLK_PAGEDOWN:
        return KeyboardKey::PageDown;
    case SDLK_BACKSPACE:
        return KeyboardKey::Backspace;
    case SDLK_RETURN:
        return KeyboardKey::Enter;
    case SDLK_SPACE:
        return KeyboardKey::Space;
    case SDLK_LSHIFT:
        return KeyboardKey::LeftShift;
    case SDLK_RSHIFT:
        return KeyboardKey::RightShift;
    case SDLK_LCTRL:
        return KeyboardKey::LeftControl;
    case SDLK_RCTRL:
        return KeyboardKey::RightControl;
    case SDLK_LALT:
        return KeyboardKey::LeftAlt;
    case SDLK_RALT:
        return KeyboardKey::RightAlt;
    case SDLK_PRINTSCREEN:
        return KeyboardKey::PrintScreen;
    case SDLK_PAUSE:
        return KeyboardKey::Break;
    case SDLK_BACKQUOTE:
        return KeyboardKey::Grave;
    case SDLK_TAB:
        return KeyboardKey::Tab;
    case SDLK_CAPSLOCK:
        return KeyboardKey::CapsLock;
    case SDLK_NUMLOCKCLEAR:
        return KeyboardKey::NumLock;
    case SDLK_SCROLLLOCK:
        return KeyboardKey::ScrollLock;
    case SDLK_LGUI:
        return KeyboardKey::LeftSuper;
    case SDLK_RGUI:
        return KeyboardKey::RightSuper;
    case SDLK_MENU:
        return KeyboardKey::Menu;
    case SDLK_LEFT:
        return KeyboardKey::Left;
    case SDLK_RIGHT:
        return KeyboardKey::Right;
    case SDLK_UP:
        return KeyboardKey::Up;
    case SDLK_DOWN:
        return KeyboardKey::Down;
    case SDLK_MINUS:
        return KeyboardKey::Minus;
    case SDLK_EQUALS:
        return KeyboardKey::Equals;
    case SDLK_SEMICOLON:
        return KeyboardKey::Semicolon;
    case SDLK_QUOTE:
        return KeyboardKey::Quote;
    case SDLK_SLASH:
        return KeyboardKey::Slash;
    case SDLK_BACKSLASH:
        return KeyboardKey::Backslash;
    case SDLK_COMMA:
        return KeyboardKey::Comma;
    case SDLK_PERIOD:
        return KeyboardKey::Period;
    case SDLK_LEFTBRACKET:
        return KeyboardKey::LeftBracket;
    case SDLK_RIGHTBRACKET:
        return KeyboardKey::RightBracket;
    }

    return KeyboardKey::Unknown;
}

MouseButton SDL2InputDriver::GetSDLMouseButton(int sdlButton)
{
    switch (sdlButton) {
    case SDL_BUTTON_LEFT:
        return MouseButton::Left;
    case SDL_BUTTON_RIGHT:
        return MouseButton::Right;
    case SDL_BUTTON_MIDDLE:
        return MouseButton::Middle;
    }

    return MouseButton::Unknown;
}

ControllerButton SDL2InputDriver::GetSDLControllerButton(int sdlButton)
{
    switch (sdlButton) {
    case SDL_CONTROLLER_BUTTON_A:
        return ControllerButton::A;
    case SDL_CONTROLLER_BUTTON_B:
        return ControllerButton::B;
    case SDL_CONTROLLER_BUTTON_X:
        return ControllerButton::X;
    case SDL_CONTROLLER_BUTTON_Y:
        return ControllerButton::Y;
    case SDL_CONTROLLER_BUTTON_BACK:
        return ControllerButton::Back;
    case SDL_CONTROLLER_BUTTON_GUIDE:
        return ControllerButton::Guide;
    case SDL_CONTROLLER_BUTTON_START:
        return ControllerButton::Start;
    case SDL_CONTROLLER_BUTTON_LEFTSTICK:
        return ControllerButton::LeftStick;
    case SDL_CONTROLLER_BUTTON_RIGHTSTICK:
        return ControllerButton::RightStick;
    case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
        return ControllerButton::LeftShoulder;
    case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
        return ControllerButton::RightShoulder;
    case SDL_CONTROLLER_BUTTON_DPAD_UP:
        return ControllerButton::DPadUp;
    case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
        return ControllerButton::DPadDown;
    case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
        return ControllerButton::DPadLeft;
    case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
        return ControllerButton::DPadRight;
    }

    return ControllerButton::Unknown;
}

} // namespace Toon::SDL2
