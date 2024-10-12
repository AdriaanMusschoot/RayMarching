#include "DataTypes.h"

#include <iostream>
#include <ostream>
#include <valarray>


float geo::ISDObject::SmoothMin(float dist1, float dist2, float smoothness)
{
    float h{ std::max(smoothness - std::abs(dist1 - dist2), 0.0f) / smoothness };
    constexpr float smoothFraction{ 1.0f / 6.0f };
    return std::min(dist1, dist2) - h * h * h * smoothness * smoothFraction;
}

geo::SDSphere::SDSphere(Vector3 const& center, float radius)
    : m_Origin{ center }
    , m_Radius{ radius }
{
}

float geo::SDSphere::GetDistance(const Vector3& point)
{
    return math::Distance(m_Origin, point) - m_Radius;
}

geo::SDPlane::SDPlane(Vector3 const& origin, Vector3 const& normal)
    : m_Origin{ origin }
    , m_Normal{ normal }
{
}

float geo::SDPlane::GetDistance(const Vector3& point)
{
    return Vector3::Dot(m_Normal, point) - m_Origin.y;
}

geo::SDBox::SDBox(Vector3 const& center, Vector3 const& boxExtent, float roundedValue)
    : m_Origin{ center }
    , m_BoxExtent{ boxExtent }
    , m_RoundedValue{ roundedValue }
{
}

float geo::SDBox::GetDistance(const Vector3& point)
{
    return Vector3::Max(Vector3::Abs(point - m_Origin) - m_BoxExtent, Vector3::Zero).Magnitude();
}

geo::SDSmoothSphereBoxPlane::SDSmoothSphereBoxPlane(SDBox const& box, SDSphere const& sphere, SDPlane const& plane,
    float smoothness)
    : m_Box{ box }
    , m_Sphere{ sphere }
    , m_Plane{ plane }
    , m_Smoothness{ smoothness }
{
}

float geo::SDSmoothSphereBoxPlane::GetDistance(const Vector3& point)
{
    Vector3 const OffsetPoint{ point.x, point.y, point.z + m_TotalTime };
    Vector3 const SpaceRepeatedPoint{ Vector3::Frac(OffsetPoint) - Vector3{ 0.5f, 0.5f, 0.5f } };
    return SmoothMin(m_Sphere.GetDistance(point), m_Box.GetDistance(SpaceRepeatedPoint), m_Smoothness);
}

void geo::SDSmoothSphereBoxPlane::Update(float elapsedSec)
{
    m_TotalTime += elapsedSec;
    m_Sphere.m_Origin.x = std::sin(m_TotalTime);
    m_Sphere.m_Origin.y = std::cos(m_TotalTime);
}
