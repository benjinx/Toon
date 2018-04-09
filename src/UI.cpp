#include "UI.hpp"

#include "Window.hpp"
#include "Application.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

bool UI::consoleSelected = false;
bool UI::optionsSelected = false;
bool UI::settingsSelected = false;
bool UI::showMainMenuBar = true;
bool UI::showTestWindow  = false;

void UI::StartUI()
{
	ImGui_ImplGlfwGL3_Init(Application::Inst()->GetWindow(), true);
}

void UI::UpdateUI()
{
	ImGui_ImplGlfwGL3_NewFrame();

	ImVec4             Red = ImVec4(255.0f, 0.0f, 0.0f, 255.0f);
	ImVec4             Yellow = ImVec4(255.0f, 255.0f, 0.0f, 255.0f);
	ImVec4             Green = ImVec4(0.0f, 255.0f, 0.0f, 255.0f);
	ImVec4             White = ImVec4(255.0f, 255.0f, 255.0f, 255.0f);

	if (UI::showMainMenuBar)
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("Tools"))
			{
				ImGui::MenuItem("Settings", "ESC", &UI::settingsSelected, true);
				ImGui::MenuItem("Console", "F1", &UI::consoleSelected, true);
				ImGui::MenuItem("Options", "F2", &UI::optionsSelected, true);
				ImGui::MenuItem("Test Window", "F11", &UI::showTestWindow, true);
				ImGui::EndMenu();
			}

			ImGui::SameLine((float)Application::Inst()->GetWindowWidth() - 150, 0.0f);
			ImGui::Text("%.2f FPS (%.2f ms)", ImGui::GetIO().Framerate, 1000.0f / ImGui::GetIO().Framerate);
			ImGui::EndMainMenuBar();
		}

		if (UI::consoleSelected)
		{
			ImGui::SetNextWindowSize(ImVec2(400, 200));
			ImGui::Begin("Console", &UI::consoleSelected);
			ImGui::BeginChild(1, ImVec2(390, 135));
			ImGui::TextColored(Red, "> Error: Test\n");
			ImGui::TextColored(Yellow, "> Warning: Test\n");
			ImGui::TextColored(Green, "> Loaded: Test\n");
			ImGui::TextColored(White, "> Normal Text: Test\n");
			ImGui::EndChild();
			ImGui::Text(">");
			ImGui::SameLine();
			//ImGui::InputText("", buffer, 255);
			ImGui::End();
		}

		if (UI::optionsSelected)
		{
			ImGui::Begin("Options", &UI::optionsSelected);
			ImGui::End();
		}

		if (UI::showTestWindow)
		{
			ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
			ImGui::ShowTestWindow(&UI::showTestWindow);
		}

		if (UI::settingsSelected)
		{
			ImGui::SetNextWindowSize(ImVec2(200, 400));
			ImGui::SetNextWindowPosCenter();
			ImGui::Begin("Settings", NULL, ImGuiWindowFlags_NoCollapse + ImGuiWindowFlags_AlwaysAutoResize);
			ImGui::Text("Credits\n");
			ImGui::TextColored(Green, "Created by BC/DC Games:\n");
			ImGui::TextColored(White, "- Benji Campbell\n");
			ImGui::TextColored(White, "- Daniel Covert\n");
			ImGui::TextColored(White, "- Stephen Lane-Walsh\n");
			ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

			if (ImGui::Button("Quit"))
				glfwSetWindowShouldClose(Application::Inst()->GetWindow(), GL_TRUE);
			ImGui::End();
		}
	}
}

void UI::RenderUI()
{
    // ImGui functions end here
    ImGui::Render();
}

void UI::DrawUI(ImDrawData* draw_data) {}
