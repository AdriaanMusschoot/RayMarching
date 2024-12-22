#include "SDFObjects.h"

#include <algorithm>
#include <execution>
#include <iostream>
#include <optional>
#include <future>

sdf::Object::Object(glm::vec3 const& origin)
    : m_Origin{ origin }
{
}

float sdf::Object::EarlyOutTest(glm::vec3 const& point)
{
    return glm::length(point) - m_EarlyOutRadius;
}

void sdf::Object::FurthestSurfaceConcentricCircles(float initialRadius)
{
    float maxDistance{ -std::numeric_limits<float>::max() };
    std::vector<glm::vec3> points{ GenerateSpherePoints(glm::vec3{ 0.f, 0.f, 0.f }, initialRadius) };

    std::optional<glm::vec3> closestPoint{};
    std::vector<float> localMaxDistances(std::thread::hardware_concurrency(), -std::numeric_limits<float>::infinity());
    
    std::for_each(std::execution::par_unseq, points.begin(), points.end(),
        [&](const glm::vec3& point)
        {
             float distance{ GetDistanceUnoptimized(point) };
             if (distance < 0.001f)
             {
                 closestPoint = point;
                 return;
             }
             size_t threadId{ std::hash<std::thread::id>{}(std::this_thread::get_id()) % localMaxDistances.size() };
             if (float& localMaxDistance{ localMaxDistances[threadId] }; distance > localMaxDistance)
             {
				 localMaxDistance = distance;
             }
        });

    if (closestPoint.has_value())
    {
		m_EarlyOutRadius = glm::length(closestPoint.value());
        return;
    }

    maxDistance = *std::max_element(localMaxDistances.begin(), localMaxDistances.end());

    return FurthestSurfaceConcentricCircles(maxDistance);
}

glm::vec3 const& sdf::Object::Origin() const
{
    return m_Origin;
}

sdf::Sphere::Sphere(float radius, glm::vec3 const& origin)
    : Object(origin)
    , m_Radius{ radius }
{
}

float sdf::Sphere::GetDistance(const glm::vec3& point)
{
    return GetDistanceUnoptimized(point);
}

float sdf::Sphere::GetDistanceUnoptimized(glm::vec3 const& point)
{
    return glm::length(Origin() - point) - m_Radius;
}

sdf::BoxFrame::BoxFrame(glm::vec3 const& boxExtent, float roundedValue, glm::vec3 const& origin)
    : Object(origin)
    , m_BoxExtent{ boxExtent }
    , m_RoundedValue{ roundedValue }
{
    FurthestSurfaceConcentricCircles();
}

float sdf::BoxFrame::GetDistance(const glm::vec3& point)
{
    //return glm::length(glm::max(glm::abs(movedPoint) - m_BoxExtent, glm::vec3{ 0, 0, 0 }));
    if (float const earlyOutDistance{ EarlyOutTest(point) }; earlyOutDistance >= 0.1f)
    {
        return earlyOutDistance;
    }

	return GetDistanceUnoptimized(point);
}

float sdf::BoxFrame::GetDistanceUnoptimized(glm::vec3 const& point)
{
    glm::vec3 const p{ abs(point) - m_BoxExtent };
    glm::vec3 const q{ abs(p + m_RoundedValue) - m_RoundedValue };
    return glm::min(glm::min(
        glm::length(glm::max(glm::vec3(p.x, q.y, q.z), 0.0f)) + glm::min(glm::max(p.x, glm::max(q.y, q.z)), 0.0f),
        glm::length(glm::max(glm::vec3(q.x, p.y, q.z), 0.0f)) + glm::min(glm::max(q.x, glm::max(p.y, q.z)), 0.0f)),
        glm::length(glm::max(glm::vec3(q.x, q.y, p.z), 0.0f)) + glm::min(glm::max(q.x, glm::max(q.y, p.z)), 0.0f));;
}

sdf::HexagonalPrism::HexagonalPrism(float depth, float radius, glm::vec3 const& origin)
    : Object(origin)
    , m_Depth{ depth }
    , m_Radius{ radius }
{
}

float sdf::HexagonalPrism::GetDistance(const glm::vec3& point)
{
    return GetDistanceUnoptimized(point);
}

float sdf::HexagonalPrism::GetDistanceUnoptimized(glm::vec3 const& point)
{
    static glm::vec3 const k{ -0.8660254f, 0.5f, 0.57735f };
    static glm::vec2 const kxy{ k };

    glm::vec3 movedPoint{ point - Origin() };

    movedPoint = glm::abs(movedPoint);
    glm::vec2 const pxy{ movedPoint };

    glm::vec2 const subtVal{ 2.0f * glm::min(glm::dot(kxy, pxy), 0.0f) * kxy };

    movedPoint.x -= subtVal.x;
    movedPoint.y -= subtVal.y;

    glm::vec2 d
    {
        glm::length((glm::vec2{ movedPoint } - glm::vec2{glm::clamp(movedPoint.x, -k.z * m_Radius, k.z * m_Radius), m_Radius}) * glm::sign(movedPoint.y - m_Radius)),
        movedPoint.z - m_Depth
    };

    return glm::min(glm::max(d.x, d.y), 0.f) + glm::length(glm::max(d, 0.0f));
}

sdf::Link::Link(float height, float innerRadius, float tubeRadius, glm::vec3 const& origin)
    : Object(origin)
    , m_HeightEmptySpace{ height }
    , m_InnerRadius{ innerRadius }
    , m_RadiusTube{ tubeRadius }
{
}

float sdf::Link::GetDistance(const glm::vec3& point)
{
	return GetDistanceUnoptimized(point);
}

float sdf::Link::GetDistanceUnoptimized(glm::vec3 const& point)
{
    glm::vec3 const movedPoint{ point - Origin() };

    glm::vec3 const q{ movedPoint.x, glm::max(glm::abs(movedPoint.y) - m_HeightEmptySpace, 0.0f), movedPoint.z };
    glm::vec2 const qxy{ q };
    return glm::length(glm::vec2{ glm::length(qxy) - m_InnerRadius, q.z }) - m_RadiusTube;
}

sdf::Octahedron::Octahedron(float size, glm::vec3 const& origin)
    : Object(origin)
    , m_Size{ size }
{
}

float sdf::Octahedron::GetDistance(glm::vec3 const& point)
{
	return GetDistanceUnoptimized(point);
}

float sdf::Octahedron::GetDistanceUnoptimized(glm::vec3 const& point)
{
    glm::vec3 p{ glm::abs(point) };
    float m{ p.x + p.y + p.z - m_Size };

    glm::vec3 q;
    if (3.0f * p.x < m)
    {
        q = p;
    }
    else if (3.0f * p.y < m)
    {
        q.x = p.y;
        q.y = p.z;
        q.z = p.x;
    }
    else if (3.0f * p.z < m)
    {
        q.x = p.z;
        q.y = p.x;
        q.z = p.y;
    }
    else
    {
        return m * 0.57735027f;
    }

    float k{ glm::clamp(0.5f * (q.z - q.y + m_Size), 0.0f, m_Size) };
    return glm::length(glm::vec3{ q.x, q.y - m_Size + k, q.z - k });
}

sdf::MandelBulb::MandelBulb(glm::vec3 const& origin)
    : Object(origin)
{
}

float sdf::MandelBulb::GetDistance(const glm::vec3& point)
{
	return GetDistanceUnoptimized(point);
}

float sdf::MandelBulb::GetDistanceUnoptimized(glm::vec3 const& point)
{
    glm::vec3 const p{ point };

    int iterations = 4;
    glm::vec3 w = p;
    float m = glm::dot(w, w);
    glm::vec4 trap{ abs(w), m };
    float dz = 1;

    for (int i{ 0 }; i < iterations; i++)
    {
        float m2 = m * m;
        float m4 = m2 * m2;
        dz = 8 * glm::sqrt(m4 * m2 * m) * dz + 1.0;

        float x = w.x; float x2 = x * x; float x4 = x2 * x2;
        float y = w.y; float y2 = y * y; float y4 = y2 * y2;
        float z = w.z; float z2 = z * z; float z4 = z2 * z2;

        float k3 = x2 + z2;
        float k2 = glm::inversesqrt(k3 * k3 * k3 * k3 * k3 * k3 * k3);
        float k1 = x4 + y4 + z4 - 6.0 * y2 * z2 - 6.0 * x2 * y2 + 2.0 * z2 * x2;
        float k4 = x2 - y2 + z2;
        w.x = p.x + 64.0 * x * y * z * (x2 - z2) * k4 * (x4 - 6.0 * x2 * z2 + z4) * k1 * k2;
        w.y = p.y + -16.0 * y2 * k3 * k4 * k4 + k1 * k1;
        w.z = p.z + -8.0 * y * k4 * (x4 * x4 - 28.0 * x4 * x2 * z2 + 70.0 * x4 * z4 - 28.0 * x2 * z2 * z4
            + z4 * z4) * k1 * k2;

        trap = glm::min(trap, glm::vec4{ abs(w), m });

        m = dot(w, w);
        if (m > 256)
            break;
    }
    return 0.25f * glm::log(m) * sqrt(m) / dz;
}

sdf::CappedTorus::CappedTorus(float innerRadius, float tubeRadius, float openingAngle, glm::vec3 const& origin)
    : Object(origin)
    , m_InnerRadius{ innerRadius }
    , m_TubeRadius{ tubeRadius }
    , m_OpeningAngle{ openingAngle }
{
}

float sdf::CappedTorus::GetDistance(glm::vec3 const& point)
{
    return GetDistanceUnoptimized(point);
}

float sdf::CappedTorus::GetDistanceUnoptimized(glm::vec3 const& point)
{
    glm::vec3 p{ point };
    p.x = glm::abs(p.x);

    glm::vec2 const pxy{ p };

    glm::vec2 const sc{ glm::cos(m_OpeningAngle), glm::sin(m_OpeningAngle) };

    float const k{ (sc.y * p.x > sc.x * p.y) ? glm::dot(pxy, sc) : glm::length(pxy) };

    return glm::sqrt(dot(p, p) + m_InnerRadius * m_InnerRadius - 2.0f * m_InnerRadius * k) - m_TubeRadius;
}

sdf::Pyramid::Pyramid(glm::vec3 const& origin)
    : Object(origin)
{
}

float sdf::Pyramid::GetDistance(glm::vec3 const& point)
{
	return GetDistanceUnoptimized(point);
}

float sdf::Pyramid::GetDistanceUnoptimized(glm::vec3 const& point)
{
    glm::vec3 p{ point };

    float const m2{ m_Height * m_Height + 0.25f };

    p.x = glm::abs(p.x);
    p.z = glm::abs(p.z);

    if (p.z > p.x)
    {
        glm::vec3 const tempPoint{ p };
        p.x = tempPoint.z;
        p.z = tempPoint.x;
    }
    p.x -= 0.5f * m_BaseWidth;
    p.z -= 0.5f * m_BaseWidth;

    glm::vec3 const q{ p.z, m_Height * p.y - 0.5f * p.x, m_Height * p.x + 0.5f * p.y };

    float const s{ glm::max(-q.x, 0.0f) };
    float const t{ glm::clamp((q.y - 0.5f * p.z) / (m2 + 0.25f), 0.0f, 1.0f) };

    float const a{ m2 * (q.x + s) * (q.x + s) + q.y * q.y };
    float const b{ m2 * (q.x + 0.5f * t) * (q.x + 0.5f * t) + (q.y - m2 * t) * (q.y - m2 * t) };

    float const d2{ glm::min(q.y, -q.x * m2 - q.y * 0.5f) > 0.0f ? 0.0f : glm::min(a, b) };

    return glm::sqrt((d2 + q.z * q.z) / m2) * glm::sign(glm::max(q.z, -p.y));
}

float sdf::SmoothMin(float dist1, float dist2, float smoothness)
{
    float h{ glm::max(smoothness - glm::abs(dist1 - dist2), 0.0f) / smoothness };
    constexpr float smoothFraction{ 1.0f / 6.0f };
    return glm::min(dist1, dist2) - h * h * h * smoothness * smoothFraction;
}

std::vector<glm::vec3> sdf::GenerateSpherePoints(glm::vec3 const& origin, float radius)
{
    static std::array<uint32_t, sdf::PointCountHorizontal* sdf::PointCountVertical> idxPointArr
    {
        []()
        {
            for (uint32_t pixelIdx{}; pixelIdx < idxPointArr.size(); ++pixelIdx)
            {
                idxPointArr[pixelIdx] = pixelIdx;
            }
            return idxPointArr;
        }()
    };

    std::vector<glm::vec3> spherePointArr{ idxPointArr.size() };

    std::for_each(std::execution::par_unseq, idxPointArr.begin(), idxPointArr.end(),
        [&](uint32_t index)
        {
            int i = index % sdf::PointCountHorizontal;  // Vertical angle index (phi)
            int j = index / sdf::PointCountHorizontal;    // Horizontal angle index (theta)
            
            float theta = glm::radians(static_cast<float>(i)); // Convert degrees to radians
            float phi = glm::radians(static_cast<float>(j));   // Convert degrees to radians
            
            float x = radius * sin(phi) * cos(theta);
            float y = radius * sin(phi) * sin(theta);
            float z = radius * cos(phi);
            
            spherePointArr[index] = origin + glm::vec3(x, y, z);
        });

    return spherePointArr;
}