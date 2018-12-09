#ifndef UI_H
#define UI_H

#include "Config.hpp"

struct ImDrawData;

namespace UI
{
	void StartUI();
	void UpdateUI();
	void RenderUI();
	void DrawUI(ImDrawData* draw_data);

	extern bool consoleSelected;
	extern bool optionsSelected;
	extern bool settingsSelected;
	extern bool showMainMenuBar;
	extern bool showTestWindow;
	extern bool showAxis;

	extern float objectColor[3];
}

#endif // UI_H
