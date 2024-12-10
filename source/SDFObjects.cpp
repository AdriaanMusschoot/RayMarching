#include "SDFObjects.h"

#include <algorithm>
#include <cmath>


float sdf::Object::SmoothMin(float dist1, float dist2, float smoothness)
{
    float h{ std::max(smoothness - std::abs(dist1 - dist2), 0.0f) / smoothness };
    constexpr float smoothFraction{ 1.0f / 6.0f };
    return std::min(dist1, dist2) - h * h * h * smoothness * smoothFraction;
}

sdf::Sphere::Sphere(VM::Vector3 const& center, float radius)
    : m_Origin{ center }
, m_Radius{ radius }
{
}

float sdf::Sphere::GetDistance(const VM::Vector3& point)
{
    return (m_Origin + point).Magnitude() - m_Radius;
}

sdf::Plane::Plane(VM::Vector3 const& origin, VM::Vector3 const& normal)
    : m_Origin{ origin }
    , m_Normal{ normal }
{
}

float sdf::Plane::GetDistance(const VM::Vector3& point)
{
    return VM::Vector3::Dot(m_Normal, point) - m_Origin.y;
}

sdf::Box::Box(VM::Vector3 const& center, VM::Vector3 const& boxExtent, float roundedValue)
    : m_Origin{ center }
    , m_BoxExtent{ boxExtent }
    , m_RoundedValue{ roundedValue }
{
}

float sdf::Box::GetDistance(const VM::Vector3& point)
{
    return VM::Vector3::Max(VM::Vector3::Abs(point - m_Origin) - m_BoxExtent, VM::Vector3::Zero).Magnitude();
}

float sdf::MandelBulb::GetDistance(const VM::Vector3& point)
{
    VM::Vector3 z = point;
    float dr{ 2.0f };
    float r{ 0.0f };
    float power{ 10.f };
    for (int i = 0; i < 10 ; i++)
    {
        r = z.Magnitude();
        if (r > 2.f) break;
    
        // convert to polar coordinates
        float theta = asin( z.z/r );
        float phi = atan2( z.y,z.x );
        dr = 4.0f * std::pow(r, power - 1.0f) * dr + 1.0f;
        
        // scale and rotate the point
        float zr = std::pow( r,power);
        theta = theta * power;
        phi = phi * power;
        
        // convert back to cartesian coordinates
        z = zr*VM::Vector3( std::cos(theta) * std::cos(phi), std::cos(theta) * std::sin(phi), std::sin(theta) );
        z += point;
    }
    return 0.5f * std::log(r) * r / dr;
}

void sdf::MandelBulb::Update(float elapsedSec)
{
    m_TotalTime += elapsedSec;
}
