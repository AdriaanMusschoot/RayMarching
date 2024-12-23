#include "SDL.h"
#include "SDL_surface.h"

//Project includes
#include "Renderer.h"
#include "glm/glm.hpp"
#include "Scene.h"
#include "Execution"
#include <cassert>

sdf::Renderer::Renderer(uint32_t const& width, uint32_t const& height)
	: m_Width{ width }
	, m_Height{ height }
{
	m_WindowPtr = SDL_CreateWindow
	(
		"SphereTracer, Adriaan Musschoot",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		width, height, 0
	);

	assert(m_WindowPtr, "Window creation failed");
	
	m_SurfacePtr = SDL_GetWindowSurface(m_WindowPtr);

	m_AspectRatio = static_cast<float>(m_Width) / static_cast<float>(m_Height);

	m_SurfacePixels = static_cast<uint32_t*>(m_SurfacePtr->pixels);

	const uint32_t nrOfPixels{ m_Width * m_Height };

	m_PixelIndices.reserve(nrOfPixels);
	for (uint32_t pixelIdx{}; pixelIdx < nrOfPixels; ++pixelIdx)
	{
		m_PixelIndices.emplace_back(pixelIdx);
	}
}

sdf::Renderer::~Renderer()
{
	SDL_DestroyWindow(m_WindowPtr);
	SDL_Quit();
}

void sdf::Renderer::Render(Scene const& pScene) const
{
	Camera const& camera{ pScene.GetCamera() };

	float const& fovValue{ camera.fovValue };
	glm::mat3 const& cameraToWorld{ camera.cameraToWorld };
	glm::vec3 const& origin{ camera.origin };

	std::for_each(std::execution::par_unseq, m_PixelIndices.begin(), m_PixelIndices.end(), [&](uint32_t pixelIdx)
		{
			RenderPixel(pScene, fovValue, origin, cameraToWorld, pixelIdx);
		});

	SDL_UpdateWindowSurface(m_WindowPtr);
}

bool sdf::Renderer::SaveBufferToImage() const
{
	return SDL_SaveBMP(m_SurfacePtr, "RayTracing_Buffer.bmp");
}

sdf::ColorRGB sdf::Renderer::Palette(float distance)
{
	glm::vec3 const a{ 0.5, 0.5, 0.5 };
	glm::vec3 const b{ 0.5, 0.5, 0.5 };
	glm::vec3 const c{ 1.0, 1.0, 1.0 };
	glm::vec3 const d{ 0.263f,0.416f,0.457f };
	
	glm::vec3 const e{ c * distance + d };
	glm::vec3 const cosE{ std::cos(e.x), std::cos(e.y),  std::cos(e.z) };
	glm::vec3 const t{ a + cosE * 6.28318f * b };
	
	return ColorRGB{ t.x, t.y, t.z };
}

void sdf::Renderer::RenderPixel(Scene const& pScene, float fovValue, glm::vec3 const& cameraOrigin, glm::mat3 const& cameraToWorld, uint32_t pixelIdx) const
{
	uint32_t const px{ pixelIdx % m_Width };
	uint32_t const py{ pixelIdx / m_Width };

	float const rx{ px + 0.5f };
	float const ry{ py + 0.5f };
	float const cx{ (2 * (rx / m_Width) - 1) * m_AspectRatio * fovValue };
	float const cy{ (1 - (2 * (ry / m_Height))) * fovValue };

	glm::vec3 const cameraDirection{ glm::normalize(cameraToWorld * glm::vec3{ cx, cy, 1.f }) };

	auto const [distance, iteration] = pScene.GetClosestHit(cameraOrigin, cameraDirection, 0.001f, 100, 10000);
	ColorRGB finalColor{ ColorRGB{ 1.f, 1.f, 1.f } * (distance * 0.05f + iteration * 0.018f) };
	finalColor.MaxToOne();	

	m_SurfacePixels[pixelIdx] =
		SDL_MapRGB
		(
			m_SurfacePtr->format,
			static_cast<uint8_t>(finalColor.r * 255),
			static_cast<uint8_t>(finalColor.g * 255),
			static_cast<uint8_t>(finalColor.b * 255)
		);
}