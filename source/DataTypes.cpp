#include "DataTypes.h"

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

geo::SDBox::SDBox(Vector3 const& center, Vector3 const& boxExtent)
    : m_Origin{ center }
    , m_BoxExtent{ boxExtent }
{
}

float geo::SDBox::GetDistance(const Vector3& point)
{
    return Vector3::Max(Vector3::Abs(point - m_Origin) - m_BoxExtent, Vector3::Zero).Magnitude();
}
