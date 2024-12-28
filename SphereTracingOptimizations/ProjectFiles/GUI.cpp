#include "GUI.h"

#include <imgui.h>
#include <SDL_events.h>
#include <SDL_render.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>

#include "Scene.h"
#include "SdEngine.h"

#define IMGUI_DISABLE_DEBUG_TOOLS

void GUI::Initialize(SDL_Window* windowPtr, SDL_Renderer* rendererPtr)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForSDLRenderer(windowPtr, rendererPtr);
    ImGui_ImplSDLRenderer2_Init(rendererPtr);
}

void GUI::BeginFrame(sdf::Engine& engine)
{
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

	static ImVec2 const settingsDimensions{ 150, 300 };
	static ImVec2 const settingsPosition{ 0, 0 };
	static ImVec2 const statsDimensions{ 150, 300 };
	static ImVec2 const statsPosition{ engine.GetRenderer().GetWindowDimensions().x - statsDimensions.x, 0 };

	LoadSettingsWindow(engine, "Settings", settingsPosition, settingsDimensions);
	LoadStatsWindow(engine, "Statistics", statsPosition, statsDimensions);
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

void GUI::LoadSettingsWindow(sdf::Engine& engine, std::string const& name, ImVec2 const& pos, ImVec2 const& size)
{
    ImGui::Begin(name.c_str());
    ImGui::SetWindowPos(pos, ImGuiCond_Once);
    ImGui::SetWindowSize(size, ImGuiCond_Once);

    ImGui::Checkbox("Use Early Out", &sdf::Scene::m_UseEarlyOut);
    ImGui::Checkbox("Use BVH", &sdf::Scene::m_UseBVH);
	ImGui::InputInt("BVH Steps", &sdf::Scene::m_BVHSteps);

	ImGui::Text("Scene complexity: ");
    ImGui::Combo("|", &engine.SetCurrentSceneID(), engine.GetSceneComplexities(), engine.GetSceneComplexityCount());

    if (ImGui::Button("Start Tracking Statistics"))
	{
		engine.GetStatTracker().StartTracking();
    }

    ImGui::End();
}

void GUI::LoadStatsWindow(sdf::Engine& engine, std::string const& name, ImVec2 const& pos, ImVec2 const& size)
{
	ImGui::Begin(name.c_str());
    ImGui::SetWindowPos(pos, ImGuiCond_Once);
    ImGui::SetWindowSize(size, ImGuiCond_Once);

    ImGui::Value("FPS", ImGui::GetIO().Framerate);
    
    sdf::StatTracker& statTracker{ engine.GetStatTracker() };

    ImGui::Text("Rays hit: %d", statTracker.GetNrCollisions());
    ImGui::Text("Rays missed: %d", statTracker.GetNrMisses());

    ImGui::End();
}
