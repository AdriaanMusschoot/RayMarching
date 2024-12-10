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
		Renderer(uint32_t const& width, uint32_t const& height);
		~Renderer();

		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) noexcept = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer& operator=(Renderer&&) noexcept = delete;
		
		bool SaveBufferToImage() const;

		void Render(const Scene& pScene) const;
		

	private:
		SDL_Window* m_WindowPtr{};

		SDL_Surface* m_SurfacePtr{};
		uint32_t* m_SurfacePixels{};

		uint32_t m_Width{};
		uint32_t m_Height{};
		float m_AspectRatio{};

		std::vector<uint32_t> m_PixelIndices{};

		static ColorRGB Palette(float distance);
		void RenderPixel(const Scene& pScene, uint32_t pixelIdx, float fov, const Camera& camera, const std::vector<Material*>& materialsVec, const
						 std::vector<sdf::Light>& lightsVec) const;
	};
}
