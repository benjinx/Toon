#include "UI.hpp"

#include "Window.hpp"
#include "Application.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

bool UI::consoleSelected = false;
bool UI::optionsSelected = true;
bool UI::settingsSelected = false;
bool UI::showMainMenuBar = true;
bool UI::showTestWindow  = false;
bool UI::showAxis = true;
float UI::objectColor[3] = { 1.0f, 1.0f, 1.0f };

void UI::StartUI()
{
	ImGui::CreateContext();
	ImGui_ImplGlfwGL3_Init(Application::Inst()->GetWindow()->GetGLFWWindow(), true);
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

			ImGui::SameLine((float)Application::Inst()->GetWindow()->GetWidth() - 150, 0.0f);
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
			ImGui::SetNextWindowSize(ImVec2(400, 200));
			ImGui::Begin("Options", &UI::optionsSelected);
			ImGui::Checkbox("Show Gameobject Axis", &UI::showAxis);
			//if (Application::Inst()->GetDemoName() == "BasicLighting")
			//	ImGui::ColorEdit3("Gameobject Color", UI::objectColor);
			//ImGui::ColorEditMode(ImGuiColorEditMode_UserSelect);
			ImGui::End();
		}

		if (UI::showTestWindow)
		{
			ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
			//ImGui::ShowTestWindow(&UI::showTestWindow);
			ImGui::ShowDemoWindow(&UI::showTestWindow);
			
		}

		if (UI::settingsSelected)
		{
			ImGui::SetNextWindowSize(ImVec2(300, 400));
			ImGui::SetNextWindowPosCenter();
			ImGui::Begin("Settings", NULL, ImGuiWindowFlags_NoCollapse + ImGuiWindowFlags_AlwaysAutoResize);
			ImGui::Text("Credits\n");
			ImGui::TextColored(Green, "Created by BC/DC Games:\n");
			ImGui::TextColored(White, "- Benji Campbell\n");
			ImGui::TextColored(White, "- Daniel Covert\n");
			ImGui::TextColored(White, "- Stephen Lane-Walsh\n");
			ImGui::ShowStyleSelector("UI Theme");

			ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

			if (ImGui::Button("Quit"))
				glfwSetWindowShouldClose(Application::Inst()->GetWindow()->GetGLFWWindow(), GL_TRUE);
			ImGui::End();
		}
	}
}

void UI::RenderUI()
{
    // ImGui functions end here
    ImGui::Render();
	DrawUI(ImGui::GetDrawData());
}

void UI::DrawUI(ImDrawData* draw_data) { ImGui_ImplGlfwGL3_RenderDrawData(draw_data); }
