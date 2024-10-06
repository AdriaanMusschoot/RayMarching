#pragma once
#include <cassert>
#include <fstream>
#include "Math.h"
#include "DataTypes.h"
#include <iostream>

#define SLABTEST
#define MULLERTROMBORE

namespace dae
{
	namespace GeometryUtils
	{
#pragma region Slab test
		inline bool SlabTest_TriangleMesh(const TriangleMesh& mesh, const Ray& ray)
		{
			float tx1 = (mesh.transformedMinAABB.x - ray.origin.x) / ray.direction.x;
			float tx2 = (mesh.transformedMaxAABB.x - ray.origin.x) / ray.direction.x;

			float tmin = std::min(tx1, tx2); 
			float tmax = std::max(tx1, tx2);
			
			float ty1 = (mesh.transformedMinAABB.y - ray.origin.y) / ray.direction.y; 
			float ty2 = (mesh.transformedMaxAABB.y - ray.origin.y) / ray.direction.y;

			tmin = std::max(tmin, std::min(ty1, ty2)); 
			tmax = std::min(tmax, std::max(ty1, ty2));

			float tz1 = (mesh.transformedMinAABB.z - ray.origin.z) / ray.direction.z; 
			float tz2 = (mesh.transformedMaxAABB.z - ray.origin.z) / ray.direction.z;

			tmin = std::max(tmin,std::min(tz1, tz2)); 
			tmax = std::min(tmax,std::max(tz1, tz2));
			return tmax > 0 && tmax >= tmin;
		}
	

#pragma endregion Slab test
#pragma region Sphere HitTest
		//SPHERE HIT-TESTS
		inline bool HitTest_Sphere(const Sphere& sphere, const Ray& ray, HitRecord& hitRecord, bool ignoreHitRecord = false)
		{
			//todo W1
			//assert(false && "No Implemented Yet!");
			Vector3 rayOriginToSphereOrigin{ ray.origin - sphere.origin };
			//float a{ Vector3::Dot(ray.direction, ray.direction) }; -> no need result is always 1
			float b{ Vector3::Dot(rayOriginToSphereOrigin, ray.direction) };
			float c{ Vector3::Dot(rayOriginToSphereOrigin, rayOriginToSphereOrigin) - (sphere.radius * sphere.radius) };

			float discriminant{ (b * b) - (c) };
			if (discriminant < 0)
				return false;
		
			float t{ (-b - sqrt(discriminant))  };

			if (t < ray.min) t = (-b + sqrt(discriminant));

			if (t < ray.min || t > ray.max)
				return false;

			if (!ignoreHitRecord)
			{
				hitRecord.t = t;
				hitRecord.origin = ray.origin + ray.direction * hitRecord.t;
				hitRecord.normal = (hitRecord.origin - sphere.origin) / sphere.radius;
				hitRecord.didHit = true;
				hitRecord.materialIndex = sphere.materialIndex;
			}

			return true;
		}

		inline bool HitTest_Sphere(const Sphere& sphere, const Ray& ray)
		{
			HitRecord temp{};
			return HitTest_Sphere(sphere, ray, temp, true);
		}
#pragma endregion
#pragma region Plane HitTest
		//PLANE HIT-TESTS
		inline bool HitTest_Plane(const Plane& plane, const Ray& ray, HitRecord& hitRecord, bool ignoreHitRecord = false)
		{
			//todo W1
			float t{ Vector3::Dot(plane.origin - ray.origin, plane.normal) / (Vector3::Dot(ray.direction, plane.normal)) };
			if (t > ray.max || t < ray.min) return false;
			
			if (Vector3::Dot(plane.normal, ray.direction) > 0) return false;

			if (!ignoreHitRecord)
			{
				hitRecord.t = t;
				hitRecord.origin = ray.origin + ray.direction * hitRecord.t;
				hitRecord.didHit = true;
				hitRecord.materialIndex = plane.materialIndex;
				hitRecord.normal = plane.normal;
			}
			return true;
		}

		inline bool HitTest_Plane(const Plane& plane, const Ray& ray)
		{
			HitRecord temp{};
			return HitTest_Plane(plane, ray, temp, true);
		}
#pragma endregion
#pragma region Triangle HitTest
		//TRIANGLE HIT-TESTS
		inline bool HitTest_Triangle(const Triangle& triangle, const Ray& ray, HitRecord& hitRecord, bool ignoreHitRecord = false)
		{
			//todo W5
#ifdef MULLERTROMBORE
			Vector3 edge1{ triangle.v1 - triangle.v0 };
			Vector3 edge2{ triangle.v2 - triangle.v0 };

			Vector3 h{ Vector3::Cross(ray.direction, edge2) };
			float a{ Vector3::Dot(edge1, h) };
			
			switch (triangle.cullMode)
			{
			case TriangleCullMode::BackFaceCulling:
				if (a < -FLT_EPSILON) return false;
				break;
			case TriangleCullMode::FrontFaceCulling:
				if (a > FLT_EPSILON) return false;
				break;
			}

			float f{ 1.0f / a };
			Vector3 s{ ray.origin - triangle.v0 };
			float u{ f * Vector3::Dot(s, h) };

			if (u < 0.0 || u > 1.0) return false;

			Vector3 q{ Vector3::Cross(s, edge1) };
			float v{ f * Vector3::Dot(ray.direction, q) };

			if (v < 0.0 || u + v > 1.0) return false;

			float t{ f * Vector3::Dot(edge2, q) };

			if (t > ray.max || t < ray.min) return false;

			if (!ignoreHitRecord)
			{
				hitRecord.t = t;
				hitRecord.origin = ray.origin + ray.direction * hitRecord.t;
				hitRecord.didHit = true;
				hitRecord.materialIndex = triangle.materialIndex;
				hitRecord.normal = triangle.normal;
			}
			return true;
#else
			Vector3 a{ triangle.v1 - triangle.v0 };
			Vector3 b{ triangle.v2 - triangle.v0 };

			const float dotRayDirectionAndNormal{ Vector3::Dot(triangle.normal, ray.direction) };
			switch (triangle.cullMode)
			{
			case TriangleCullMode::FrontFaceCulling:
				if (dotRayDirectionAndNormal < 0) return false;
				break;
			case TriangleCullMode::BackFaceCulling:
				if (dotRayDirectionAndNormal > 0) return false;
				break;
			}

			if (dotRayDirectionAndNormal < FLT_EPSILON && dotRayDirectionAndNormal > -FLT_EPSILON) return false;

			float t{ Vector3::Dot(triangle.v0 - ray.origin, triangle.normal) / dotRayDirectionAndNormal };

			if (t < ray.min || t > ray.max) return false;

			Vector3 intersectionPoint{ ray.origin + ray.direction * t };

			Vector3 e{};
			Vector3 p{};

			e = triangle.v1 - triangle.v0;
			p = intersectionPoint - triangle.v0;
			if (Vector3::Dot(Vector3::Cross(e, p), triangle.normal) < 0) return false;

			e = triangle.v2 - triangle.v1;
			p = intersectionPoint - triangle.v1;
			if (Vector3::Dot(Vector3::Cross(e, p), triangle.normal) < 0) return false;

			e = triangle.v0 - triangle.v2;
			p = intersectionPoint - triangle.v2;
			if (Vector3::Dot(Vector3::Cross(e, p), triangle.normal) < 0) return false;

			if (!ignoreHitRecord)
			{
				hitRecord.t = t;
				hitRecord.origin = intersectionPoint;
				hitRecord.didHit = true;
				hitRecord.materialIndex = triangle.materialIndex;
				hitRecord.normal = triangle.normal;
			}
			return true;
#endif
		}

		inline bool HitTest_Triangle(const Triangle& triangle, const Ray& ray)
		{
			HitRecord temp{};
			return HitTest_Triangle(triangle, ray, temp, true);
		}
#pragma endregion
#pragma region TriangeMesh HitTest
		inline bool HitTest_TriangleMesh(const TriangleMesh& mesh, const Ray& ray, HitRecord& hitRecord, bool ignoreHitRecord = false)
		{
			//todo W5
#ifdef SLABTEST
			if (!SlabTest_TriangleMesh(mesh, ray)) return false;
#endif

			bool hitSomething{ false };
			hitRecord.t = FLT_MAX;

			for (int idx{}; idx < mesh.indices.size(); idx += 3)
			{
				const Vector3& v0 = mesh.transformedPositions[mesh.indices[idx]];
				const Vector3& v1 = mesh.transformedPositions[mesh.indices[idx + 1]];
				const Vector3& v2 = mesh.transformedPositions[mesh.indices[idx + 2]];

#ifdef MULLERTROMBORE
				const Vector3 edge1{ v1 - v0 };
				const Vector3 edge2{ v2 - v0 };

				const Vector3 h{ Vector3::Cross(ray.direction, edge2) };
				const float a{ Vector3::Dot(edge1, h) };

				switch (mesh.cullMode)
				{
				case TriangleCullMode::BackFaceCulling:
					if (a < -FLT_EPSILON) continue;
					break;
				case TriangleCullMode::FrontFaceCulling:
					if (a > FLT_EPSILON) continue;
					break;
				}

				const float f{ 1.0f / a };
				const Vector3 s{ ray.origin - v0 };
				const float u{ f * Vector3::Dot(s, h) };

				if (u < 0.0 || u > 1.0) continue;

				const Vector3 q{ Vector3::Cross(s, edge1) };
				const float v{ f * Vector3::Dot(ray.direction, q) };

				if (v < 0.0 || u + v > 1.0) continue;

				const float t{ f * Vector3::Dot(edge2, q) };

				if (t > ray.max || t < ray.min) continue;

				if (t < hitRecord.t)
				{
					hitSomething = true;
					hitRecord.t = t;

					if (ignoreHitRecord) return hitSomething;

					if (!ignoreHitRecord)
					{
						hitRecord.t = t;
						hitRecord.origin = ray.origin + ray.direction * hitRecord.t;
						hitRecord.didHit = true;
						hitRecord.materialIndex = mesh.materialIndex;
						hitRecord.normal = Vector3::Cross(edge1, edge2).Normalized();
					}
				}
#else

				Vector3 a{ v1 - v0 };
				Vector3 b{ v2 - v0 };

				Vector3 normal{ Vector3::Cross(a, b).Normalized() };
				const float dotRayDirectionAndNormal{ Vector3::Dot(normal, ray.direction) };
				switch (mesh.cullMode)
				{
				case TriangleCullMode::FrontFaceCulling:
					if (dotRayDirectionAndNormal < 0) continue;
					break;
				case TriangleCullMode::BackFaceCulling:
					if (dotRayDirectionAndNormal > 0) continue;
					break;
				}

				if (dotRayDirectionAndNormal < FLT_EPSILON && dotRayDirectionAndNormal > -FLT_EPSILON) continue;

				float t{ Vector3::Dot(v0 - ray.origin, normal) / dotRayDirectionAndNormal };

				if (t < ray.min || t > ray.max) continue;

				Vector3 intersectionPoint{ ray.origin + ray.direction * t };

				Vector3 e{};
				Vector3 p{};

				e = v1 - v0;
				p = intersectionPoint - v0;
				if (Vector3::Dot(Vector3::Cross(e, p), normal) < 0) continue;

				e = v2 - v1;
				p = intersectionPoint - v1;
				if (Vector3::Dot(Vector3::Cross(e, p), normal) < 0) continue;

				e = v0 - v2;
				p = intersectionPoint - v2;
				if (Vector3::Dot(Vector3::Cross(e, p), normal) < 0) continue;

				if (t < hitRecord.t)
				{
					hitSomething = true;
					hitRecord.t = t;

					if (ignoreHitRecord) retunr hitSomething;

					if (!ignoreHitRecord)
					{
						hitRecord.t = t;
						hitRecord.origin = intersectionPoint;
						hitRecord.didHit = true;
						hitRecord.materialIndex = mesh.materialIndex;
						hitRecord.normal = normal;
					}
				}
#endif
			}
			return hitSomething;
		}

		inline bool HitTest_TriangleMesh(const TriangleMesh& mesh, const Ray& ray)
		{
			HitRecord temp{};
			return HitTest_TriangleMesh(mesh, ray, temp, true);
		}
#pragma endregion
	}

	namespace LightUtils
	{
		//Direction from target to light
		inline Vector3 GetDirectionToLight(const Light& light, const Vector3 origin)
		{
			return { light.origin - origin };
		}

		inline ColorRGB GetRadiance(const Light& light, const Vector3& target)
		{

			if (light.type == LightType::Point)
			{
				const float distanceSquared{ GetDirectionToLight(light, target).SqrMagnitude() };
				const float irradiance{ light.intensity / distanceSquared };
				return { irradiance * light.color };
			}
			if (light.type == LightType::Directional)
			{
				return { light.color * light.intensity };
			}

		}
	}

	namespace Utils
	{
		//Just parses vertices and indices
#pragma warning(push)
#pragma warning(disable : 4505) //Warning unreferenced local function
		static bool ParseOBJ(const std::string& filename, std::vector<Vector3>& positions, std::vector<Vector3>& normals, std::vector<int>& indices)
		{
			std::ifstream file(filename);
			if (!file)
				return false;

			std::string sCommand;
			// start a while iteration ending when the end of file is reached (ios::eof)
			while (!file.eof())
			{
				//read the first word of the string, use the >> operator (istream::operator>>) 
				file >> sCommand;
				//use conditional statements to process the different commands	
				if (sCommand == "#")
				{
					// Ignore Comment
				}
				else if (sCommand == "v")
				{
					//Vertex
					float x, y, z;
					file >> x >> y >> z;
					positions.push_back({ x, y, z });
				}
				else if (sCommand == "f")
				{
					float i0, i1, i2;
					file >> i0 >> i1 >> i2;

					indices.push_back((int)i0 - 1);
					indices.push_back((int)i1 - 1);
					indices.push_back((int)i2 - 1);
				}
				//read till end of line and ignore all remaining chars
				file.ignore(1000, '\n');

				if (file.eof()) 
					break;
			}

			//Precompute normals
			for (uint64_t index = 0; index < indices.size(); index += 3)
			{
				uint32_t i0 = indices[index];
				uint32_t i1 = indices[index + 1];
				uint32_t i2 = indices[index + 2];

				Vector3 edgeV0V1 = positions[i1] - positions[i0];
				Vector3 edgeV0V2 = positions[i2] - positions[i0];
				Vector3 normal = Vector3::Cross(edgeV0V1, edgeV0V2);

				if(isnan(normal.x))
				{
					int k = 0;
				}

				normal.Normalize();
				if (isnan(normal.x))
				{
					int k = 0;
				}

				normals.push_back(normal);
			}

			return true;
		}
#pragma warning(pop)
	}
}