#pragma once

#include "Renderer.h"
#include "Timer.h"

namespace sdf
{
    class Scene; 

    class Engine final
    {
    public:
        Engine(uint32_t const& width, uint32_t const& height);
        ~Engine();

        Engine(const Engine&) = delete;
        Engine(Engine&&) noexcept = delete;
        Engine& operator=(const Engine&) = delete;
        Engine& operator=(Engine&&) noexcept = delete;
    
        void Run();
        int& SetCurrentSceneID();
        char const* const* GetSceneComplexities() const;
        int GetSceneComplexityCount() const;

		Renderer const& GetRenderer() const { return m_Renderer; }
		GameTimer& GetTimer() { return m_Timer; }
    private:
        Renderer m_Renderer;
        GameTimer m_Timer;
        std::vector<std::unique_ptr<Scene>> m_SceneUPtrVec;

        int m_CurrentSceneID{ 0 };
        std::vector<const char*> m_SceneComplexity{ "Low", "Medium", "High" };
        
        bool ShouldQuit{ false };
        void HandleInput();
    };

}
