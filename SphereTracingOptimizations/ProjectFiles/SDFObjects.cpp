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

float sdf::Object::FurthestSurfaceConcentricCircles(float initialRadius)
{
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime{ std::chrono::high_resolution_clock::now() };

    std::vector<glm::vec3> points{ GenerateSpherePoints(glm::vec3{ 0.f, 0.f, 0.f }, initialRadius) };

    std::vector<std::future<std::pair<float, float>>> futures{};

    size_t const chunkSize{ 10000 };

	for (int idx{}; idx < points.size(); idx += chunkSize)
	{
		size_t chunkEnd{ std::min(points.size(), idx + chunkSize) };
		std::promise<std::pair<float, float>> promise{};
		futures.emplace_back(promise.get_future());

		std::jthread thread
        (
            [&](std::promise<std::pair<float, float>> promise)
            {
                float localFurthestDistance = -std::numeric_limits<float>::max();
                float localMinimumDistance = std::numeric_limits<float>::max();

				std::for_each(points.begin() + idx, points.begin() + chunkEnd,
					[&](glm::vec3 const& point)
					{
                        float distance{ GetDistance(point) };

                        if (distance < 0.001f)
                        {
							localMinimumDistance = glm::length(point);
                        }
                        if (localFurthestDistance < distance)
						{
							localFurthestDistance = distance;
						}
					});
				promise.set_value(std::make_pair(localFurthestDistance, localMinimumDistance));
			}, std::move(promise)
        );
	}

	float furthestDistance = -std::numeric_limits<float>::max();

	for (auto& future : futures)
	{
		auto [localFurthestDistance, minimumDistance] = future.get();
		if (minimumDistance != std::numeric_limits<float>::max())
		{
			return minimumDistance;
		}
		if (furthestDistance < localFurthestDistance)
		{
			furthestDistance = localFurthestDistance;
		}
	}

    std::chrono::time_point<std::chrono::high_resolution_clock> endTime{ std::chrono::high_resolution_clock::now() };

    std::chrono::duration<float> elapsed = endTime - startTime;
    std::cout << elapsed.count() << "\n";

    return FurthestSurfaceConcentricCircles(furthestDistance);

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
    return glm::length(Origin() + point) - m_Radius;
}

sdf::BoxFrame::BoxFrame(glm::vec3 const& boxExtent, float roundedValue, glm::vec3 const& origin)
    : Object(origin)
    , m_BoxExtent{ boxExtent }
    , m_RoundedValue{ roundedValue }
{
    std::cout << FurthestSurfaceConcentricCircles() << "\n";
}

float sdf::BoxFrame::GetDistance(const glm::vec3& point)
{
    //return glm::length(glm::max(glm::abs(movedPoint) - m_BoxExtent, glm::vec3{ 0, 0, 0 }));
    glm::vec3 const p{ abs(point) - m_BoxExtent };
    glm::vec3 const q{ abs(p + m_RoundedValue) - m_RoundedValue };
    return glm::min(glm::min(
        glm::length(glm::max(glm::vec3(p.x, q.y, q.z), 0.0f)) + glm::min(glm::max(p.x, glm::max(q.y, q.z)), 0.0f),
        glm::length(glm::max(glm::vec3(q.x, p.y, q.z), 0.0f)) + glm::min(glm::max(q.x, glm::max(p.y, q.z)), 0.0f)),
        glm::length(glm::max(glm::vec3(q.x, q.y, p.z), 0.0f)) + glm::min(glm::max(q.x, glm::max(q.y, p.z)), 0.0f));
}

sdf::HexagonalPrism::HexagonalPrism(float depth, float radius, glm::vec3 const& origin)
    : Object(origin)
    , m_Depth{ depth }
    , m_Radius{ radius }
{
}

float sdf::HexagonalPrism::GetDistance(const glm::vec3& point)
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
    glm::vec3 movedPoint{ point - Origin() };
    movedPoint = glm::abs(movedPoint);
    float m{ movedPoint.x + movedPoint.y + movedPoint.z - m_Size };

    glm::vec3 q;
    if (3.0f * movedPoint.x < m)
    {
        q = movedPoint;
    }
    else if (3.0f * movedPoint.y < m)
    {
        q.x = movedPoint.y;
        q.y = movedPoint.z;
        q.z = movedPoint.x;
    }
    else if (3.0f * movedPoint.z < m)
    {
        q.x = movedPoint.z;
        q.y = movedPoint.x;
        q.z = movedPoint.y;
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
    glm::vec3 const movedPoint{ point - Origin() };

    int iterations = 4;
    glm::vec3 w = movedPoint;
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
        w.x = movedPoint.x + 64.0 * x * y * z * (x2 - z2) * k4 * (x4 - 6.0 * x2 * z2 + z4) * k1 * k2;
        w.y = movedPoint.y + -16.0 * y2 * k3 * k4 * k4 + k1 * k1;
        w.z = movedPoint.z + -8.0 * y * k4 * (x4 * x4 - 28.0 * x4 * x2 * z2 + 70.0 * x4 * z4 - 28.0 * x2 * z2 * z4
            + z4 * z4) * k1 * k2;

        trap = glm::min(trap, glm::vec4{ abs(w), m });

        m = dot(w, w);
        if (m > 256)
            break;
    }
    return 0.25f * glm::log(m) * sqrt(m) / dz;

    //glm::vec3 z = movedPoint;
    //float dr{ 1.0f };
    //float r{ 0.0f };
    //float power{ 10 };
    //for (int i = 0; i < 10; i++)
    //{
    //    r = glm::length(z);
    //    if (r > 2.f) break;
    //    // convert to polar coordinates
    //    float theta = asin(z.z / r);
    //    float phi = atan2(z.y, z.x);
    //    dr = 4.0f * std::pow(r, power - 1.0f) * dr + 1.0f;
    //    // scale and rotate the point
    //    float zr = std::pow(r, power);
    //    theta = theta * power;
    //    phi = phi * power;
    //    // convert back to cartesian coordinates
    //    z = zr * glm::vec3(std::cos(theta) * std::cos(phi), std::cos(theta) * std::sin(phi), std::sin(theta));
    //    z += movedPoint;
    //}
    //return 0.5f * std::log(r) * r / dr;

    //int Iterations = 10;
    //float Power = 10;
    //glm::vec3 z = movedPoint;
    //float dr = 1.0;
    //float r = 0.0;
    //for (int i{ 0 }; i < Iterations; i++)
    //{
    //    // convert to polar coordinates
    //    r = glm::length(z);
    //    float theta = glm::acos(z.z / r);
    //    float phi = glm::atan(z.y, z.x);
    //    if (r > 2) break; // the point has escaped the threshold of 2
    //    // evaluate spatial derivative
    //    dr = glm::pow(r, Power - 1.0) * Power * dr + 1.0;
    //    // scale and rotate the point
    //    float zr = glm::pow(r, Power);
    //    theta = theta * Power;
    //    phi = phi * Power;
    //    // convert back to cartesian coordinates
    //    z = zr * glm::vec3(glm::sin(theta) * glm::cos(phi), glm::sin(phi) * glm::sin(theta), glm::cos(theta));
    //    z = z + movedPoint;
    //}
    //return 0.5 * glm::log(r) * r / dr;
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
    glm::vec3 movedPoint{ point - Origin() };
    movedPoint.x = glm::abs(movedPoint.x);

    glm::vec2 const pxy{ movedPoint };

    glm::vec2 const sc{ glm::cos(m_OpeningAngle), glm::sin(m_OpeningAngle) };

    float const k{ (sc.y * movedPoint.x > sc.x * movedPoint.y) ? glm::dot(pxy, sc) : glm::length(pxy) };

    return glm::sqrt(dot(movedPoint, movedPoint) + m_InnerRadius * m_InnerRadius - 2.0f * m_InnerRadius * k) - m_TubeRadius;
}

sdf::Pyramid::Pyramid(glm::vec3 const& origin)
    : Object(origin)
{
}

float sdf::Pyramid::GetDistance(glm::vec3 const& point)
{
    glm::vec3 movedPoint{ point - Origin() };

    float const m2{ m_Height * m_Height + 0.25f };

    movedPoint.x = glm::abs(movedPoint.x);
    movedPoint.z = glm::abs(movedPoint.z);

    if (movedPoint.z > movedPoint.x)
    {
        glm::vec3 const tempPoint{ movedPoint };
        movedPoint.x = tempPoint.z;
        movedPoint.z = tempPoint.x;
    }
    movedPoint.x -= 0.5f * m_BaseWidth;
    movedPoint.z -= 0.5f * m_BaseWidth;

    glm::vec3 const q{ movedPoint.z, m_Height * movedPoint.y - 0.5f * movedPoint.x, m_Height * movedPoint.x + 0.5f * movedPoint.y };

    float const s{ glm::max(-q.x, 0.0f) };
    float const t{ glm::clamp((q.y - 0.5f * movedPoint.z) / (m2 + 0.25f), 0.0f, 1.0f) };

    float const a{ m2 * (q.x + s) * (q.x + s) + q.y * q.y };
    float const b{ m2 * (q.x + 0.5f * t) * (q.x + 0.5f * t) + (q.y - m2 * t) * (q.y - m2 * t) };

    float const d2{ glm::min(q.y, -q.x * m2 - q.y * 0.5f) > 0.0f ? 0.0f : glm::min(a, b) };

    return glm::sqrt((d2 + q.z * q.z) / m2) * glm::sign(glm::max(q.z, -movedPoint.y));
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