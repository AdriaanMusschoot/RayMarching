#pragma once

struct SDL_Window;
struct SDL_Renderer;
union SDL_Event;

namespace GUI
{
	void Initialize(SDL_Window* windowPtr, SDL_Renderer* randererPtr);

	void BeginFrame();
	void EndFrame();
	void Destroy();

	bool ProcessEvent(SDL_Event* eventPtr);
}