#ifndef INPUT_HPP
#define INPUT_HPP

#include <SDL.h>
#include <string>
#include <unordered_map>

class Input
{
public:
    Input();
    ~Input();

    // 
    void InitControllers(Sint32 controllerID);
    void RemoveControllers(Sint32 controllerID);
    void ProcessEvent(SDL_Event* event);

    // Need an event system
    // Globally fired events
    // Keybindings (Have a shoot, moveLeft, etc that we can bind to)
  
    enum MouseButton
    {
        MOUSE_BUTTON_UNKNOWN = 0,
        MOUSE_BUTTON_LEFT,
        MOUSE_BUTTON_MIDDLE,
        MOUSE_BUTTON_RIGHT,
    };

    enum KeyboardKey
    {
        KEY_UNKNOWN = 0,
        KEY_RETURN,
        KEY_ESCAPE,
        KEY_BACKSPACE,
        KEY_TAB,
        KEY_SPACE,
        KEY_EXCLAIM,
        KEY_QUOTEDBL,
        KEY_HASH,
        KEY_PERCENT,
        KEY_DOLLAR,
        KEY_AMPERSAND,
        KEY_QUOTE,
        KEY_LEFTPAREN,
        KEY_RIGHTPAREN,
        KEY_ASTERISK,
        KEY_PLUS,
        KEY_COMMA,
        KEY_MINUS,
        KEY_PERIOD,
        KEY_SLASH,
        KEY_0,
        KEY_1,
        KEY_2,
        KEY_3,
        KEY_4,
        KEY_5,
        KEY_6,
        KEY_7,
        KEY_8,
        KEY_9,
        KEY_COLON,
        KEY_SEMICOLON,
        KEY_LESS,
        KEY_EQUALS,
        KEY_GREATER,
        KEY_QUESTION,
        KEY_AT,
        KEY_LEFTBRACKET,
        KEY_BACKSLASH,
        KEY_RIGHTBRACKET,
        KEY_CARET,
        KEY_UNDERSCORE,
        KEY_BACKQUOTE,
        KEY_A,
        KEY_B,
        KEY_C,
        KEY_D,
        KEY_E,
        KEY_F,
        KEY_G,
        KEY_H,
        KEY_I,
        KEY_J,
        KEY_K,
        KEY_L,
        KEY_M,
        KEY_N,
        KEY_O,
        KEY_P,
        KEY_Q,
        KEY_R,
        KEY_S,
        KEY_T,
        KEY_U,
        KEY_V,
        KEY_W,
        KEY_X,
        KEY_Y,
        KEY_Z,
        KEY_CAPSLOCK,
        KEY_F1,
        KEY_F2,
        KEY_F3,
        KEY_F4,
        KEY_F5,
        KEY_F6,
        KEY_F7,
        KEY_F8,
        KEY_F9,
        KEY_F10,
        KEY_F11,
        KEY_F12,
        KEY_PRINTSCREEN,
        KEY_SCROLLLOCK,
        KEY_PAUSE,
        KEY_INSERT,
        KEY_HOME,
        KEY_PAGEUP,
        KEY_DELETE,
        KEY_END,
        KEY_PAGEDOWN,
        KEY_RIGHT,
        KEY_LEFT,
        KEY_DOWN,
        KEY_UP,
        KEY_NUMLOCKCLEAR,
        KEY_KP_DIVIDE,
        KEY_KP_MULTIPLY,
        KEY_KP_MINUS,
        KEY_KP_PLUS,
        KEY_KP_ENTER,
        KEY_KP_1,
        KEY_KP_2,
        KEY_KP_3,
        KEY_KP_4,
        KEY_KP_5,
        KEY_KP_6,
        KEY_KP_7,
        KEY_KP_8,
        KEY_KP_9,
        KEY_KP_0,
        KEY_KP_PERIOD,
    };

    enum GamepadButton
    {
        GAME_CONTROLLER_BUTTON_INVALID,
        GAME_CONTROLLER_BUTTON_A,
        GAME_CONTROLLER_BUTTON_B,
        GAME_CONTROLLER_BUTTON_X,
        GAME_CONTROLLER_BUTTON_Y,
        GAME_CONTROLLER_BUTTON_BACK,
        GAME_CONTROLLER_BUTTON_GUIDE,
        GAME_CONTROLLER_BUTTON_START,
        GAME_CONTROLLER_BUTTON_LEFTSTICK,
        GAME_CONTROLLER_BUTTON_RIGHTSTICK,
        GAME_CONTROLLER_BUTTON_LEFTSHOULDER,
        GAME_CONTROLLER_BUTTON_RIGHTSHOULDER,
        GAME_CONTROLLER_BUTTON_DPAD_UP,
        GAME_CONTROLLER_BUTTON_DPAD_DOWN,
        GAME_CONTROLLER_BUTTON_DPAD_LEFT,
        GAME_CONTROLLER_BUTTON_DPAD_RIGHT,
        GAME_CONTROLLER_BUTTON_MAX,
    };

private:
    
    KeyboardKey     SDLKeyToMyKey(SDL_Keycode key);
    MouseButton     SDLMouseButtonToMyButton(Uint8 mouseButton);
    GamepadButton   SDLGamepadButtonToMyButton(Uint8 button);

    std::unordered_map<std::string, KeyboardKey> _mKeyboardMap;
    std::unordered_map<std::string, MouseButton> _mMouseButtonMap;
    std::unordered_map<std::string, GamepadButton> _mGamepadMap;

    std::unordered_map<Sint32, SDL_GameController*> _mGameControllers;

    const int JOYSTICK_DEAD_ZONE = 8000;
    int _mXDir = 0, _mYDir = 0;
    float _mLastMouseX = 0.0f, _mLastMouseY = 0.0f;
    bool _mRightButtonDown = false;
    // So we need to map them correctly corrosponding to the SDL_KEY
    // then figure out how to connect the event occuring in SDL (when the key is pressed),
    //      to then fire our events.
    // We can "fire" an event whenever one of these things is triggered.

    // Still needs work:
    // Axis controls on the gamepad
};

#endif // INPUT_HPP
