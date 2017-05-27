#include "UI.h"

#include "Window.h"
#include "imgui/imgui.h"

bool UI::consoleSelected = false;
bool UI::optionsSelected = false;
bool UI::settingsSelected = false;
bool UI::showMainMenuBar = true;
bool UI::showTestWindow  = false;

void UI::StartUI(Window* aWindow) {}

void UI::UpdateUI() {}

void UI::RenderUI()
{
    // ImGui functions end here
    ImGui::Render();
}

void UI::DrawUI(ImDrawData* draw_data) {}
