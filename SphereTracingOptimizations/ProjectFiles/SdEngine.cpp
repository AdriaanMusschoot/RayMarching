#include "SdEngine.h"

#include <SDL.h>
#include <SDL_events.h>

#include "GUI.h"
#include "Scene.h"

int sdf::Engine::m_CurrentSceneID{ 0 };
std::vector<const char*> sdf::Engine::m_SceneComplexity{ "Low", "Medium", "High" };

sdf::Engine::Engine(uint32_t const& width, uint32_t const& height)
    : m_Renderer{ width, height }
{
    m_SceneUPtrVec.emplace_back(std::make_unique<SceneEasyComplexity>());
    m_SceneUPtrVec.emplace_back(std::make_unique<SceneMediumComplexity>());
    m_SceneUPtrVec.emplace_back(std::make_unique<SceneHighComplexity>());
}

void sdf::Engine::Run()
{
    while (not ShouldQuit)
    {
		GameTimer& timer = GameTimer::GetInstance();
        timer.Update();

        GUI::BeginFrame();
        
        HandleInput();

        m_SceneUPtrVec[m_CurrentSceneID]->Update(timer.GetElapsed());
        
        m_Renderer.Render(*m_SceneUPtrVec[m_CurrentSceneID]);
    }
}

void sdf::Engine::HandleInput()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (GUI::ProcessEvent(&e))
            continue;

        switch (e.type)
        {
        case SDL_QUIT:
            ShouldQuit = true;
            break;
        default: ;
        }
    }
}
