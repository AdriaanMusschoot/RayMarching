#include "SDL.h"
#include "SDL_surface.h"

//Project includes
#include "Renderer.h"
#include "Math.h"
#include "Material.h"
#include "Scene.h"
#include "Execution"

#define MULTITHREADING

using namespace VM;

Renderer::Renderer(uint32_t const& width, uint32_t const& height)
	: m_Width{ width }
	, m_Height{ height }
{
	m_WindowPtr = SDL_CreateWindow
	(
		"Raymarcher, Adriaan Musschoot",
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

Renderer::~Renderer()
{
	SDL_DestroyWindow(m_WindowPtr);
	SDL_Quit();
}

void VM::Renderer::Render(const Scene& pScene) const
{
	const Camera& camera = pScene.GetCamera();
	const std::vector<Material*>& materialsVec{ pScene.GetMaterials() };
	const std::vector<sdf::Light>& lights = pScene.GetLights();
	
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
	SDL_UpdateWindowSurface(m_WindowPtr);

}

bool Renderer::SaveBufferToImage() const
{
	return SDL_SaveBMP(m_SurfacePtr, "RayTracing_Buffer.bmp");
}

void VM::Renderer::RenderPixel(const Scene& pScene, uint32_t pixelIdx, float fov, const Camera& camera, const std::vector<Material*>& materialsVec, const std::vector<sdf::Light>& lightsVec) const
{
	const uint32_t px{ pixelIdx % m_Width };
	const uint32_t py{ pixelIdx / m_Width };
	
	const float rx{ px + 0.5f };
	const float ry{ py + 0.5f };
	const float cx{ (2 * (rx / static_cast<float>(m_Width)) - 1) * m_AspectRatio * fov };
	const float cy{ (1 - (2 * (ry / static_cast<float>(m_Height)))) * fov };

	const sdf::Ray cameraToWorldRay{ camera.origin, camera.cameraToWorld.TransformVector(Vector3{ cx, cy, 1 }.Normalized()) };
	VM::ColorRGB finalColor{};

	auto[distance, iteration] = pScene.GetClosestHit(cameraToWorldRay);
	finalColor = VM::ColorRGB{ 1.f, 1.f, 1.f } * (distance * 0.06f + iteration * 0.01);
	finalColor.MaxToOne();
	
	m_SurfacePixels[static_cast<int>(px) + static_cast<int>(py) * m_Width] =
		SDL_MapRGB
		(
			m_SurfacePtr->format,
           static_cast<uint8_t>(finalColor.r * 255),
           static_cast<uint8_t>(finalColor.g * 255),
           static_cast<uint8_t>(finalColor.b * 255)
        );
}

VM::ColorRGB Renderer::Palette(float distance)
{
	Vector3 const a{ 0.5, 0.5, 0.5 };
	Vector3 const b{ 0.5, 0.5, 0.5 };
	Vector3 const c{ 1.0, 1.0, 1.0 };
	Vector3 const d{ 0.263f,0.416f,0.457f };

	Vector3 const e{ c * distance + d };
	Vector3 const cosE{ std::cos(e.x), std::cos(e.y),  std::cos(e.z) };
	Vector3 const t{ a + cosE * 6.28318f * b };
	
	return ColorRGB{ t.x, t.y, t.z };
}

