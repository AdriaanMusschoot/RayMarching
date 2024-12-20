#include "Scene.h"

#include <algorithm>

namespace sdf
{
	Scene::Scene()
		: m_Camera{ vm::Vector3{ 0.f,0,-4.f }, 45 }
	{
	}

	Scene::~Scene()
	{
	}

	std::pair<float,int> Scene::GetClosestHit(const vm::Vector3& origin, const vm::Vector3& direction, float minDistance, float maxDistance, int maxSteps) const
	{
		float currentDistance{ 0.f };
		vm::Vector3 const& rayOrigin{ origin };

		int i{};
		for (i = 0; i < maxSteps; ++i)
		{
			vm::Vector3 const newPoint{ rayOrigin + direction * currentDistance };
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

	float Scene::GetDistanceToScene(const vm::Vector3& rayOrigin) const
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

	MandelBulbScene::MandelBulbScene()
	{
		m_SDObjectUPtrVec.emplace_back(std::make_unique<sdf::MandelBulb>());
	}	

	BoxScene::BoxScene()
	{
		m_SDObjectUPtrVec.emplace_back(std::make_unique<sdf::Box>(vm::Vector3{ 0, 0, 0 }, vm::Vector3{ 1, 1, 1 }));
	}
}