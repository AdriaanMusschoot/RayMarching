#include <SDL.h>
#include <vector>
#include "Scene.h"
#include "ColorRGB.h"

namespace sdf
{
    class Renderer
    {
    public:
        Renderer(uint32_t const& width, uint32_t const& height);
        ~Renderer();

        void Render(Scene const& pScene) const;
        bool SaveBufferToImage() const;

    private:
        void RenderPixel(Scene const& pScene, float fovValue, glm::vec3 const& cameraOrigin, glm::mat3 const& cameraToWorld, uint32_t pixelIdx) const;
        static ColorRGB Palette(float distance);

        uint32_t m_Width;
        uint32_t m_Height;
        float m_AspectRatio;
        SDL_Window* m_WindowPtr;
        SDL_Renderer* m_RendererPtr;
        SDL_Texture* m_TexturePtr;
        std::vector<uint32_t> m_PixelIndices;
        mutable std::vector<uint32_t> m_Pixels{};
    };
}