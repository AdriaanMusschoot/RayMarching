#pragma once
#include <cassert>

#include "Math.h"
#include "vector"

namespace geo
{
#pragma region GEOMETRY
	struct ISDObject
	{
	public:
		virtual ~ISDObject() = default;

		virtual float GetDistance(Vector3 const& point) = 0;
		virtual void Update(float elapsedSec) {}
		
		static float SmoothMin(float dist1, float dist2, float smoothness);
		unsigned char m_MaterialIndex{ 0 };
	};
	
	struct SDSphere final : public ISDObject
	{
	public:
		SDSphere(Vector3 const& center, float radius);
		float GetDistance(const Vector3& point) override;

		Vector3 m_Origin{};
		float m_Radius{};
	};

	struct SDPlane final : public ISDObject
	{
	public:
		SDPlane(Vector3 const& origin, Vector3 const& normal);
		
		float GetDistance(const Vector3& point) override;

		Vector3 m_Origin{};
		Vector3 m_Normal{};
	};

	struct SDBox final : public ISDObject
	{
	public:
		SDBox(Vector3 const& center, Vector3 const& boxExtent, float roundedValue = 0);
		
		float GetDistance(const Vector3& point) override;

		Vector3 m_Origin{};
		Vector3 m_BoxExtent{};
		float m_RoundedValue{};
	};

	struct SDSmoothSphereBoxPlane final : public ISDObject
	{
	public:
		SDSmoothSphereBoxPlane(SDBox const& box, SDSphere const& sphere, SDPlane const& plane, float smoothness);
		
		float GetDistance(const Vector3& point) override;
		void Update(float elapsedSec) override;

		SDBox m_Box{ Vector3::Zero, Vector3::Zero };
		SDSphere m_Sphere{ Vector3::Zero, 0 };
		SDPlane m_Plane{ Vector3::Zero, Vector3::UnitY };
		SDBox m_Box2{ Vector3::Zero, Vector3{ 0.2, 0.2, 0.2 } };

	private:
		float m_TotalTime{};
		float m_Smoothness{};
	};

	struct SDMandelBulb final : public ISDObject
	{
		float GetDistance(const Vector3& point) override;
		void Update(float elapsedSec) override;

	private:
		float m_TotalTime{};
	};
#pragma endregion
#pragma region LIGHT
	enum class LightType
	{
		Point,
		Directional
	};

	struct Light
	{
		Vector3 origin{};
		Vector3 direction{};
		ColorRGB color{};
		float intensity{};

		LightType type{};
	};
#pragma endregion
#pragma region MISC
	struct Ray
	{
		Vector3 origin{};
		Vector3 direction{};

		float min{ 0.0001f };
		float max{ FLT_MAX };
	};

	struct HitRecord
	{
		Vector3 origin{};
		Vector3 normal{};
		float t = FLT_MAX;

		bool didHit{ false };
		unsigned char materialIndex{ 0 };
	};
#pragma endregion
}