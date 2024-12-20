#pragma once
#include "Vector3.h"

namespace sdf
{
    
    class Object
    {
    public:
        virtual ~Object() = default;

        virtual float GetDistance(vm::Vector3 const& point) = 0;
        virtual void Update(float elapsedSec) {}
		
        static float SmoothMin(float dist1, float dist2, float smoothness);
    private:
        unsigned char m_MaterialIndex{ 0 };
    };
	
    class Sphere final : public Object
    {
    public:
        Sphere(vm::Vector3 const& center, float radius);
        float GetDistance(const vm::Vector3& point) override;
    private:
        vm::Vector3 m_Origin{};
        float m_Radius{};
    };

    class Plane final : public Object
    {
    public:
        Plane(vm::Vector3 const& origin, vm::Vector3 const& normal);
		
        float GetDistance(const vm::Vector3& point) override;
    private:
        vm::Vector3 m_Origin{};
        vm::Vector3 m_Normal{};
    };

    class Box final : public Object
    {
    public:
        Box(vm::Vector3 const& center, vm::Vector3 const& boxExtent, float roundedValue = 0);
		
        float GetDistance(const vm::Vector3& point) override;
    private:
        vm::Vector3 m_Origin{};
        vm::Vector3 m_BoxExtent{};
        float m_RoundedValue{};
    };

    class MandelBulb final : public Object
    {
    public:
        float GetDistance(const vm::Vector3& point) override;
        void Update(float elapsedSec) override;

    private:
        float m_TotalTime{};
    };
    
}