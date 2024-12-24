#pragma once
#include <cstdint>

#include "Renderer.h"
#include "Scene.h"
#include "Timer.h"

namespace sdf
{

    class Engine final
    {
    public:
        Engine(uint32_t const& width, uint32_t const& height);
        ~Engine() = default;

        Engine(const Engine&) = delete;
        Engine(Engine&&) noexcept = delete;
        Engine& operator=(const Engine&) = delete;
        Engine& operator=(Engine&&) noexcept = delete;
    
        void Run();

        static int m_CurrentSceneID;
        static std::vector<const char*> m_SceneComplexity;
    private:
        Renderer m_Renderer;
        std::vector<std::unique_ptr<Scene>> m_SceneUPtrVec{};

        bool ShouldQuit{ false };
        void HandleInput();
    };

}
