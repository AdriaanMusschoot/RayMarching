#pragma once
#include <cstdint>

#include "Renderer.h"
#include "Scene.h"

namespace sdf
{

    class SdEngine final
    {
    public:
        SdEngine(uint32_t const& width, uint32_t const& height);
        ~SdEngine() = default;

        SdEngine(const SdEngine&) = delete;
        SdEngine(SdEngine&&) noexcept = delete;
        SdEngine& operator=(const SdEngine&) = delete;
        SdEngine& operator=(SdEngine&&) noexcept = delete;
    
        void Run();
    private:
        Renderer Renderer;
        GameTimer Timer;
        RayMarchingScene Scene;

        bool ShouldQuit{ false };
        void HandleInput();
    };

}
