#pragma once
#include <cassert>
#include <valarray>

#include "Math.h"

namespace sdf
{
	namespace BRDF
	{
		static ColorRGB Lambert(float kd, const ColorRGB& cd)
		{
			return { cd * kd / std::numbers::pi_v<float> };
		}

		static ColorRGB Lambert(const ColorRGB& kd, const ColorRGB& cd)
		{
			return { cd * kd / std::numbers::pi_v<float> };
		}

		static ColorRGB Phong(float ks, float exp, const vm::Vector3& l, const vm::Vector3& v, const vm::Vector3& n)
		{
			const vm::Vector3 reflectionVector{ vm::Vector3::Reflect(l, n)};
			float cosAngle{ vm::Vector3::Dot(reflectionVector, v) };
			if (cosAngle < 0) cosAngle = 0;
			const float phongSpecularReflection{ ks * std::pow(cosAngle, exp) };
			return { phongSpecularReflection, phongSpecularReflection, phongSpecularReflection };
		}

		static ColorRGB FresnelFunction_Schlick(const vm::Vector3& h, const vm::Vector3& v, const ColorRGB& f0)
		{
			float angle{ vm::Vector3::Dot(h, v) };
			if (angle < 0) angle = 0;
			return f0 + (ColorRGB{ 1, 1, 1 } - f0) * ((1 - angle) * (1 - angle) * (1 - angle) * (1 - angle) * (1 - angle));
		}

		static float NormalDistribution_GGX(const vm::Vector3& n, const vm::Vector3& h, float roughness)
		{
			float roughnessSquared{					roughness * roughness };
			float normalDistribution{	vm::Vector3::Dot(n, h) * vm::Vector3::Dot(n, h)
										* ((roughnessSquared * roughnessSquared) - 1.f)
										+ 1.f };
			return roughnessSquared * roughnessSquared / (std::numbers::pi_v<float> * normalDistribution * normalDistribution);
		}

		static float GeometryFunction_SchlickGGX(const vm::Vector3& n, const vm::Vector3& v, float roughness)
		{
			float roughnessSquared{ roughness * roughness };
			float kDirect{ (roughnessSquared + 1)  * (roughnessSquared + 1) / 8.0f };

			float angle{ vm::Vector3::Dot(n, v) };
			if (angle < 0) angle = 0;
			return angle / (angle * (1 - kDirect) + kDirect);
		}

		static float GeometryFunction_Smith(const vm::Vector3& n, const vm::Vector3& v, const vm::Vector3& l, float roughness)
		{
			return GeometryFunction_SchlickGGX(n, v, roughness) * GeometryFunction_SchlickGGX(n, l, roughness);
		}

	}
}