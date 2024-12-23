#pragma once

#include "Camera.h"
#include "ColorRGB.h"
#include <vector>

struct SDL_Window;
struct SDL_Surface;

namespace sdf
{
	class Scene;

	class Renderer final
	{
	public:
		Renderer(uint32_t const& width, uint32_t const& height);
		~Renderer();

		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) noexcept = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer& operator=(Renderer&&) noexcept = delete;
		
		bool SaveBufferToImage() const;


		void Render(Scene const& pScene) const;
	private:
		SDL_Window* m_WindowPtr{};

		SDL_Surface* m_SurfacePtr{};
		uint32_t* m_SurfacePixels{};

		uint32_t m_Width{};
		uint32_t m_Height{};
		float m_AspectRatio{};

		std::vector<uint32_t> m_PixelIndices{};

		static ColorRGB Palette(float distance);

		void RenderPixel(Scene const& pScene, float fovValue, glm::vec3 const& cameraOrigin, glm::mat3 const& cameraToWorld, uint32_t pixelIdx) const;
	};
}