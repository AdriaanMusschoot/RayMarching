#include "DataTypes.h"

#include <valarray>

float geo::ISDObject::SmoothMin(float dist1, float dist2, float smoothness)
{
    float h{ std::max(smoothness - std::abs(dist1 - dist2), 0.0f) / smoothness };
    return std::min(dist1, dist2) - h * h * h * smoothness * (1.0 / 6.0);
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

geo::SDSphereBox::SDSphereBox(SDBox const& box, SDSphere const& sphere)
    : m_Box{ box }
    , m_Sphere{ sphere }
{
}

float geo::SDSphereBox::GetDistance(const Vector3& point)
{
    return SmoothMin(m_Sphere.GetDistance(point), m_Box.GetDistance(point), 1);
}
