#include "Scene.h"

#include <algorithm>
#include <execution>

#include "Misc.h"
#include "Camera.h"

#include "SDFObjects.h"
#include "BVHNode.h"


namespace sdf
{
	bool Scene::m_UseEarlyOut{ true };

	bool Scene::m_UseBVH{ true };

	int Scene::m_BVHSteps{ 3 };

	//needs to be defaulted here, because it needs the full definition of the unique_ptr
	Scene::Scene() = default;
	Scene::~Scene() = default;

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
				if (object)
				{
					hitRecord.Shade = object->Shade();
				}
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

	void Scene::Update(float ElapsedSec)
	{
		m_Camera.Update(ElapsedSec);
	}

	std::pair<float, const sdf::Object*> Scene::GetDistanceToScene(const glm::vec3& point, HitRecord& outHitRecord) const
	{
		if (m_UseBVH)
		{
			return m_BVHRoot->GetDistance(point, m_UseEarlyOut, 0, m_BVHSteps, outHitRecord);
		}

		float minDistance{ std::numeric_limits<float>::max() };
		sdf::Object const* closestObject{ nullptr };

		std::for_each(std::execution::unseq, m_SDObjectUPtrVec.begin(), m_SDObjectUPtrVec.end(),
			[&](const std::unique_ptr<sdf::Object>& obj)
			{
				float const distance{ obj->GetDistance(point - obj->Origin(), m_UseEarlyOut, outHitRecord) };
		
				if (distance < minDistance)
				{
					minDistance = distance;
					closestObject = obj.get();
				}
			}
		);
		return { minDistance, closestObject };
	}

	void Scene::CreateBVHStructure()
	{
		std::vector<sdf::Object*> objectVec{ m_SDObjectUPtrVec.size() };

		std::transform(m_SDObjectUPtrVec.begin(), m_SDObjectUPtrVec.end(), objectVec.begin(),
			[](const std::unique_ptr<sdf::Object>& obj)
			{
				return obj.get();
			});

		m_BVHRoot = std::move(sdf::BVHNode::CreateBVHNode(objectVec));		
	}	
}