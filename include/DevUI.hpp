#ifndef DEVUI_H
#define DEVUI_H

#include <Config.hpp>

#include <functional>

namespace DevUI
{
    void Start();
    void Render();

    void RegisterOptionsFunc(std::function<void()> func);

    extern bool consoleSelected;
    extern bool optionsSelected;
    extern bool settingsSelected;
    extern bool showMainMenuBar;
    extern bool showDemoWindow;
    extern bool showAxis;

    extern float objectColor[3];
}

#endif // DEVUI_H
