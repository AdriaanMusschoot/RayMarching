#pragma once
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix.h"
#include "ColorRGB.h"
#include "MathHelpers.h"

namespace math
{
    float DistanceSquared(dae::Vector3 const& p1, dae::Vector3 const& p2);

    float Distance(dae::Vector3 const& p1, dae::Vector3 const& p2);
}

