#include "SdEngine.h"

#include <SDL.h>
#include <SDL_events.h>

sdf::SdEngine::SdEngine(uint32_t const& width, uint32_t const& height)
    : Renderer{ width, height }
    , Timer{}
    , Scene{}
{
}

void sdf::SdEngine::Run()
{
    while (not ShouldQuit)
    {
        Timer.Update();
        
        HandleInput();

        Scene.Update(Timer.GetElapsed());
        
        Renderer.Render(Scene);
    }
}

void sdf::SdEngine::HandleInput()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        const Uint8* pStates = SDL_GetKeyboardState(nullptr);
			
        switch (e.type)
        {
        case SDL_QUIT:
            ShouldQuit = true;
            break;
        case SDL_KEYDOWN:
            if (e.key.keysym.scancode == SDL_SCANCODE_F6)
                Timer.StartBenchmark(1000);
            break;
        default: ;
        }
    }
}
