//External includes
#include "vld.h"
#include "SDL.h"
#include "SDL_surface.h"
#undef main

//Standard includes
#include <iostream>

//Project includes
#include "Timer.h"
#include "Renderer.h"
#include "Scene.h"

int main(int argc, char* args[])
{
	SDL_Init(SDL_INIT_VIDEO);

	constexpr uint32_t width = 600;
	constexpr uint32_t height = 600;

	SDL_Window* pWindow = SDL_CreateWindow
	(
		"Raymarcher, Adriaan Musschoot",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		width, height, 0
	);

	if (!pWindow)
		return 1;

	VM::Timer Timer{};
	VM::Renderer Renderer{ pWindow };
	VM::RayMarchingScene Scene{};

	Timer.Start();

	// Start Benchmark
	Timer.StartBenchmark();

	float printTimer = 0.f;
	bool isLooping = true;
	bool takeScreenshot = false;
	constexpr int lightMoveSpeed{ 10 };
	float totalFrames{};
	while (isLooping)
	{
		//--------- Get input events ---------
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			const Uint8* pStates = SDL_GetKeyboardState(nullptr);
			
			switch (e.type)
			{
			case SDL_QUIT:
				isLooping = false;
				break;
			case SDL_KEYDOWN:
				if(e.key.keysym.scancode == SDL_SCANCODE_X)
					takeScreenshot = true;
				if (e.key.keysym.scancode == SDL_SCANCODE_F2)
					Renderer.ToggleShadows();
				if (e.key.keysym.scancode == SDL_SCANCODE_F3)
					Renderer.CycleLightingMode();
				if (e.key.keysym.scancode == SDL_SCANCODE_F6)
					Timer.StartBenchmark();

				break;
			default: ;
			}
		}
	
		
		Scene.Update(Timer.GetElapsed());

		Renderer.Render(Scene);

		Timer.Update();
		printTimer += Timer.GetElapsed();
		if (printTimer >= 1.f)
		{
			printTimer = 0.f;
			std::cout << "current FPS: " << Timer.GetdFPS() << ", \n";
			totalFrames += Timer.GetdFPS();
			std::cout << "average FPS: " << totalFrames / Timer.GetTotalTime() << '\n';
		}

		if (takeScreenshot)
		{
			if (!Renderer.SaveBufferToImage())
				std::cout << "Screenshot saved!" << '\n';
			else
				std::cout << "Something went wrong. Screenshot not saved!" << '\n';
			takeScreenshot = false;
		}
	}
	Timer.Stop();

	SDL_DestroyWindow(pWindow);
	SDL_Quit();
}