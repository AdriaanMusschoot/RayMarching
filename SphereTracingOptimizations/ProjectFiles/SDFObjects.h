#pragma once
#include "glm/glm.hpp"

namespace sdf
{
    
    class Object
    {
    public:
        virtual ~Object() = default;

        virtual float GetDistance(glm::vec3 const& point) = 0;
		
        static float SmoothMin(float dist1, float dist2, float smoothness);
    private:
    };
	
    class Sphere final : public Object
    {
    public:
        Sphere(glm::vec3 const& center, float radius);
        float GetDistance(const glm::vec3& point) override;
    private:
        glm::vec3 m_Origin{};
        float m_Radius{};
    };

    class Plane final : public Object
    {
    public:
        Plane(glm::vec3 const& origin, glm::vec3 const& normal);
		
        float GetDistance(const glm::vec3& point) override;
    private:
        glm::vec3 m_Origin{};
        glm::vec3 m_Normal{};
    };

    class Box final : public Object
    {
    public:
        Box(glm::vec3 const& center, glm::vec3 const& boxExtent, float roundedValue = 0);
		
        float GetDistance(const glm::vec3& point) override;
    private:
        glm::vec3 m_Origin{};
        glm::vec3 m_BoxExtent{};
        float m_RoundedValue{};
    };

    class MandelBulb final : public Object
    {
    public:
        float GetDistance(const glm::vec3& point) override;
    private:
    };

    class HexagonalPrism final : public Object
    {
    public:
        float GetDistance(const glm::vec3& point) override;
    private:
    };
    
}