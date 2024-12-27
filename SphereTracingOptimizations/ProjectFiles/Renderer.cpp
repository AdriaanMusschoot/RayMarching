#include "SDL.h"
#include "SDL_surface.h"

//Project includes
#include "Renderer.h"
#include "glm/glm.hpp"
#include "Scene.h"
#include "Execution"
#include <cassert>
#include "GUI.h"
#include "Misc.h"
#include "Camera.h"

sdf::Renderer::Renderer(uint32_t const& width, uint32_t const& height)
	: m_Width{ width }
	, m_Height{ height }
{
	m_WindowPtr = SDL_CreateWindow("SphereTracer, Adriaan Musschoot", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,	width, height, SDL_WINDOW_SHOWN);

	if (m_WindowPtr == nullptr)
	{
		throw(std::runtime_error("Window creation failed"));
	}

	m_RendererPtr = SDL_CreateRenderer(m_WindowPtr, -1, SDL_RENDERER_ACCELERATED);

	if (m_RendererPtr == nullptr)
	{
		throw(std::runtime_error("Renderer creation failed"));
	}

	m_TexturePtr = SDL_CreateTexture(m_RendererPtr, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);

	if (m_TexturePtr == nullptr)
	{
		throw(std::runtime_error("Texture creation failed"));
	}
	
	m_AspectRatio = static_cast<float>(m_Width) / static_cast<float>(m_Height);

	const uint32_t nrOfPixels{ m_Width * m_Height };

	m_PixelIndices.reserve(nrOfPixels);
	for (uint32_t pixelIdx{}; pixelIdx < nrOfPixels; ++pixelIdx)
	{
		m_PixelIndices.emplace_back(pixelIdx);
	}

	m_PixelVec.resize(nrOfPixels);
	m_HitRecordVec.resize(nrOfPixels);

	m_PixelFormatPtr = SDL_AllocFormat(SDL_PIXELFORMAT_ARGB8888);

	GUI::Initialize(m_WindowPtr, m_RendererPtr);
}

sdf::Renderer::~Renderer()
{
	GUI::Destroy();
	SDL_DestroyTexture(m_TexturePtr);
	SDL_DestroyRenderer(m_RendererPtr);
	SDL_DestroyWindow(m_WindowPtr);
	SDL_Quit();
}

void sdf::Renderer::Render(Scene const& pScene) const
{
	Camera const& camera{ pScene.GetCamera() };

	float const& fovValue{ camera.fovValue };
	glm::mat3 const& cameraToWorld{ camera.cameraToWorld };
	glm::vec3 const& origin{ camera.origin };

	std::fill(std::execution::par_unseq, m_PixelVec.begin(), m_PixelVec.end(), SDL_MapRGB(m_PixelFormatPtr, 255, 255, 255));

	std::for_each(std::execution::par_unseq, m_PixelIndices.begin(), m_PixelIndices.end(), [&](uint32_t pixelIdx)
		{
			CalculateHitRecords(pScene, fovValue, origin, cameraToWorld, pixelIdx);
		});
	
	std::for_each(std::execution::par_unseq, m_PixelIndices.begin(), m_PixelIndices.end(), [&](uint32_t pixelIdx)
		{
			if (HitRecord const& hitRecord{ m_HitRecordVec[pixelIdx] }; 
				hitRecord.DidHit)
			{
				m_PixelVec[pixelIdx] = SDL_MapRGB(m_PixelFormatPtr, hitRecord.Shade.r * 255, hitRecord.Shade.g * 255, hitRecord.Shade.b * 255);
			}
		});
	int old{ Scene::m_BVHSteps };
	Scene::m_BVHSteps = 100; 

	std::for_each(std::execution::par_unseq, m_PixelIndices.begin(), m_PixelIndices.end(), [&](uint32_t pixelIdx)
		{
			CalculateHitRecords(pScene, fovValue, origin, cameraToWorld, pixelIdx);
		});

	std::for_each(std::execution::par_unseq, m_PixelIndices.begin(), m_PixelIndices.end(), [&](uint32_t pixelIdx)
		{
			if (HitRecord const& hitRecord{ m_HitRecordVec[pixelIdx] };
				hitRecord.DidHit)
			{
				m_PixelVec[pixelIdx] = SDL_MapRGB(m_PixelFormatPtr, hitRecord.Shade.r * 255, hitRecord.Shade.g * 255, hitRecord.Shade.b * 255);
			}
		});

	Scene::m_BVHSteps = old;

	SDL_UpdateTexture(m_TexturePtr, nullptr, m_PixelVec.data(), m_Width * sizeof(uint32_t));
	SDL_RenderClear(m_RendererPtr);
	SDL_RenderCopy(m_RendererPtr, m_TexturePtr, nullptr, nullptr);

	GUI::EndFrame();

	SDL_RenderPresent(m_RendererPtr);
}

bool sdf::Renderer::SaveBufferToImage() const
{
	SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, m_Width, m_Height, 32, SDL_PIXELFORMAT_ARGB8888);
	SDL_RenderReadPixels(m_RendererPtr, nullptr, SDL_PIXELFORMAT_ARGB8888, surface->pixels, surface->pitch);
	bool result = SDL_SaveBMP(surface, "RayTracing_Buffer.bmp") == 0;
	SDL_FreeSurface(surface);
	return result;
}

int sdf::Renderer::GetNrCollisions() const
{
	return std::count_if(std::execution::par_unseq, m_HitRecordVec.begin(), m_HitRecordVec.end(), [](HitRecord const& hitRecord)
		{ 
			return hitRecord.DidHit; 
		});
}

int sdf::Renderer::GetNrMisses() const
{
	return m_HitRecordVec.size() - GetNrCollisions();
}

glm::ivec2 sdf::Renderer::GetWindowDimensions() const
{
	return glm::ivec2(m_Width, m_Height);
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

void sdf::Renderer::CalculateHitRecords(Scene const& pScene, float fovValue, glm::vec3 const& cameraOrigin, glm::mat3 const& cameraToWorld, uint32_t pixelIdx) const
{
	uint32_t const px{ pixelIdx % m_Width };
	uint32_t const py{ pixelIdx / m_Width };

	float const rx{ px + 0.5f };
	float const ry{ py + 0.5f };
	float const cx{ (2 * (rx / m_Width) - 1) * m_AspectRatio * fovValue };
	float const cy{ (1 - (2 * (ry / m_Height))) * fovValue };

	glm::vec3 const cameraDirection{ glm::normalize(cameraToWorld * glm::vec3{ cx, cy, 1.f }) };

	m_HitRecordVec[pixelIdx] = pScene.GetClosestHit(cameraOrigin, cameraDirection, 0.001f, 100, 10000);
}