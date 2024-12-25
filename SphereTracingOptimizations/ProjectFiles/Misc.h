#pragma once
#include "ColorRGB.h"

namespace sdf
{

	struct HitRecord
	{
		bool DidHit{ false };

		float Distance{};
		int TotalSteps{};
		int StepsUsingAABB{};

		ColorRGB Shade{};
	};

}