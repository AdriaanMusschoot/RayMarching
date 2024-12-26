#include "BVHNode.h"

#include <numeric>

#include "SDFObjects.h"
#include "Misc.h"

sdf::BVHNode::BVHNode(glm::vec3 origin, float radius)
{
}

std::pair<float, sdf::Object*> sdf::BVHNode::GetDistance(const glm::vec3& point, bool useAABBs, HitRecord& outHitRecord) const
{
	return std::pair<float, sdf::Object*>();
}

void sdf::BVHNode::SetLeftNode(std::unique_ptr<BVHNode> node)
{
	m_LeftNodeUPtr = std::move(node);	
}

void sdf::BVHNode::SetRightNode(std::unique_ptr<BVHNode> node)
{
	m_RightNodeUPtr = std::move(node);
}

std::unique_ptr<sdf::BVHNode> sdf::BVHNode::CreateBVHNode(ObjectUPtrVec objects)
{
	if (objects.empty())
	{
		return nullptr;
	}

	glm::vec3 const origin{ CalculateBVHOrigin(objects) };
	float const radius{ CalculateBVHRadius(objects, origin) };

	std::unique_ptr nodeUPtr{ std::make_unique<BVHNode>(origin, radius) };

	if (objects.size() == 1)
	{
		nodeUPtr->m_ObjectUPtr = std::move(objects[0]);
	}
	else
	{
		auto [leftObjects, rightObjects] { std::move(SplitObjects(std::move(objects))) };
		nodeUPtr->m_LeftNodeUPtr = std::move(CreateBVHNode(std::move(leftObjects)));
		nodeUPtr->m_RightNodeUPtr = std::move(CreateBVHNode(std::move(rightObjects)));
	}

	return std::move(nodeUPtr);
}

glm::vec3 sdf::BVHNode::CalculateBVHOrigin(ObjectUPtrVec const& objects)
{
	glm::vec3 const origin
	{
		std::accumulate(objects.begin(), objects.end(), glm::vec3{ 0.f, 0.f, 0.f },
		[](glm::vec3 const& sum, std::unique_ptr<sdf::Object> const& obj)
		{
			return sum + obj->Origin();
		})
	};

	return origin / static_cast<float>(objects.size());
}

float sdf::BVHNode::CalculateBVHRadius(ObjectUPtrVec const& objects, glm::vec3 const& origin)
{
	auto maxDistanceIt
	{
		std::max_element(objects.begin(), objects.end(),
		[&origin](const std::unique_ptr<sdf::Object>& a, const std::unique_ptr<sdf::Object>& b)
		{
			float distanceA = glm::length(a->Origin() - origin) + a->GetEarlyOutRadius();
			float distanceB = glm::length(b->Origin() - origin) + b->GetEarlyOutRadius();
			return distanceA < distanceB;
		})
	};

	if (maxDistanceIt != objects.end())
	{
		return glm::length((*maxDistanceIt)->Origin() - origin) + (*maxDistanceIt)->GetEarlyOutRadius();
	}

	return 0.0f;
}

std::pair<sdf::ObjectUPtrVec, sdf::ObjectUPtrVec> sdf::BVHNode::SplitObjects(ObjectUPtrVec objects)
{
	std::sort(objects.begin(), objects.end(),
		[](const std::unique_ptr<sdf::Object>& a, const std::unique_ptr<sdf::Object>& b)
		{
			return a->Origin().x < b->Origin().x;
		});

	size_t const halfSize{ objects.size() / 2 };
	ObjectUPtrVec leftObjectVec{ halfSize };
	ObjectUPtrVec rightObjectVec{ halfSize };

	for (size_t i{ 0 }; i < halfSize; ++i)
	{
		leftObjectVec[i] = std::move(objects[i]);
		rightObjectVec[i] = std::move(objects[i + halfSize]);
	}

	return { std::move(leftObjectVec), std::move(rightObjectVec) };
}
