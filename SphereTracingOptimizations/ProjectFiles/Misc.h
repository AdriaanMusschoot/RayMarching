#pragma once
#include "ColorRGB.h"

namespace sdf
{

	struct HitRecord
	{
		float Distance{};
		int TotalSteps{};
		int StepsUsingAABB{};

		ColorRGB Shade{};
		bool DidHit{ false };
	};

}