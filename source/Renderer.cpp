#include "SDL.h"
#include "SDL_surface.h"

//Project includes
#include "Renderer.h"
#include "Math.h"
#include "Material.h"
#include "Scene.h"
#include "Execution"

#define MULTITHREADING

using namespace geo;

Renderer::Renderer(SDL_Window* pWindow) :
	m_pWindow(pWindow),
	m_pBuffer(SDL_GetWindowSurface(pWindow))
{
	//Initialize
	SDL_GetWindowSize(pWindow, &m_Width, &m_Height);
	m_AspectRatio = static_cast<float>(m_Width) / static_cast<float>(m_Height);
	m_pBufferPixels = static_cast<uint32_t*>(m_pBuffer->pixels);
	const int nrOfPixels{ m_Width * m_Height };

	m_PixelIndices.reserve(nrOfPixels);
	for (int pixelIdx{}; pixelIdx < nrOfPixels; ++pixelIdx) m_PixelIndices.emplace_back(pixelIdx);

}

void geo::Renderer::Render(Scene* pScene) const
{
	Camera& camera = pScene->GetCamera();
	const std::vector<Material*>& materialsVec{ pScene->GetMaterials() };
	const std::vector<Light>& lights = pScene->GetLights();
	
#ifdef MULTITHREADING
	std::for_each(std::execution::par_unseq, m_PixelIndices.begin(), m_PixelIndices.end(), [&](int pixelIdx)
		{
			RenderPixel(pScene, pixelIdx, camera.fovValue, camera, materialsVec, lights);
		});
#else
	const int nrOfPixels{ m_Width * m_Height };

	for (int pixelIdx{}; pixelIdx < nrOfPixels; ++pixelIdx)
	{
		RenderPixel(pScene, pixelIdx, camera.fovValue, camera, materialsVec, lights);
	}

#endif

	
	//@END
	//Update SDL Surface
	SDL_UpdateWindowSurface(m_pWindow);

}

bool Renderer::SaveBufferToImage() const
{
	return SDL_SaveBMP(m_pBuffer, "RayTracing_Buffer.bmp");
}

void Renderer::CycleLightingMode()
{
	if (static_cast<int>(m_CurrentLightingMode) == static_cast<int>(LightingMode::NrOfEnums) - 1)
	{
		m_CurrentLightingMode = LightingMode::ObservedArea;
	}
	else 
	{
		m_CurrentLightingMode = static_cast<LightingMode>(static_cast<int>(m_CurrentLightingMode) + 1);
	}
}

void geo::Renderer::ToggleShadows()
{
	m_ShadowsEnabled = !m_ShadowsEnabled;
}

void geo::Renderer::RenderPixel(Scene* pScene, uint32_t pixelIdx, float fov, const Camera& camera, const std::vector<Material*>& materialsVec, const std::vector<Light>& lightsVec) const
{
	const uint32_t px{ pixelIdx % m_Width };
	const uint32_t py{ pixelIdx / m_Width };
	
	const float rx{ px + 0.5f };
	const float ry{ py + 0.5f };
	const float cx{ (2 * (rx / float(m_Width)) - 1) * m_AspectRatio * fov };
	const float cy{ (1 - (2 * (ry / float(m_Height)))) * fov };

	const Ray cameraToWorldRay{ camera.origin, camera.cameraToWorld.TransformVector(Vector3{ cx, cy, 1 }.Normalized()) };
	ColorRGB finalColor{};

	auto[distance, iteration] = pScene->GetClosestHit(cameraToWorldRay);
	finalColor = Palette(distance * 0.04f + iteration * 0.005f);
	finalColor.MaxToOne();
	
	m_pBufferPixels[int(px) + int(py) * m_Width] = SDL_MapRGB(m_pBuffer->format,
		static_cast<uint8_t>(finalColor.r * 255),
		static_cast<uint8_t>(finalColor.g * 255),
		static_cast<uint8_t>(finalColor.b * 255));
}

ColorRGB Renderer::Palette(float distance)
{
	Vector3 const a = Vector3(0.5, 0.5, 0.5);
	Vector3 const b = Vector3(0.5, 0.5, 0.5);
	Vector3 const c = Vector3(1.0, 1.0, 1.0);
	Vector3 const d = Vector3(0.263,0.416,0.557);

	Vector3 const e = c * distance + d;
	Vector3 const cosE{ std::cos(e.x), std::cos(e.y),  std::cos(e.z) };
	Vector3 const t{ a + cosE * 6.28318f * b };
	
	return ColorRGB{ t.x, t.y, t.z };
}

