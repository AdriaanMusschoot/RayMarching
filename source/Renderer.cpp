#include "SDL.h"
#include "SDL_surface.h"

//Project includes
#include "Renderer.h"
#include "Math.h"
#include "Material.h"
#include "Scene.h"
#include "Utils.h"
#include "Execution"

#define MULTITHREADING
//#define REFLECTIONS 2

using namespace dae;

Renderer::Renderer(SDL_Window* pWindow) :
	m_pWindow(pWindow),
	m_pBuffer(SDL_GetWindowSurface(pWindow))
{
	//Initialize
	SDL_GetWindowSize(pWindow, &m_Width, &m_Height);
	m_AspectRatio = float(m_Width) / float(m_Height);
	m_pBufferPixels = static_cast<uint32_t*>(m_pBuffer->pixels);
	const int nrOfPixels{ m_Width * m_Height };

	m_PixelIndices.reserve(nrOfPixels);
	for (int pixelIdx{}; pixelIdx < nrOfPixels; ++pixelIdx) m_PixelIndices.emplace_back(pixelIdx);

}

void dae::Renderer::Render(Scene* pScene) const
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

void dae::Renderer::ToggleShadows()
{
	m_ShadowsEnabled = !m_ShadowsEnabled;
}

void dae::Renderer::RenderPixel(Scene* pScene, uint32_t pixelIdx, float fov, const Camera& camera, const std::vector<Material*>& materialsVec, const std::vector<Light>& lightsVec) const
{
	const uint32_t px{ pixelIdx % m_Width };
	const uint32_t py{ pixelIdx / m_Width };
	
	const float rx{ px + 0.5f };
	const float ry{ py + 0.5f };
	const float cx{ (2 * (rx / float(m_Width)) - 1) * m_AspectRatio * fov };
	const float cy{ (1 - (2 * (ry / float(m_Height)))) * fov };

	const Ray cameraToWorldRay{ camera.origin, camera.cameraToWorld.TransformVector(Vector3{ cx, cy, 1 }.Normalized()) };
	ColorRGB finalColor{};

#ifdef REFLECTIONS
	float reflectionValue{ 1.f };
	for (int idx{}; idx < REFLECTIONS; ++idx)
	{
		HitRecord closestHit{};
		pScene->GetClosestHit(cameraToWorldRay, closestHit);

		if (closestHit.didHit)
		{
			Vector3 lightToHitDirection;
			Ray lightToHitRay;

			const Vector3 hitPointOffset{ closestHit.origin + closestHit.normal * 0.001f };

			for (int idx{}; idx < lightsVec.size(); ++idx)
			{
				lightToHitDirection = { hitPointOffset - lightsVec[idx].origin };
				const float lightToHitDistance{ lightToHitDirection.Magnitude() };
				lightToHitRay = { lightsVec[idx].origin, lightToHitDirection / lightToHitDistance };
				lightToHitRay.max = lightToHitDistance;

				if (m_ShadowsEnabled && pScene->DoesHit(lightToHitRay)) continue;

				switch (m_CurrentLightingMode)
				{
				case LightingMode::ObservedArea:
				{
					float observedAreaValue{ Vector3::Dot(-lightToHitRay.direction, closestHit.normal) };
					if (observedAreaValue > 0) finalColor += ColorRGB{ 1, 1, 1 } *observedAreaValue;
				}
				break;
				case LightingMode::Radiance:
				{
					finalColor += LightUtils::GetRadiance(pScene->GetLights()[idx], hitPointOffset);
				}
				break;
				case LightingMode::BRDF:
				{
					finalColor += materialsVec[closestHit.materialIndex]->Shade(closestHit, -lightToHitRay.direction, -cameraToWorldRay.direction.Normalized());
				}
				break;
				case LightingMode::Combined:
				{
					float observedAreaValue{ Vector3::Dot(-lightToHitRay.direction, closestHit.normal) };
					if (observedAreaValue < 0) observedAreaValue = 0;
					finalColor += reflectionValue * LightUtils::GetRadiance(pScene->GetLights()[idx], closestHit.origin) *
						observedAreaValue *
						materialsVec[closestHit.materialIndex]->Shade(closestHit, -lightToHitRay.direction, -cameraToWorldRay.direction.Normalized());
				}
				break;
				}
			}
		}
		//Update Color in Buffer
		cameraToWorldRay.origin = closestHit.origin;
		cameraToWorldRay.direction = Vector3::Reflect(cameraToWorldRay.direction, closestHit.normal);
		reflectionValue -= reflectionValue / REFLECTIONS;
	}
#else
	float t =  pScene->GetClosestHit(cameraToWorldRay);
	finalColor.r = t / 6.f;
#endif // REFLECTIONS
	finalColor.MaxToOne();
	
	m_pBufferPixels[int(px) + int(py) * m_Width] = SDL_MapRGB(m_pBuffer->format,
		static_cast<uint8_t>(finalColor.r * 255),
		static_cast<uint8_t>(finalColor.g * 255),
		static_cast<uint8_t>(finalColor.b * 255));
}

