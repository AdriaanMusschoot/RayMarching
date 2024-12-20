#include "Scene.h"

#include <algorithm>

namespace sdf
{
	Scene::Scene()
		: m_Camera{ glm::vec3{0.f,0,-4.f }, 45 }
	{
	}

	std::pair<float,int> Scene::GetClosestHit(const glm::vec3& origin, const glm::vec3& direction, float minDistance, float maxDistance, int maxSteps) const
	{
		float currentDistance{ 0.f };

		glm::vec3 const origin1{ origin.x, origin.y, origin.z };
		glm::vec3 const direction1{ direction.x, direction.y, direction.z };

		int i{ 0 };
		for (i; i < maxSteps; ++i)
		{
			glm::vec3 const newPoint{ origin1 + direction1 * currentDistance };
			// const float sinDist{ std::sin(currentDistance * 0.3f) };
			// const float sinTime{ std::sin(m_TotalTime * 0.4f) };
			// newPoint = Matrix::CreateRotationZ(currentDistance * sinTime * 0.14).TransformPoint(newPoint);
			// newPoint += Vector3{ 0.f, sinDist * sinTime * 10, 0.f } * 0.3f;
			float const distanceAbleToTravel{ GetDistanceToScene(newPoint) };
			currentDistance += distanceAbleToTravel;

			if (distanceAbleToTravel < minDistance)
			{
				break;
			}
			if (currentDistance > maxDistance)
			{
				break;
			}
		}
		return { currentDistance, i };
	}

	float Scene::GetDistanceToScene(const glm::vec3& rayOrigin) const
	{
		auto minDistanceIt =
			std::min_element(m_SDObjectUPtrVec.begin(), m_SDObjectUPtrVec.end(),
			[&rayOrigin](const std::unique_ptr<sdf::Object>& a, const std::unique_ptr<sdf::Object>& b)
			{
				return a->GetDistance(rayOrigin) < b->GetDistance(rayOrigin);
			});

		if (minDistanceIt != m_SDObjectUPtrVec.end())
		{
			return (*minDistanceIt)->GetDistance(rayOrigin);
		}
		return -1;
	}

	PrismLinkScene::PrismLinkScene()
	{
		m_SDObjectUPtrVec.emplace_back(std::make_unique<sdf::Box>(glm::vec3{ 0, 0, 0 } , glm::vec3{ 0.1f, 0.1f, 0.1f }));
	}

	OctahedronTorusScene::OctahedronTorusScene()
	{
		
	}

	PyramidMandelBulbScene::PyramidMandelBulbScene()
	{
		m_SDObjectUPtrVec.emplace_back(std::make_unique<sdf::MandelBulb>());
	}	
}