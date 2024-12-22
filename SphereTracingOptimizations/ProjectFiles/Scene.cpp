#include "Scene.h"

#include <algorithm>

namespace sdf
{
	Camera Scene::m_Camera{ glm::vec3{0.f,0,-4.f }, 45 };

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

	float Scene::GetDistanceToScene(const glm::vec3& point) const
	{
		float minDistance{ std::numeric_limits<float>::max() };

		std::for_each(m_SDObjectUPtrVec.begin(), m_SDObjectUPtrVec.end(),
			[&point, &minDistance](const std::unique_ptr<sdf::Object>& obj)
			{
				float const distance{ obj->GetDistance(point - obj->Origin()) };

				if (distance < minDistance)
				{
					minDistance = distance;
				}
			}
		);

		return minDistance;
	}

	SceneEasyComplexity::SceneEasyComplexity()
	{
		m_SDObjectUPtrVec.emplace_back(std::make_unique<sdf::Link>());
		m_SDObjectUPtrVec.emplace_back(std::make_unique<sdf::Octahedron>());
	}

	SceneMediumComplexity::SceneMediumComplexity()
	{
		m_SDObjectUPtrVec.emplace_back(std::make_unique<sdf::BoxFrame>());
		m_SDObjectUPtrVec.emplace_back(std::make_unique<sdf::HexagonalPrism>());
	}

	SceneMaxComplexity::SceneMaxComplexity()
	{
		m_SDObjectUPtrVec.emplace_back(std::make_unique<sdf::Pyramid>());
		m_SDObjectUPtrVec.emplace_back(std::make_unique<sdf::MandelBulb>());
	}	
}