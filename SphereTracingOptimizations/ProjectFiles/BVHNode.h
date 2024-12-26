#pragma once
#include "glm/glm.hpp"

#include <memory>
#include <vector>

namespace sdf
{
	class Object;
	struct HitRecord;

	using ObjectUPtrVec = std::vector<std::unique_ptr<sdf::Object>>;


	class BVHNode final
	{
	public:
		BVHNode(glm::vec3 origin, float radius);

		std::pair<float, sdf::Object*> GetDistance(const glm::vec3& point, bool useEarlyOuts, int currentStep, int maxSteps, HitRecord& outHitRecord) const;

		static std::unique_ptr<BVHNode> CreateBVHNode(std::vector<sdf::Object*> const& objects);
	private:
		glm::vec3 m_Origin{};
		float m_Radius{ FLT_MAX };
		std::unique_ptr<BVHNode> m_LeftNodeUPtr{ nullptr };
		std::unique_ptr<BVHNode> m_RightNodeUPtr{ nullptr };

		sdf::Object* m_ObjectUPtr{ nullptr };

		static glm::vec3 CalculateBVHOrigin(std::vector<sdf::Object*> const& objects);
		static float CalculateBVHRadius(std::vector<sdf::Object*> const& objects, glm::vec3 const& origin);
		static std::pair<std::vector<sdf::Object*>, std::vector<sdf::Object*>> SplitObjects(std::vector<sdf::Object*> const& objects);
		static float CalculateBoundingBoxArea(std::vector<sdf::Object*> const& objects);
	};

}