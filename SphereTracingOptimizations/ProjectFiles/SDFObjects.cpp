#include "SDFObjects.h"

#include <algorithm>
#include <cmath>


float sdf::Object::SmoothMin(float dist1, float dist2, float smoothness)
{
    float h{ std::max(smoothness - std::abs(dist1 - dist2), 0.0f) / smoothness };
    constexpr float smoothFraction{ 1.0f / 6.0f };
    return std::min(dist1, dist2) - h * h * h * smoothness * smoothFraction;
}

sdf::Sphere::Sphere(glm::vec3 const& center, float radius)
    : m_Origin{ center }
    , m_Radius{ radius }
{
}

float sdf::Sphere::GetDistance(const glm::vec3& point)
{
    return glm::length(m_Origin + point) - m_Radius;
}

sdf::Plane::Plane(glm::vec3 const& origin, glm::vec3 const& normal)
    : m_Origin{ origin }
    , m_Normal{ normal }
{
}

float sdf::Plane::GetDistance(const glm::vec3& point)
{
    return glm::dot(m_Normal, point) - m_Origin.y;
}

sdf::Box::Box(glm::vec3 const& center, glm::vec3 const& boxExtent, float roundedValue)
    : m_Origin{ center }
    , m_BoxExtent{ boxExtent }
    , m_RoundedValue{ roundedValue }
{
}

float sdf::Box::GetDistance(const glm::vec3& point)
{
    return glm::length(glm::max(glm::abs(point - m_Origin) - m_BoxExtent, glm::vec3{ 0, 0, 0 }));
}

float sdf::MandelBulb::GetDistance(const glm::vec3& point)
{
    glm::vec3 z = point;
    float dr{ 2.0f };
    float r{ 0.0f };
    float power{ 10.f };
    for (int i = 0; i < 10 ; i++)
    {
        r = glm::length(z);
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
        z = zr*glm::vec3( std::cos(theta) * std::cos(phi), std::cos(theta) * std::sin(phi), std::sin(theta) );
        z += point;
    }
    return 0.5f * std::log(r) * r / dr;
}

float sdf::HexagonalPrism::GetDistance(const glm::vec3& point)
{
    glm::vec3 const k{ -0.8660254f, 0.5f, 0.57735 };

    glm::vec3 absolutePoint{ glm::abs(point) };

    //float subtractionValueX{ 2.0 * std::min(vm::Vector3::Dot(), 0.0f)}
    
    return 0.0f;
}
