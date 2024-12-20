#pragma once
#include <cassert>
#include <valarray>

#include "Math.h"

namespace VM
{
	namespace BRDF
	{
		static ColorRGB Lambert(float kd, const VM::ColorRGB& cd)
		{
			return { cd * kd / std::numbers::pi_v<float> };
		}

		static ColorRGB Lambert(const VM::ColorRGB& kd, const VM::ColorRGB& cd)
		{
			return { cd * kd / std::numbers::pi_v<float> };
		}

		static VM::ColorRGB Phong(float ks, float exp, const Vector3& l, const Vector3& v, const Vector3& n)
		{
			const Vector3 reflectionVector{ Vector3::Reflect(l, n)};
			float cosAngle{ Vector3::Dot(reflectionVector, v) };
			if (cosAngle < 0) cosAngle = 0;
			const float phongSpecularReflection{ ks * std::pow(cosAngle, exp) };
			return { phongSpecularReflection, phongSpecularReflection, phongSpecularReflection };
		}

		static VM::ColorRGB FresnelFunction_Schlick(const Vector3& h, const Vector3& v, const VM::ColorRGB& f0)
		{
			float angle{ Vector3::Dot(h, v) };
			if (angle < 0) angle = 0;
			return f0 + (VM::ColorRGB{ 1, 1, 1 } - f0) * ((1 - angle) * (1 - angle) * (1 - angle) * (1 - angle) * (1 - angle));
		}

		static float NormalDistribution_GGX(const Vector3& n, const Vector3& h, float roughness)
		{
			float roughnessSquared{					roughness * roughness };
			float normalDistribution{	Vector3::Dot(n, h) * Vector3::Dot(n, h)
										* ((roughnessSquared * roughnessSquared) - 1.f)
										+ 1.f };
			return roughnessSquared * roughnessSquared / (std::numbers::pi_v<float> * normalDistribution * normalDistribution);
		}

		static float GeometryFunction_SchlickGGX(const Vector3& n, const Vector3& v, float roughness)
		{
			float roughnessSquared{ roughness * roughness };
			float kDirect{ (roughnessSquared + 1)  * (roughnessSquared + 1) / 8.0f };

			float angle{ Vector3::Dot(n, v) };
			if (angle < 0) angle = 0;
			return angle / (angle * (1 - kDirect) + kDirect);
		}

		static float GeometryFunction_Smith(const Vector3& n, const Vector3& v, const Vector3& l, float roughness)
		{
			return GeometryFunction_SchlickGGX(n, v, roughness) * GeometryFunction_SchlickGGX(n, l, roughness);
		}

	}
}