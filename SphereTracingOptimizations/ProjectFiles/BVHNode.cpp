#include "BVHNode.h"

#include <numeric>
#include <iostream>

#include "SDFObjects.h"
#include "Misc.h"

sdf::BVHNode::BVHNode(glm::vec3 origin, float radius)
	: m_Origin{ origin }
	, m_Radius{ radius }
{
}

std::pair<float, sdf::Object*> sdf::BVHNode::GetDistance(const glm::vec3& point, bool useEarlyOuts, int currentStep, int maxSteps, HitRecord& outHitRecord) const
{
	if (currentStep >= maxSteps)
	{
		if (m_ObjectUPtr)
		{
			return{ m_ObjectUPtr->GetDistance(point - m_ObjectUPtr->Origin(), useEarlyOuts, outHitRecord), m_ObjectUPtr };
		}
		return { glm::length(point - m_Origin) - m_Radius, nullptr };
	}

	currentStep++;
	//if leaf node just return the distance to the object
	if (m_ObjectUPtr)
	{
		return { m_ObjectUPtr->GetDistance(point - m_ObjectUPtr->Origin(), useEarlyOuts, outHitRecord), m_ObjectUPtr };
	}

	//if the distance to the bounding volume is large enough return it 
	if (float boundingVolumeDistance{ glm::length(point - m_Origin) - m_Radius };
		boundingVolumeDistance > 0.1f)
	{
		return { boundingVolumeDistance, nullptr };
	}

	//we are in the bounding volume check the children
	auto const leftResult{ m_LeftNodeUPtr->GetDistance(point, useEarlyOuts, currentStep, maxSteps, outHitRecord) };
	auto const rightResult{ m_RightNodeUPtr->GetDistance(point, useEarlyOuts, currentStep, maxSteps, outHitRecord) };

	if (leftResult.first < rightResult.first)
	{
		return leftResult;
	}
	return rightResult;
}

std::unique_ptr<sdf::BVHNode> sdf::BVHNode::CreateBVHNode(std::vector<sdf::Object*> const& objects)
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
		nodeUPtr->m_ObjectUPtr = objects[0];
	}
	else
	{
		auto [leftObjects, rightObjects] { std::move(SplitObjects(objects)) };
		nodeUPtr->m_LeftNodeUPtr = std::move(CreateBVHNode(leftObjects));
		nodeUPtr->m_RightNodeUPtr = std::move(CreateBVHNode(rightObjects));
	}

	std::cout << "BVHNode created with " << objects.size() << " objects" << std::endl;

	return std::move(nodeUPtr);
}

glm::vec3 sdf::BVHNode::CalculateBVHOrigin(std::vector<sdf::Object*> const& objects)
{
	glm::vec3 const origin
	{
		std::accumulate(objects.begin(), objects.end(), glm::vec3{ 0.f, 0.f, 0.f },
		[](glm::vec3 const& sum, sdf::Object const* obj)
		{
			return sum + obj->Origin();
		})
	};

	return origin / static_cast<float>(objects.size());
}

float sdf::BVHNode::CalculateBVHRadius(std::vector<sdf::Object*> const& objects, glm::vec3 const& origin)
{
	auto maxDistanceIt
	{
		std::max_element(objects.begin(), objects.end(),
		[&origin](sdf::Object const* a, sdf::Object const* b)
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

std::pair<std::vector<sdf::Object*>, std::vector<sdf::Object*>> sdf::BVHNode::SplitObjects(std::vector<sdf::Object*> const& objects)
{
	std::vector<Object*> objectPtrVec{ objects.size() };

	std::transform(objects.begin(), objects.end(), objectPtrVec.begin(),
		[](sdf::Object* obj)
		{
			return obj;
		});
	
	std::sort(objectPtrVec.begin(), objectPtrVec.end(),
		[](const sdf::Object* a, const sdf::Object* b)
		{
			return a->Origin().x < b->Origin().x;
		});

	size_t const halfSize{ objects.size() / 2 };
	std::vector<Object*> leftObjectVec{ objectPtrVec.begin(), objectPtrVec.begin() + halfSize };
	std::vector<Object*> rightObjectVec{ objectPtrVec.begin() + halfSize, objectPtrVec.end() };

	return { std::move(leftObjectVec), std::move(rightObjectVec) };
}
