#include "SdEngine.h"

#include <SDL.h>
#include <SDL_events.h>

#include "Scene.h"

sdf::Engine::Engine(uint32_t const& width, uint32_t const& height)
    : m_Renderer{ width, height }
    , m_Timer{}
{
    m_SceneUPtrVec.emplace_back(std::make_unique<SceneEasyComplexity>());
    m_SceneUPtrVec.emplace_back(std::make_unique<SceneMediumComplexity>());
    m_SceneUPtrVec.emplace_back(std::make_unique<SceneMaxComplexity>());
}

void sdf::Engine::Run()
{
    while (not ShouldQuit)
    {
        m_Timer.Update();
        
        HandleInput();

        m_SceneUPtrVec[m_CurrentSceneID]->Update(m_Timer.GetElapsed());
        
        m_Renderer.Render(*m_SceneUPtrVec[m_CurrentSceneID]);
    }
}

void sdf::Engine::HandleInput()
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
                m_Timer.StartBenchmark(1000);
            else if (e.key.keysym.scancode == SDL_SCANCODE_F7)
                m_CurrentSceneID = (m_CurrentSceneID + 1) % (m_SceneUPtrVec.size());
            else if (e.key.keysym.scancode == SDL_SCANCODE_F8)
				m_SceneUPtrVec[m_CurrentSceneID]->ToggleUseAABBs();
            else if (e.key.keysym.scancode == SDL_SCANCODE_F9)
				m_SceneUPtrVec[m_CurrentSceneID]->ToggleVisibilityAABBs();
        default: ;
        }
    }
}
