#pragma once

#include <cstdint>
#include "DataTypes.h"
#include "Camera.h"
#include "Material.h"

struct SDL_Window;
struct SDL_Surface;

namespace VM
{
	class Scene;

	class Renderer final
	{
	public:
		Renderer(SDL_Window* pWindow);
		~Renderer() = default;

		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) noexcept = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer& operator=(Renderer&&) noexcept = delete;
		bool SaveBufferToImage() const;

		void CycleLightingMode();
		void ToggleShadows();
		void Render(Scene* pScene) const;
		void RenderPixel(Scene* pScene, uint32_t pixelIdx, float fov, const Camera & camera, const std::vector<Material*> & materialsVec, const std::vector<SDF::Light> & lightsVec) const;

	private:
		SDL_Window* m_pWindow{};

		SDL_Surface* m_pBuffer{};
		uint32_t* m_pBufferPixels{};

		int m_Width{};
		int m_Height{};
		float m_AspectRatio{};

		enum class LightingMode
		{
			ObservedArea,
			Radiance, 
			BRDF,
			Combined,

			NrOfEnums
		};

		LightingMode m_CurrentLightingMode{ LightingMode::Combined };
		bool m_ShadowsEnabled{ false };

		std::vector<uint32_t> m_PixelIndices{};

		static VM::ColorRGB Palette(float distance);
	};
}
