#include "SDL.h"
#include "SDL_surface.h"

//Project includes
#include "Renderer.h"
#include "Math.h"
#include "Material.h"
#include "Scene.h"
#include "Execution"

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

void sdf::Renderer::Render(const Scene& pScene) const
{
	const Camera& camera = pScene.GetCamera();
	const std::vector<Material*>& materialsVec{ pScene.GetMaterials() };
	const std::vector<Light>& lights = pScene.GetLights();
	
	std::for_each(std::execution::par_unseq, m_PixelIndices.begin(), m_PixelIndices.end(), [&](int pixelIdx)
		{
			RenderPixel(pScene, pixelIdx, camera.fovValue, camera, materialsVec, lights);
		});

	
	//@END
	//Update SDL Surface
	SDL_UpdateWindowSurface(m_WindowPtr);

}

bool sdf::Renderer::SaveBufferToImage() const
{
	return SDL_SaveBMP(m_SurfacePtr, "RayTracing_Buffer.bmp");
}

void sdf::Renderer::RenderPixel(const Scene& pScene, uint32_t pixelIdx, float fov, const Camera& camera, const std::vector<Material*>& materialsVec, const std::vector<Light>& lightsVec) const
{
	const uint32_t px{ pixelIdx % m_Width };
	const uint32_t py{ pixelIdx / m_Width };
	
	const float rx{ px + 0.5f };
	const float ry{ py + 0.5f };
	const float cx{ (2 * (rx / static_cast<float>(m_Width)) - 1) * m_AspectRatio * fov };
	const float cy{ (1 - (2 * (ry / static_cast<float>(m_Height)))) * fov };

	const sdf::Ray cameraToWorldRay{ camera.origin, camera.cameraToWorld.TransformVector(vm::Vector3{ cx, cy, 1 }.Normalized()) };
	ColorRGB finalColor{};

	auto[distance, iteration] = pScene.GetClosestHit(cameraToWorldRay, 0.01f, 100.f, 100);
	finalColor = ColorRGB{ 1.f, 1.f, 1.f } * (distance * 0.06f + iteration * 0.01);
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

sdf::ColorRGB sdf::Renderer::Palette(float distance)
{
	vm::Vector3 const a{ 0.5, 0.5, 0.5 };
	vm::Vector3 const b{ 0.5, 0.5, 0.5 };
	vm::Vector3 const c{ 1.0, 1.0, 1.0 };
	vm::Vector3 const d{ 0.263f,0.416f,0.457f };

	vm::Vector3 const e{ c * distance + d };
	vm::Vector3 const cosE{ std::cos(e.x), std::cos(e.y),  std::cos(e.z) };
	vm::Vector3 const t{ a + cosE * 6.28318f * b };
	
	return ColorRGB{ t.x, t.y, t.z };
}

