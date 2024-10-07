#include "Math.h"
#include "cmath"

float math::DistanceSquared(geo::Vector3 const& p1, geo::Vector3 const& p2)
{
    float const dx { p2.x - p1.x };
    float const dy { p2.y - p1.y };
    float const dz { p2.z - p1.z };

    // Return the squared distance
    return dx * dx + dy * dy + dz * dz;
}

float math::Distance(geo::Vector3 const& p1, geo::Vector3 const& p2)
{
    return sqrt(math::DistanceSquared(p1, p2));
}
