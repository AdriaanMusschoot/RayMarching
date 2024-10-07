#pragma once
#include <cassert>

#include "Math.h"
#include "vector"

namespace geo
{
#pragma region GEOMETRY
	class ISDObject
	{
	public:
		virtual ~ISDObject() = default;

		virtual float GetDistance(Vector3 const& point) = 0;

	protected:
		unsigned char m_MaterialIndex{ 0 };
	};
	
	class SDSphere final : public ISDObject
	{
	public:
		SDSphere(Vector3 const& center, float radius);
		float GetDistance(const Vector3& point) override;

	private:
		Vector3 m_Origin{};
		float m_Radius{};
	};

	class SDPlane final : public ISDObject
	{
	public:
		SDPlane(Vector3 const& origin, Vector3 const& normal);
		float GetDistance(const Vector3& point) override;

	private:
		Vector3 m_Origin{};
		Vector3 m_Normal{};
	};

	class SDBox final : public ISDObject
	{
	public:
		SDBox(Vector3 const& center, Vector3 const& boxExtent);
		float GetDistance(const Vector3& point) override;

	private:
		Vector3 m_Origin{};
		Vector3 m_BoxExtent{};
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