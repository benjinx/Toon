#include <Input.hpp>

#include <App.hpp>
#include <Camera.hpp>
#include <Log.hpp>
#include <DevUI.hpp>

#include <cmath>

Input::Input()
{
    // Initialize each one with the defaults
    _mMouseButtonMap.emplace("LEFT_CLICK", MouseButton::MOUSE_BUTTON_LEFT);
    _mMouseButtonMap.emplace("MIDDLE_CLICK", MouseButton::MOUSE_BUTTON_MIDDLE);
    _mMouseButtonMap.emplace("RIGHT_CLICK", MouseButton::MOUSE_BUTTON_RIGHT);

    //_mKeyboardMap.emplace("QUIT", KeyboardKey::ESC);
}

Input::~Input()
{

}

void Input::InitControllers(Sint32 controllerID)
{
    int const MAX_JOYSTICKS = SDL_NumJoysticks();

    for (int i = 0; i < MAX_JOYSTICKS; ++i)
    {
        if (SDL_IsGameController(i))
        {
            _mGameControllers.emplace(controllerID, SDL_GameControllerOpen(i));
        }
    }

    for (size_t j = 0; j < _mGameControllers.size(); ++j)
        LogInfo("Controller Added: %d, %s", controllerID, SDL_GameControllerName(_mGameControllers[j]));

}

void Input::RemoveControllers(Sint32 controllerID)
{
    // Clear all controllers
    _mGameControllers.clear();
    LogInfo("All Controllers removed.");

    // Readd all controllers
    InitControllers(controllerID);
}

void Input::ProcessEvent(SDL_Event* event)
{
    if (event->type == SDL_KEYDOWN) //|| event->type == SDL_KEYUP)
    {
        // Handle Keyboard
        SDL_Keycode key = SDLKeyToMyKey(event->key.keysym.sym);

        int mod = event->key.keysym.mod;

        auto camera = App::Inst()->GetCurrentCamera();
        auto dir = camera->GetDirection();

        // Then change the switch below to match ours
        switch (key)
        {
            case KEY_UNKNOWN:
                //LogInfo("Unknown Key: %d", event->key.keysym.sym);
                break;
            case KEY_RETURN:
                break;
            case KEY_ESCAPE:
                break;
            case KEY_BACKSPACE:
                break;
            case KEY_TAB:
                break;
            case KEY_SPACE:
                break;
            case KEY_EXCLAIM:
                break;
            case KEY_QUOTEDBL:
                break;
            case KEY_HASH:
                break;
            case KEY_PERCENT:
                break;
            case KEY_DOLLAR:
                break;
            case KEY_AMPERSAND:
                break;
            case KEY_QUOTE:
                break;
            case KEY_LEFTPAREN:
                break;
            case KEY_RIGHTPAREN:
                break;
            case KEY_ASTERISK:
                break;
            case KEY_PLUS:
                break;
            case KEY_COMMA:
                break;
            case KEY_MINUS:
                break;
            case KEY_PERIOD:
                break;
            case KEY_SLASH:
                break;
            case KEY_0:
                break;
            case KEY_1:
                break;
            case KEY_2:
                break;
            case KEY_3:
                break;
            case KEY_4:
                break;
            case KEY_5:
                break;
            case KEY_6:
                break;
            case KEY_7:
                break;
            case KEY_8:
                break;
            case KEY_9:
                break;
            case KEY_COLON:
                break;
            case KEY_SEMICOLON:
                break;
            case KEY_LESS:
                break;
            case KEY_EQUALS:
                break;
            case KEY_GREATER:
                break;
            case KEY_QUESTION:
                break;
            case KEY_AT:
                break;
            case KEY_LEFTBRACKET:
                break;
            case KEY_BACKSLASH:
                break;
            case KEY_RIGHTBRACKET:
                break;
            case KEY_CARET:
                break;
            case KEY_UNDERSCORE:
                break;
            case KEY_BACKQUOTE:
                break;
            case KEY_A:
                camera->SetDirection({ -1, dir.y, dir.z });
                break;
            case KEY_B:
                break;
            case KEY_C:
                break;
            case KEY_D:
                camera->SetDirection({ 1, dir.y, dir.z });
                break;
            case KEY_E:
                camera->SetDirection({ dir.x, -1, dir.z });
                break;
            case KEY_F:
                break;
            case KEY_G:
                break;
            case KEY_H:
                break;
            case KEY_I:
                break;
            case KEY_J:
                break;
            case KEY_K:
                break;
            case KEY_L:
                break;
            case KEY_M:
                break;
            case KEY_N:
                break;
            case KEY_O:
                break;
            case KEY_P:
                break;
            case KEY_Q:
                camera->SetDirection({ dir.x, 1, dir.z });
                break;
            case KEY_R:
                break;
            case KEY_S:
                camera->SetDirection({ dir.x, dir.y, -1 });
                break;
            case KEY_T:
                break;
            case KEY_U:
                break;
            case KEY_V:
                break;
            case KEY_W:
                camera->SetDirection({ dir.x, dir.y, 1 });
                break;
            case KEY_X:
                break;
            case KEY_Y:
                break;
            case KEY_Z:
                break;
            case KEY_CAPSLOCK:
                break;
            case KEY_F1:
                break;
            case KEY_F2:
                break;
            case KEY_F3:
                break;
            case KEY_F4:
                break;
            case KEY_F5:
                App::Inst()->ReloadShaders();
                break;
            case KEY_F6:
                break;
            case KEY_F7:
                break;
            case KEY_F8:
                break;
            case KEY_F9:
                break;
            case KEY_F10:
                break;
            case KEY_F11:
                break;
            case KEY_F12:
                break;
            case KEY_PRINTSCREEN:
                App::Inst()->Screenshot();
                break;
            case KEY_SCROLLLOCK:
                break;
            case KEY_PAUSE:
                break;
            case KEY_INSERT:
                break;
            case KEY_HOME:
                break;
            case KEY_PAGEUP:
                break;
            case KEY_DELETE:
                break;
            case KEY_END:
                break;
            case KEY_PAGEDOWN:
                break;
            case KEY_RIGHT:
                break;
            case KEY_LEFT:
                break;
            case KEY_DOWN:
                break;
            case KEY_UP:
                break;
            case KEY_NUMLOCKCLEAR:
                break;
            case KEY_KP_DIVIDE:
                break;
            case KEY_KP_MULTIPLY:
                break;
            case KEY_KP_MINUS:
                break;
            case KEY_KP_PLUS:
                break;
            case KEY_KP_ENTER:
                break;
            case KEY_KP_1:
                break;
            case KEY_KP_2:
                break;
            case KEY_KP_3:
                break;
            case KEY_KP_4:
                break;
            case KEY_KP_5:
                break;
            case KEY_KP_6:
                break;
            case KEY_KP_7:
                break;
            case KEY_KP_8:
                break;
            case KEY_KP_9:
                break;
            case KEY_KP_0:
                break;
            case KEY_KP_PERIOD:
                break;
        }
    }
    else if (event->type == SDL_KEYUP)
    {
        // Handle Keyboard
        SDL_Keycode key = SDLKeyToMyKey(event->key.keysym.sym);

        int mod = event->key.keysym.mod;

        auto camera = App::Inst()->GetCurrentCamera();
        auto dir = camera->GetDirection();

        // Then change the switch below to match ours
        switch (key)
        {
        case KEY_UNKNOWN:
            //LogInfo("Unknown Key: %d", event->key.keysym.sym);
            break;
        case KEY_RETURN:
            break;
        case KEY_ESCAPE:
            DevUI::settingsSelected = !DevUI::settingsSelected;
            break;
        case KEY_BACKSPACE:
            break;
        case KEY_TAB:
            break;
        case KEY_SPACE:
            break;
        case KEY_EXCLAIM:
            break;
        case KEY_QUOTEDBL:
            break;
        case KEY_HASH:
            break;
        case KEY_PERCENT:
            break;
        case KEY_DOLLAR:
            break;
        case KEY_AMPERSAND:
            break;
        case KEY_QUOTE:
            break;
        case KEY_LEFTPAREN:
            break;
        case KEY_RIGHTPAREN:
            break;
        case KEY_ASTERISK:
            break;
        case KEY_PLUS:
            break;
        case KEY_COMMA:
            break;
        case KEY_MINUS:
            break;
        case KEY_PERIOD:
            break;
        case KEY_SLASH:
            break;
        case KEY_0:
            break;
        case KEY_1:
            break;
        case KEY_2:
            break;
        case KEY_3:
            break;
        case KEY_4:
            break;
        case KEY_5:
            break;
        case KEY_6:
            break;
        case KEY_7:
            break;
        case KEY_8:
            break;
        case KEY_9:
            break;
        case KEY_COLON:
            break;
        case KEY_SEMICOLON:
            break;
        case KEY_LESS:
            break;
        case KEY_EQUALS:
            break;
        case KEY_GREATER:
            break;
        case KEY_QUESTION:
            break;
        case KEY_AT:
            break;
        case KEY_LEFTBRACKET:
            break;
        case KEY_BACKSLASH:
            break;
        case KEY_RIGHTBRACKET:
            break;
        case KEY_CARET:
            break;
        case KEY_UNDERSCORE:
            break;
        case KEY_BACKQUOTE:
            break;
        case KEY_A:
            camera->SetDirection({ 0, dir.y, dir.z });
            break;
        case KEY_B:
            break;
        case KEY_C:
            break;
        case KEY_D:
            camera->SetDirection({ 0, dir.y, dir.z });
            break;
        case KEY_E:
            camera->SetDirection({ dir.x, 0, dir.z });
            break;
        case KEY_F:
            break;
        case KEY_G:
            break;
        case KEY_H:
            break;
        case KEY_I:
            break;
        case KEY_J:
            break;
        case KEY_K:
            break;
        case KEY_L:
            break;
        case KEY_M:
            break;
        case KEY_N:
            break;
        case KEY_O:
            break;
        case KEY_P:
            break;
        case KEY_Q:
            camera->SetDirection({ dir.x, 0, dir.z });
            break;
        case KEY_R:
            break;
        case KEY_S:
            camera->SetDirection({ dir.x, dir.y, 0 });
            break;
        case KEY_T:
            break;
        case KEY_U:
            break;
        case KEY_V:
            break;
        case KEY_W:
            camera->SetDirection({ dir.x, dir.y, 0 });
            break;
        case KEY_X:
            if (mod & KMOD_CTRL)
            {
                DevUI::showAxis = !DevUI::showAxis;
            }
            break;
        case KEY_Y:
            break;
        case KEY_Z:
            break;
        case KEY_CAPSLOCK:
            break;
        case KEY_F1:
            break;
        case KEY_F2:
            DevUI::consoleSelected = !DevUI::consoleSelected;
            break;
        case KEY_F3:
            DevUI::optionsSelected = !DevUI::optionsSelected;
            break;
        case KEY_F4:
            break;
        case KEY_F5:
            break;
        case KEY_F6:
            break;
        case KEY_F7:
            break;
        case KEY_F8:
            break;
        case KEY_F9:
            break;
        case KEY_F10:
            break;
        case KEY_F11:
            DevUI::showDemoWindow = !DevUI::showDemoWindow;
            break;
        case KEY_F12:
            break;
        case KEY_PRINTSCREEN:
            break;
        case KEY_SCROLLLOCK:
            break;
        case KEY_PAUSE:
            break;
        case KEY_INSERT:
            break;
        case KEY_HOME:
            break;
        case KEY_PAGEUP:
            break;
        case KEY_DELETE:
            break;
        case KEY_END:
            break;
        case KEY_PAGEDOWN:
            break;
        case KEY_RIGHT:
            break;
        case KEY_LEFT:
            break;
        case KEY_DOWN:
            break;
        case KEY_UP:
            break;
        case KEY_NUMLOCKCLEAR:
            break;
        case KEY_KP_DIVIDE:
            break;
        case KEY_KP_MULTIPLY:
            break;
        case KEY_KP_MINUS:
            break;
        case KEY_KP_PLUS:
            break;
        case KEY_KP_ENTER:
            break;
        case KEY_KP_1:
            break;
        case KEY_KP_2:
            break;
        case KEY_KP_3:
            break;
        case KEY_KP_4:
            break;
        case KEY_KP_5:
            break;
        case KEY_KP_6:
            break;
        case KEY_KP_7:
            break;
        case KEY_KP_8:
            break;
        case KEY_KP_9:
            break;
        case KEY_KP_0:
            break;
        case KEY_KP_PERIOD:
            break;
        }
    }
    else if (event->type == SDL_MOUSEMOTION)
    {
        float x = (float)event->motion.x;
        float y = (float)event->motion.y;

        if (_mLastMouseX < 0 && _mLastMouseY < 0) {
            _mLastMouseX = (float)x;
            _mLastMouseY = (float)y;
        }

        float xoffset = (float)x - _mLastMouseX;
        float yoffset = _mLastMouseY - (float)y;

        _mLastMouseX = (float)x;
        _mLastMouseY = (float)y;

        if (_mRightButtonDown)
            App::Inst()->GetCurrentCamera()->HandleRotation(xoffset, yoffset);

    }
    else if (event->type == SDL_MOUSEWHEEL)
    {
        // Add any interaction based on mouse wheel
        //App::Inst()->GetCurrentCamera()->HandleFoV((float)xoffset, (float)yoffset);
    }
    else if (event->type == SDL_MOUSEBUTTONDOWN)
    {
        // Handle Mouse
        auto mouse = SDLMouseButtonToMyButton(event->button.button);

        switch (mouse)
        {
        case MOUSE_BUTTON_UNKNOWN:
            //LogInfo("Unknown Mouse Button Down: %d", event->button.button);
            break;
        case MOUSE_BUTTON_LEFT:
            //LogInfo("Left Mouse Button Down: %d", event->button.button);
            break;
        case MOUSE_BUTTON_MIDDLE:
            //LogInfo("Middle Mouse Button Down: %d", event->button.button);
            break;
        case MOUSE_BUTTON_RIGHT:
            //LogInfo("Right Mouse Button Down: %d", event->button.button);
            _mRightButtonDown = true;
            break;
        }
    }
    else if (event->type == SDL_MOUSEBUTTONUP)
    {
        // Handle Mouse
        auto mouse = SDLMouseButtonToMyButton(event->button.button);

        switch (mouse)
        {
        case MOUSE_BUTTON_UNKNOWN:
            break;
        case MOUSE_BUTTON_LEFT:
            //LogInfo("Left Mouse Button Up: %d", event->button.button);
            break;
        case MOUSE_BUTTON_MIDDLE:
            //LogInfo("Middle Mouse Button Up: %d", event->button.button);
            break;
        case MOUSE_BUTTON_RIGHT:
            //LogInfo("Right Mouse Button Up: %d", event->button.button);
            _mRightButtonDown = false;
            break;
        }
    }
    else if (event->type == SDL_CONTROLLERDEVICEADDED)
    {
        InitControllers(event->cdevice.which);
    }
    else if (event->type == SDL_CONTROLLERDEVICEREMOVED)
    {
        RemoveControllers(event->cdevice.which);
    }
    else if (event->type == SDL_CONTROLLERDEVICEREMAPPED)
    {
        RemoveControllers(event->cdevice.which);
    }
    else if (event->type == SDL_CONTROLLERBUTTONDOWN)
    {
        // Handle Gamepad
        auto gamepad = SDLGamepadButtonToMyButton(event->cbutton.button);

        switch (gamepad)
        {
            case GAME_CONTROLLER_BUTTON_INVALID:
                LogInfo("Controller Button Invalid: %d", event->cbutton.button);
                break;
            case GAME_CONTROLLER_BUTTON_A:
                LogInfo("Controller Button A: %d", event->cbutton.button);
                break;
            case GAME_CONTROLLER_BUTTON_B:
                LogInfo("Controller Button B: %d", event->cbutton.button);
                break;
            case GAME_CONTROLLER_BUTTON_X:
                LogInfo("Controller Button X: %d", event->cbutton.button);
                break;
            case GAME_CONTROLLER_BUTTON_Y:
                LogInfo("Controller Button Y: %d", event->cbutton.button);
                break;
            case GAME_CONTROLLER_BUTTON_BACK:
                LogInfo("Controller Button Back: %d", event->cbutton.button);
                break;
            case GAME_CONTROLLER_BUTTON_GUIDE:
                LogInfo("Controller Button Guide: %d", event->cbutton.button);
                break;
            case GAME_CONTROLLER_BUTTON_START:
                LogInfo("Controller Button Start: %d", event->cbutton.button);
                break;
            case GAME_CONTROLLER_BUTTON_LEFTSTICK:
                LogInfo("Controller Button Left Stick: %d", event->cbutton.button);
                break;
            case GAME_CONTROLLER_BUTTON_RIGHTSTICK:
                LogInfo("Controller Button Right Stick: %d", event->cbutton.button);
                break;
            case GAME_CONTROLLER_BUTTON_LEFTSHOULDER:
                LogInfo("Controller Button Left Shoulder: %d", event->cbutton.button);
                break;
            case GAME_CONTROLLER_BUTTON_RIGHTSHOULDER:
                LogInfo("Controller Button Right Shoulder: %d", event->cbutton.button);
                break;
            case GAME_CONTROLLER_BUTTON_DPAD_UP:
                LogInfo("Controller Button Dpad Up: %d", event->cbutton.button);
                break;
            case GAME_CONTROLLER_BUTTON_DPAD_DOWN:
                LogInfo("Controller Button Dpad Down: %d", event->cbutton.button);
                break;
            case GAME_CONTROLLER_BUTTON_DPAD_LEFT:
                LogInfo("Controller Button Dpad Left: %d", event->cbutton.button);
                break;
            case GAME_CONTROLLER_BUTTON_DPAD_RIGHT:
                LogInfo("Controller Button Dpad Right: %d", event->cbutton.button);
                break;
            case GAME_CONTROLLER_BUTTON_MAX:
                LogInfo("Controller Button Max: %d", event->cbutton.button);
                break;
        }
    }
    else if (event->type == SDL_CONTROLLERAXISMOTION)
    {
        // Using the game controller and casting the axis we can find which one is being triggered.
        Sint16 axis = SDL_GameControllerGetAxis(
                    _mGameControllers[event->cdevice.which],
                    (SDL_GameControllerAxis)event->caxis.axis);

        //LogInfo("Axis value: %d\n", axis);
        
        // handle x motion w/ dead zone.
        if ((SDL_GameControllerAxis)event->caxis.axis % 2 == 0)
        {
            // Left of the dead zone
            if (axis < -JOYSTICK_DEAD_ZONE)
            {
                _mXDir = -1;
            }
            // Right of the dead zone
            else if (axis > JOYSTICK_DEAD_ZONE)
            {
                _mXDir = 1;
            }
            else
            {
                _mXDir = 0;
            }

            LogInfo("xDir = %d", _mXDir);
        }
        // handle y motion w/ dead zone.
        else if ((SDL_GameControllerAxis)event->caxis.axis % 2 == 1)
        {
            // Below the dead zone
            if (axis < -JOYSTICK_DEAD_ZONE)
            {
                _mYDir = -1;
            }
            // Above the dead zone
            else if (axis > JOYSTICK_DEAD_ZONE)
            {
                _mYDir = 1;
            }
            else
            {
                _mYDir = 0;
            }

            LogInfo("yDir = %d", _mYDir);
        }

        // formula: angle = atan2((double)yDir, (double)xDir) * (180/PI);
        double angle = atan2((double)_mYDir, (double)_mXDir) * (180.0 / M_PI);

        // Correct angle
        if (_mXDir == 0 && _mYDir == 0)
            angle = 0;

        //LogInfo("Axis Angle: %d\n", angle);
    }

}

Input::KeyboardKey Input::SDLKeyToMyKey(SDL_Keycode key)
{
    switch (key)
    {
        case SDLK_UNKNOWN:          //= 0,
            return KEY_UNKNOWN;
        case SDLK_RETURN:           //= '\r',
            return KEY_RETURN;
        case SDLK_ESCAPE:           //= '\033',
            return KEY_ESCAPE;
        case SDLK_BACKSPACE:        //= '\b',
            return KEY_BACKSPACE;
        case SDLK_TAB:              //= '\t',
            return KEY_TAB;
        case SDLK_SPACE:            //= ' ',
            return KEY_SPACE;
        case SDLK_EXCLAIM:          //= '!',
            return KEY_EXCLAIM;
        case SDLK_QUOTEDBL:         //= '"',
            return KEY_QUOTEDBL;
        case SDLK_HASH:             //= '#',
            return KEY_HASH;
        case SDLK_PERCENT:          //= '%',
            return KEY_PERCENT;
        case SDLK_DOLLAR:           //= '$',
            return KEY_DOLLAR;
        case SDLK_AMPERSAND:        //= '&',
            return KEY_AMPERSAND;
        case SDLK_QUOTE:            //= '\'',
            return KEY_QUOTE;
        case SDLK_LEFTPAREN:        //= '(',
            return KEY_LEFTPAREN;
        case SDLK_RIGHTPAREN:       //= ')',
            return KEY_RIGHTPAREN;
        case SDLK_ASTERISK:         //= '*',
            return KEY_ASTERISK;
        case SDLK_PLUS:             //= '+',
            return KEY_PLUS;
        case SDLK_COMMA:            //= ',',
            return KEY_COMMA;
        case SDLK_MINUS:            //= '-',
            return KEY_MINUS;
        case SDLK_PERIOD:           //= '.',
            return KEY_PERIOD;
        case SDLK_SLASH:            //= '/',
            return KEY_SLASH;
        case SDLK_0:                //= '0',
            return KEY_0;
        case SDLK_1:                //= '1',
            return KEY_1;
        case SDLK_2:                //= '2',
            return KEY_2;
        case SDLK_3:                //= '3',
            return KEY_3;
        case SDLK_4:                //= '4',
            return KEY_4;
        case SDLK_5:                //= '5',
            return KEY_5;
        case SDLK_6:                //= '6',
            return KEY_6;
        case SDLK_7:                //= '7',
            return KEY_7;
        case SDLK_8:                //= '8',
            return KEY_8;
        case SDLK_9:                //= '9',
            return KEY_9;
        case SDLK_COLON:            //= ':',
            return KEY_COLON;
        case SDLK_SEMICOLON:        //= ';',
            return KEY_SEMICOLON;
        case SDLK_LESS:             //= '<',
            return KEY_LESS;
        case SDLK_EQUALS:           //= '=',
            return KEY_EQUALS;
        case SDLK_GREATER:          //= '>',
            return KEY_GREATER;
        case SDLK_QUESTION:         //= '?',
            return KEY_QUESTION;
        case SDLK_AT:               //= '@',
            return KEY_AT;
        case SDLK_LEFTBRACKET:      //= '[',
            return KEY_LEFTBRACKET;
        case SDLK_BACKSLASH:        //= '\\',
            return KEY_BACKSLASH;
        case SDLK_RIGHTBRACKET:     //= ']',
            return KEY_RIGHTBRACKET;
        case SDLK_CARET:            //= '^',
            return KEY_CARET;
        case SDLK_UNDERSCORE:       //= '_',
            return KEY_UNDERSCORE;
        case SDLK_BACKQUOTE:        //= '`',
            return KEY_BACKQUOTE;
        case SDLK_a:                //= 'a',
            return KEY_A;
        case SDLK_b:                //= 'b',
            return KEY_B;
        case SDLK_c:                //= 'c',
            return KEY_C;
        case SDLK_d:                //= 'd',
            return KEY_D;
        case SDLK_e:                //= 'e',
            return KEY_E;
        case SDLK_f:                //= 'f',
            return KEY_F;
        case SDLK_g:                //= 'g',
            return KEY_G;
        case SDLK_h:                //= 'h',
            return KEY_H;
        case SDLK_i:                //= 'i',
            return KEY_I;
        case SDLK_j:                //= 'j',
            return KEY_J;
        case SDLK_k:                //= 'k',
            return KEY_K;
        case SDLK_l:                //= 'l',
            return KEY_L;
        case SDLK_m:                //= 'm',
            return KEY_M;
        case SDLK_n:                //= 'n',
            return KEY_N;
        case SDLK_o:                //= 'o',
            return KEY_O;
        case SDLK_p:                //= 'p',
            return KEY_P;
        case SDLK_q:                //= 'q',
            return KEY_Q;
        case SDLK_r:                //= 'r',
            return KEY_R;
        case SDLK_s:                //= 's',
            return KEY_S;
        case SDLK_t:                //= 't',
            return KEY_T;
        case SDLK_u:                //= 'u',
            return KEY_U;
        case SDLK_v:                //= 'v',
            return KEY_V;
        case SDLK_w:                //= 'w',
            return KEY_W;
        case SDLK_x:                //= 'x',
            return KEY_X;
        case SDLK_y:                //= 'y',
            return KEY_Y;
        case SDLK_z:                //= 'z',
            return KEY_Z;
        case SDLK_CAPSLOCK:         //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CAPSLOCK),
            return KEY_CAPSLOCK;
        case SDLK_F1:               //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F1),
            return KEY_F1;
        case SDLK_F2:               //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F2),
            return KEY_F2;
        case SDLK_F3:               //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F3),
            return KEY_F3;
        case SDLK_F4:               //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F4),
            return KEY_F4;
        case SDLK_F5:               //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F5),
            return KEY_F5;
        case SDLK_F6:               //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F6),
            return KEY_F6;
        case SDLK_F7:               //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F7),
            return KEY_F7;
        case SDLK_F8:               //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F8),
            return KEY_F8;
        case SDLK_F9:               //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F9),
            return KEY_F9;
        case SDLK_F10:              //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F10),
            return KEY_F10;
        case SDLK_F11:              //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F11),
            return KEY_F11;
        case SDLK_F12:              //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F12),
            return KEY_F12;
        case SDLK_PRINTSCREEN:      //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PRINTSCREEN),
            return KEY_PRINTSCREEN;
        case SDLK_SCROLLLOCK:       //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SCROLLLOCK),
            return KEY_SCROLLLOCK;
        case SDLK_PAUSE:            //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PAUSE),
            return KEY_PAUSE;
        case SDLK_INSERT:           //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_INSERT),
            return KEY_INSERT;
        case SDLK_HOME:             //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_HOME),
            return KEY_HOME;
        case SDLK_PAGEUP:           //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PAGEUP),
            return KEY_PAGEUP;
        case SDLK_DELETE:           //= '\177',
            return KEY_DELETE;
        case SDLK_END:              //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_END),
            return KEY_END;
        case SDLK_PAGEDOWN:         //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PAGEDOWN),
            return KEY_PAGEDOWN;
        case SDLK_RIGHT:            //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_RIGHT),
            return KEY_RIGHT;
        case SDLK_LEFT:             //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_LEFT),
            return KEY_LEFT;
        case SDLK_DOWN:             //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_DOWN),
            return KEY_DOWN;
        case SDLK_UP:               //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_UP),
            return KEY_UP;
        case SDLK_NUMLOCKCLEAR:     //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMLOCKCLEAR),
            return KEY_NUMLOCKCLEAR;
        case SDLK_KP_DIVIDE:        //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_DIVIDE),
            return KEY_KP_DIVIDE;
        case SDLK_KP_MULTIPLY:      //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MULTIPLY),
            return KEY_KP_MULTIPLY;
        case SDLK_KP_MINUS:         //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MINUS),
            return KEY_KP_MINUS;
        case SDLK_KP_PLUS:          //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_PLUS),
            return KEY_KP_PLUS;
        case SDLK_KP_ENTER:         //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_ENTER),
            return KEY_KP_ENTER;
        case SDLK_KP_1:             //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_1),
            return KEY_KP_1;
        case SDLK_KP_2:             //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_2),
            return KEY_KP_2;
        case SDLK_KP_3:             //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_3),
            return KEY_KP_3;
        case SDLK_KP_4:             //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_4),
            return KEY_KP_4;
        case SDLK_KP_5:             //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_5),
            return KEY_KP_5;
        case SDLK_KP_6:             //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_6),
            return KEY_KP_6;
        case SDLK_KP_7:             //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_7),
            return KEY_KP_7;
        case SDLK_KP_8:             //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_8),
            return KEY_KP_8;
        case SDLK_KP_9:             //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_9),
            return KEY_KP_9;
        case SDLK_KP_0:             //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_0),
            return KEY_KP_0;
        case SDLK_KP_PERIOD:        //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_PERIOD),
            return KEY_KP_PERIOD;
    }

    return KEY_UNKNOWN;
}

Input::MouseButton Input::SDLMouseButtonToMyButton(Uint8 mouseButton)
{
    switch (mouseButton)
    {
        case SDL_BUTTON_LEFT:
            return MOUSE_BUTTON_LEFT;
        case SDL_BUTTON_MIDDLE:
            return MOUSE_BUTTON_MIDDLE;
        case SDL_BUTTON_RIGHT:
            return MOUSE_BUTTON_RIGHT;
    }

    return MOUSE_BUTTON_UNKNOWN;
}

Input::GamepadButton Input::SDLGamepadButtonToMyButton(Uint8 button)
{
    switch (button)
    {
        case SDL_CONTROLLER_BUTTON_INVALID:
            return GAME_CONTROLLER_BUTTON_INVALID;
        case SDL_CONTROLLER_BUTTON_A:
            return GAME_CONTROLLER_BUTTON_A;
        case SDL_CONTROLLER_BUTTON_B:
            return GAME_CONTROLLER_BUTTON_B;
        case SDL_CONTROLLER_BUTTON_X:
            return GAME_CONTROLLER_BUTTON_X;
        case SDL_CONTROLLER_BUTTON_Y:
            return GAME_CONTROLLER_BUTTON_Y;
        case SDL_CONTROLLER_BUTTON_BACK:
            return GAME_CONTROLLER_BUTTON_BACK;
        case SDL_CONTROLLER_BUTTON_GUIDE:
            return GAME_CONTROLLER_BUTTON_GUIDE;
        case SDL_CONTROLLER_BUTTON_START:
            return GAME_CONTROLLER_BUTTON_START;
        case SDL_CONTROLLER_BUTTON_LEFTSTICK:
            return GAME_CONTROLLER_BUTTON_LEFTSTICK;
        case SDL_CONTROLLER_BUTTON_RIGHTSTICK:
            return GAME_CONTROLLER_BUTTON_RIGHTSTICK;
        case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
            return GAME_CONTROLLER_BUTTON_LEFTSHOULDER;
        case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
            return GAME_CONTROLLER_BUTTON_RIGHTSHOULDER;
        case SDL_CONTROLLER_BUTTON_DPAD_UP:
            return GAME_CONTROLLER_BUTTON_DPAD_UP;
        case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
            return GAME_CONTROLLER_BUTTON_DPAD_DOWN;
        case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
            return GAME_CONTROLLER_BUTTON_DPAD_LEFT;
        case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
            return GAME_CONTROLLER_BUTTON_DPAD_RIGHT;
        case SDL_CONTROLLER_BUTTON_MAX:
            return GAME_CONTROLLER_BUTTON_MAX;
    }
    return GAME_CONTROLLER_BUTTON_INVALID;
}