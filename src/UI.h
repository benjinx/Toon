#ifndef UI_H
#define UI_H

#include "Common.h"

class Window;
struct ImDrawData;

namespace UI
{
void StartUI(Window* aWindow);
void UpdateUI();
void RenderUI();
void DrawUI(ImDrawData* draw_data);

extern bool consoleSelected;
extern bool optionsSelected;
extern bool settingsSelected;
extern bool showMainMenuBar;
extern bool showTestWindow;
}

#endif // UI_H
