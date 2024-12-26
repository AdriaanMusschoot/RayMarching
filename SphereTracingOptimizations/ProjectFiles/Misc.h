#pragma once
#include "ColorRGB.h"

namespace sdf
{

	struct HitRecord
	{
		bool DidHit{ false };

		float Distance{};
		int TotalSteps{};
		int StepsUsingEarlyOut{};

		ColorRGB Shade{ 1.f, 1.f, 1.f };
	};

}