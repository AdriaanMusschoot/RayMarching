#include <SDL.h>
#include <vector>

#include <glm/glm.hpp>

#include "StatTracker.h"

namespace sdf
{
    struct HitRecord;
    struct ColorRGB;

    class Scene;
	class GameTimer;

	class Renderer final
    {
    public:
        Renderer(uint32_t const& width, uint32_t const& height);
        ~Renderer();

        void RenderFrame(Scene const& pScene, GameTimer& gameTimer) const;
        bool SaveBufferToImage() const;

		glm::ivec2 GetWindowDimensions() const;

		StatTracker& GetStatTracker() const { return m_StatTracker; }
    private:
        void CalculateHitRecords(Scene const& pScene, float fovValue, glm::vec3 const& cameraOrigin, glm::mat3 const& cameraToWorld, uint32_t pixelIdx) const;
        static ColorRGB Palette(float distance);

        uint32_t m_Width;
        uint32_t m_Height;
        float m_AspectRatio;
        SDL_Window* m_WindowPtr{ nullptr };
        SDL_Renderer* m_RendererPtr{ nullptr };
        SDL_Texture* m_TexturePtr{ nullptr };
        std::vector<uint32_t> m_PixelIndices{};
        mutable std::vector<uint32_t> m_PixelVec{};
        SDL_PixelFormat* m_PixelFormatPtr{ nullptr };
        mutable std::vector<HitRecord> m_HitRecordVec;

		mutable StatTracker m_StatTracker;
    };
}