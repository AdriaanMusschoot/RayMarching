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
    Vector3 const OffsetPoint{ point.x, point.y, point.z + m_TotalTime * 0.5f };
    Vector3 const SpaceRepeatedPoint{ Vector3::Frac(OffsetPoint) - Vector3{ 0.5f, 0.5f, 0.5f } };
    return m_Box.GetDistance(SpaceRepeatedPoint);
}

void geo::SDSmoothSphereBoxPlane::Update(float elapsedSec)
{
    m_TotalTime += elapsedSec;
    m_Sphere.m_Origin.x = std::sin(m_TotalTime);
    m_Sphere.m_Origin.y = std::cos(m_TotalTime);
}

float geo::SDMandelBulb::GetDistance(const Vector3& point)
{
    Vector3 z = point;
    float dr = 2.0;
    float r = 0.0;
    float power{ 5 + (std::cos(m_TotalTime * 0.2f) + 1) * 0.5f * (10 - 5) };
    for (int i = 0; i < 10 ; i++) {
        r = z.Magnitude();
        if (r>2) break;
    
        // convert to polar coordinates
        float theta = asin( z.z/r );
        float phi = atan2( z.y,z.x );
        dr =  pow( r, power-1.0)*4*dr + 1.0;
        // scale and rotate the point
        float zr = pow( r,power);
        theta = theta*power;
        phi = phi*power;
        // convert back to cartesian coordinates
        z = zr*Vector3( cos(theta)*cos(phi), cos(theta)*sin(phi), sin(theta) );
        z+=point;
    }
    return 0.5f*log(r)*r/dr;
    //
    // Vector3 w = point;
    // float m = Vector3::Dot(w,w);
    //
    // Vector3 absW{ Vector3::Abs(w) };
    // Vector4 trap = Vector4( absW,m);
    // float dz = 1.0;
    //
    // for( int i=0; i<4; i++ )
    // {
    //     // trigonometric version (MUCH faster than polynomial)
    //     
    //     // dz = 8*z^7*dz
    //     dz = 8.0*pow(m,3.5)*dz + 1.0;
    //   
    //     // z = z^8+c
    //     float r = w.Magnitude();
    //     float b = 8.0*acos( w.y/r);
    //     float a = 8.0*atan2( w.x, w.z );
    //     w = point + pow(r,8.0) * Vector3( sin(b)*sin(a), cos(b), sin(b)*cos(a) );
    //
    //     Vector4 vectorWM{ absW,m };
    //     trap = Vector4( std::min(trap.x, vectorWM.x), std::min(trap.y, vectorWM.y), std::min(trap.z, vectorWM.z), std::min(trap.w, vectorWM.w) );
    //
    //     m = Vector3::Dot(w,w);
    //     if( m > 256.0 )
    //         break;
    // }
    //
    // // distance estimation (through the Hubbard-Douady potential)
    // return 0.25*log(m)*sqrt(m)/dz;
}

void geo::SDMandelBulb::Update(float elapsedSec)
{
    m_TotalTime += elapsedSec;
}
