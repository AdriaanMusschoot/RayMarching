#pragma once
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix.h"
#include "ColorRGB.h"
#include "MathHelpers.h"

namespace math
{
    float DistanceSquared(geo::Vector3 const& p1, geo::Vector3 const& p2);

    float Distance(geo::Vector3 const& p1, geo::Vector3 const& p2);
}

