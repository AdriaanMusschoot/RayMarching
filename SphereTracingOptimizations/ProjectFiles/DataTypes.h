#pragma once
#include "ColorRGB.h"
#include "vector"
#include "Vector3.h"

namespace sdf
{
#pragma region GEOMETRY
	
#pragma endregion
#pragma region LIGHT
	enum class LightType
	{
		Point,
		Directional
	};

	struct Light
	{
		VM::Vector3 origin{};
		VM::Vector3 direction{};
		VM::ColorRGB color{};
		float intensity{};

		LightType type{};
	};
#pragma endregion
#pragma region MISC
	struct Ray
	{
		VM::Vector3 origin{};
		VM::Vector3 direction{};

		float min{ 0.0001f };
		float max{ FLT_MAX };
	};

	struct HitRecord
	{
		VM::Vector3 origin{};
		VM::Vector3 normal{};
		float t = FLT_MAX;

		bool didHit{ false };
		unsigned char materialIndex{ 0 };
	};
#pragma endregion
}
