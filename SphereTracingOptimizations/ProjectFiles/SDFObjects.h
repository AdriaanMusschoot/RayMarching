#pragma once
#include "glm/glm.hpp"
#include <array>
#include <vector>

namespace sdf
{
    
    class Object
    {
    public:
        Object(glm::vec3 const& origin);
        virtual ~Object() = default;

        virtual float GetDistance(glm::vec3 const& point) = 0;
		
        float FurthestSurfaceConcentricCircles(float initialRadius = 100);

        glm::vec3 const& Origin() const;
    private:
        glm::vec3 m_Origin{ 0.f, 0.f, 0.f };
    };
	
    class Sphere final : public Object
    {
    public:
        Sphere(float radius, glm::vec3 const& origin = glm::vec3{ 0.f, 0.f, 0.f });
        virtual ~Sphere() = default;

        float GetDistance(const glm::vec3& point) override;
    private:
        float m_Radius{};
    };

    class BoxFrame final : public Object
    {
    public:
        BoxFrame(glm::vec3 const& boxExtent = glm::vec3{ 0.3f, 0.3f, 0.3f }, float roundedValue = 0.02f, glm::vec3 const& origin = glm::vec3{ 1.f, 0.f, 0.f });
        virtual ~BoxFrame() = default;
		
        float GetDistance(const glm::vec3& point) override;
    private:
        glm::vec3 m_BoxExtent{};
        float m_RoundedValue{};
    };

    class HexagonalPrism final : public Object
    {
    public:
        HexagonalPrism(float depth = 0.2f, float radius = 0.3f, glm::vec3 const& origin = glm::vec3{0.f, 0.f, 0.f});
        virtual ~HexagonalPrism() = default;

        float GetDistance(const glm::vec3& point) override;
    private:
        float m_Depth{ 0.2f };
        float m_Radius{ 0.3f };
    };

    class Link final : public Object
    {
    public:
        Link(float height = 0.2f, float innerRadius = 0.2f, float tubeRadius = 0.07f, glm::vec3 const& origin = glm::vec3{ 0.f, 0.f, 0.f });
        virtual ~Link() = default;

        float GetDistance(const glm::vec3& point) override;
    private:
        float m_HeightEmptySpace{ 0.2f };
        float m_InnerRadius{ 0.2f };
        float m_RadiusTube{ 0.05f };
    };

    class Octahedron final : public Object
    {
    public:
        Octahedron(float size = 0.3f, glm::vec3 const& origin = glm::vec3{ 0.f, 0.f, 0.f });
        virtual ~Octahedron() = default;

        float GetDistance(glm::vec3 const& point) override;
    private:
        float m_Size{ 0.3f };
    };

    class CappedTorus final : public Object
    {
    public:
        CappedTorus(float innerRadius = 0.3f, float tubeRadius = 0.06f, float openingAngle = -1.2f, glm::vec3 const& origin = glm::vec3{ 0.f, 0.f, 0.f });
        virtual ~CappedTorus() = default;

        float GetDistance(glm::vec3 const& point) override;
    private:
        float m_InnerRadius{ 0.3f };
        float m_TubeRadius{ 0.05f };
        float m_OpeningAngle{ -1.0f };
    };

    class Pyramid final : public Object
    {
    public:
        Pyramid(glm::vec3 const& origin = glm::vec3{ 0.f, 0.f, 0.f });
        virtual ~Pyramid() = default;

        float GetDistance(glm::vec3 const& point) override;
    private:
        float m_Height{ 1.f };
        float m_BaseWidth{ 1.f };
    };
    
    class MandelBulb final : public Object
    {
    public:
        MandelBulb(glm::vec3 const& origin = glm::vec3{ 0.f, 0.f, 0.f });
        virtual ~MandelBulb() = default;

        float GetDistance(const glm::vec3& point) override;
    private:
    };

    static float SmoothMin(float dist1, float dist2, float smoothness);
    constexpr int PointCountHorizontal{ 360 };
    constexpr int PointCountVertical{ 360 };
    static std::vector<glm::vec3> GenerateSpherePoints(glm::vec3 const& origin, float radius);
}