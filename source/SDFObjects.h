#pragma once
#include "Vector3.h"

namespace SDF
{
    
    struct Object
    {
    public:
        virtual ~Object() = default;

        virtual float GetDistance(VM::Vector3 const& point) = 0;
        virtual void Update(float elapsedSec) {}
		
        static float SmoothMin(float dist1, float dist2, float smoothness);
        unsigned char m_MaterialIndex{ 0 };
    };
	
    struct Sphere final : public Object
    {
    public:
        Sphere(VM::Vector3 const& center, float radius);
        float GetDistance(const VM::Vector3& point) override;

        VM::Vector3 m_Origin{};
        float m_Radius{};
    };

    struct Plane final : public Object
    {
    public:
        Plane(VM::Vector3 const& origin, VM::Vector3 const& normal);
		
        float GetDistance(const VM::Vector3& point) override;

        VM::Vector3 m_Origin{};
        VM::Vector3 m_Normal{};
    };

    struct Box final : public Object
    {
    public:
        Box(VM::Vector3 const& center, VM::Vector3 const& boxExtent, float roundedValue = 0);
		
        float GetDistance(const VM::Vector3& point) override;

        VM::Vector3 m_Origin{};
        VM::Vector3 m_BoxExtent{};
        float m_RoundedValue{};
    };

    struct MandelBulb final : public Object
    {
        float GetDistance(const VM::Vector3& point) override;
        void Update(float elapsedSec) override;

    private:
        float m_TotalTime{};
    };
    
}