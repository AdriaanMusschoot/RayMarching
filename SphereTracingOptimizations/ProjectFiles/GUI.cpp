#include "GUI.h"

#include <imgui.h>
#include <SDL_events.h>
#include <SDL_render.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>

#include "Scene.h"
#include "SdEngine.h"

void GUI::Initialize(SDL_Window* windowPtr, SDL_Renderer* rendererPtr)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForSDLRenderer(windowPtr, rendererPtr);
    ImGui_ImplSDLRenderer2_Init(rendererPtr);
}

void GUI::BeginFrame()
{
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

	LoadSettingsWindow("Settings", ImVec2(0, 0), ImVec2(200, 200));
}

void GUI::EndFrame()
{
    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
}

void GUI::Destroy()
{
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

bool GUI::ProcessEvent(SDL_Event* eventPtr)
{
    return ImGui_ImplSDL2_ProcessEvent(eventPtr);;
}

void GUI::LoadSettingsWindow(std::string const& name, ImVec2 const& pos, ImVec2 const& size)
{
    ImGui::Begin(name.c_str());

    ImGui::SetWindowPos(pos, ImGuiCond_Once);
    ImGui::SetWindowSize(size, ImGuiCond_Once);

    ImGui::Checkbox("Use AABBs", &sdf::Scene::m_UseAABBs);

    ImGui::Combo("Quality", &sdf::Engine::m_CurrentSceneID, sdf::Engine::m_SceneComplexity.data(), sdf::Engine::m_SceneComplexity.size());


    ImGui::End();
}
