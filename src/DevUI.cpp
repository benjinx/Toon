#include "DevUI.hpp"

#include "Application.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

bool _SettingsShown;

std::vector<std::function<void()>> _OptionsFuncs;

void DevUI::Start()
{
	ImGui::CreateContext();
	ImGui_ImplGlfwGL3_Init(Application::Inst()->GetWindow(), true);
}

void DevUI::Render()
{
	// Call this to start ImGUI
	ImGui_ImplGlfwGL3_NewFrame();

	ImVec4             Red = ImVec4(255.0f, 0.0f, 0.0f, 255.0f);
	ImVec4             Yellow = ImVec4(255.0f, 255.0f, 0.0f, 255.0f);
	ImVec4             Green = ImVec4(0.0f, 255.0f, 0.0f, 255.0f);
	ImVec4             White = ImVec4(255.0f, 255.0f, 255.0f, 255.0f);

	// Menu Bar
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Tools"))
		{
			ImGui::MenuItem("Settings", "ESC", &_SettingsShown, true);
			//ImGui::MenuItem("Console", "F1", &DevUI::consoleSelected, true);
			//ImGui::MenuItem("Options", "F2", &DevUI::optionsSelected, true);
			ImGui::EndMenu();
		}

		ImGui::SameLine((float)Application::Inst()->GetWindowWidth() - 150, 0.0f);
		ImGui::Text("%.2f FPS (%.2f ms)", ImGui::GetIO().Framerate, 1000.0f / ImGui::GetIO().Framerate);
		ImGui::EndMainMenuBar();
	}

	if (_SettingsShown)
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
			glfwSetWindowShouldClose(Application::Inst()->GetWindow(), GL_TRUE);
		ImGui::End();
	}

	ImGui::SetNextWindowSize(ImVec2(400, 200));
	if (ImGui::Begin("Options", &UI::optionsSelected)) {
		for (auto& f : _OptionsFuncs) {
			f();
		}
		ImGui::End();
	}

	ImGui::Render();
	ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
}

void DevUI::HandleEvent(int key)
{
	switch (key)
	{
		case GLFW_KEY_ESCAPE:
		{
			_SettingsShown = !_SettingsShown;
			break;
		}
	}
}

void DevUI::RegisterOptionsFunc(std::function<void()> func)
{
	_OptionsFuncs.push_back(func);
}