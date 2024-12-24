#pragma once

namespace sdf
{

	struct HitRecord
	{
		float Distance{};
		int TotalSteps{};
		int StepsUsingAABB{};
	};

}