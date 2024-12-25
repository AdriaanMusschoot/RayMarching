#include "Scene.h"

#include <algorithm>
#include <iostream>
#include <execution>
#include "Misc.h"

namespace sdf
{
	bool Scene::m_UseAABBs{ true };

	Camera Scene::m_Camera{ glm::vec3{ 0.f,0,-4.f }, 90 };

	HitRecord Scene::GetClosestHit(const glm::vec3& origin, const glm::vec3& direction, float minDistance, float maxDistance, int maxSteps) const
	{
		HitRecord hitRecord{};
		float currentDistance{ 0.f };

		glm::vec3 const origin1{ origin.x, origin.y, origin.z };
		glm::vec3 const direction1{ direction.x, direction.y, direction.z };

		int currentStep{ 0 };
		for (currentStep; currentStep < maxSteps; ++currentStep)
		{
			glm::vec3 const newPoint{ origin1 + direction1 * currentDistance };
			// const float sinDist{ std::sin(currentDistance * 0.3f) };
			// const float sinTime{ std::sin(m_TotalTime * 0.4f) };
			// newPoint = Matrix::CreateRotationZ(currentDistance * sinTime * 0.14).TransformPoint(newPoint);
			// newPoint += Vector3{ 0.f, sinDist * sinTime * 10, 0.f } * 0.3f;
			const auto[distanceAbleToTravel, object]{ GetDistanceToScene(newPoint, hitRecord) };
			currentDistance += distanceAbleToTravel;

			if (distanceAbleToTravel < minDistance)
			{
				hitRecord.DidHit = true;
				hitRecord.Shade = object->Shade();
				break;
			}
			if (currentDistance > maxDistance)
			{
				break;
			}
		}

		hitRecord.Distance = currentDistance;
		hitRecord.TotalSteps = currentStep;

		return hitRecord;
	}

	void Scene::CreateBVH()
	{
		std::cout << "Traverse the bvh\n";
	}

	std::pair<float, const sdf::Object*> Scene::GetDistanceToScene(const glm::vec3& point, HitRecord& outHitRecord) const
	{
		float minDistance{ std::numeric_limits<float>::max() };
		const sdf::Object* closestObject{ nullptr };

		//traverse all objects in the scene
		std::for_each(std::execution::unseq, m_SDObjectUPtrVec.begin(), m_SDObjectUPtrVec.end(),
			[&](const std::unique_ptr<sdf::Object>& obj)
			{
				float const distance{ obj->GetDistance(point - obj->Origin(), m_UseAABBs, outHitRecord) };

				if (distance < minDistance)
				{
					minDistance = distance;
					closestObject = obj.get();
				}
			}
		);
		//traverse the bvh instead 


		return { minDistance, closestObject };
	}

	SceneEasyComplexity::SceneEasyComplexity()
	{
		m_SDObjectUPtrVec.emplace_back(std::make_unique<sdf::Link>(0.5f, 0.5f, 0.1f, glm::vec3{ 1.f, 0.f, 0.f }));
		m_SDObjectUPtrVec.emplace_back(std::make_unique<sdf::Octahedron>(1.f, glm::vec3{ -1.f, 0.f, 0.f }));
		CreateBVH();
	}

	SceneMediumComplexity::SceneMediumComplexity()
	{
		m_SDObjectUPtrVec.emplace_back(std::make_unique<sdf::BoxFrame>(glm::vec3{ 0.7f, 0.7f, 0.7f }, 0.05f, glm::vec3{ 2.f, 1.f, 0.f }));
		m_SDObjectUPtrVec.emplace_back(std::make_unique<sdf::HexagonalPrism>(0.7f, 0.7f, glm::vec3{ -2.f, 1.f, 0.f }));
		m_SDObjectUPtrVec.emplace_back(std::make_unique<sdf::Pyramid>(2.f, glm::vec3{ 0.f, -1.8f, 0.f }));
		CreateBVH();
	}

	SceneHighComplexity::SceneHighComplexity()
	{
		m_SDObjectUPtrVec.emplace_back(std::make_unique<sdf::MandelBulb>(glm::vec3{ 2.f, 0.f, 0.f }));
		m_SDObjectUPtrVec.emplace_back(std::make_unique<sdf::MandelBulb>(glm::vec3{ -2.f, 0.f, 0.f }));
		CreateBVH();
	}	
}