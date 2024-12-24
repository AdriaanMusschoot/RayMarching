#pragma once

struct SDL_Window;
struct SDL_Renderer;
union SDL_Event;

#include <string>
#include <imgui.h>


namespace GUI
{
	void Initialize(SDL_Window* windowPtr, SDL_Renderer* randererPtr);

	void BeginFrame();
	void EndFrame();
	void Destroy();

	bool ProcessEvent(SDL_Event* eventPtr);

	void LoadSettingsWindow(std::string const& name, ImVec2 const& pos, ImVec2 const& size);
}