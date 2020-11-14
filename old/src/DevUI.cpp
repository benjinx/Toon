#include "DevUI.hpp"

#include <App.hpp>

#include <vector>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl.h>
#include <imgui/imgui_impl_opengl3.h>

std::vector<std::function<void()>> _OptionsFuncs;

bool DevUI::consoleSelected = false;
bool DevUI::optionsSelected = false;
bool DevUI::settingsSelected = false;
bool DevUI::showMainMenuBar = true;
bool DevUI::showDemoWindow = false;
bool DevUI::showAxis = false;
float DevUI::objectColor[3] = { 1.0f, 1.0f, 1.0f };

void DevUI::Start()
{
    // Setup ImGui
    ImGui::CreateContext();

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL(App::Inst()->GetWindow()->GetSDLWindow(), App::Inst()->GetWindow()->GetGLContext());
    ImGui_ImplOpenGL3_Init("#version 150");
}

void DevUI::Render()
{
    // Call this to start ImGUI
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(App::Inst()->GetWindow()->GetSDLWindow());
    ImGui::NewFrame();
    
    ImVec4 Red = ImVec4(255.0f, 0.0f, 0.0f, 255.0f);
    ImVec4 Yellow = ImVec4(255.0f, 255.0f, 0.0f, 255.0f);
    ImVec4 Green = ImVec4(0.0f, 255.0f, 0.0f, 255.0f);
    ImVec4 White = ImVec4(255.0f, 255.0f, 255.0f, 255.0f);

    // Menu Bar
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Tools"))
        {
            ImGui::MenuItem("Settings", "ESC", &DevUI::settingsSelected, true);
            ImGui::MenuItem("Console", "F1", &DevUI::consoleSelected, true);
            ImGui::MenuItem("Options", "F2", &DevUI::optionsSelected, true);
            ImGui::MenuItem("Test Window", "F11", &DevUI::showDemoWindow, true);
            ImGui::EndMenu();
        }

        ImGui::SameLine((float)App::Inst()->GetWindow()->GetWidth() - 150, 0.0f);
        ImGui::Text("%.2f FPS (%.2f ms)", ImGui::GetIO().Framerate, 1000.0f / ImGui::GetIO().Framerate);
        ImGui::EndMainMenuBar();
    }

    if (DevUI::settingsSelected)
    {
        ImGui::SetNextWindowSize(ImVec2(300, 400), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowPos(ImVec2((float)((App::Inst()->GetWindow()->GetWidth() / 2) - 150), (float)((App::Inst()->GetWindow()->GetHeight() / 2) - 100)));
        ImGui::Begin("Settings", NULL, ImGuiWindowFlags_NoCollapse + ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::Text("Credits\n");
        ImGui::TextColored(Green, "Created by BC/DC Games:\n");
        ImGui::TextColored(White, "- Benji Campbell\n");
        ImGui::TextColored(White, "- Stephen Lane-Walsh\n");
        ImGui::TextColored(White, "- Daniel Covert\n");
        ImGui::ShowStyleSelector("UI Theme");

        ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

        if (ImGui::Button("Quit"))
        {
            App::Inst()->Quit();
        }
        ImGui::End();
    }

    if (DevUI::consoleSelected)
    {
        ImGui::SetNextWindowSize(ImVec2(400, 200));
        ImGui::Begin("Console", &DevUI::consoleSelected);
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

    
    if (DevUI::optionsSelected)
    {
        ImGui::SetNextWindowSize(ImVec2(400, 200));
        if (ImGui::Begin("Options", &DevUI::optionsSelected, ImGuiWindowFlags_NoCollapse)) {
            for (auto& f : _OptionsFuncs) {
                f();
            }
            ImGui::End();
        }
    }
    
    if (DevUI::showDemoWindow)
    {
        ImGui::SetNextWindowPos(ImVec2(650, 20));// , ImGuiSetCond_FirstUseEver);
        ImGui::ShowDemoWindow(&DevUI::showDemoWindow);
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void DevUI::RegisterOptionsFunc(std::function<void()> func)
{
    _OptionsFuncs.push_back(func);
}