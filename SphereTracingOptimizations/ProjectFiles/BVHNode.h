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

		std::pair<float, sdf::Object*> GetDistance(const glm::vec3& point, bool useAABBs, HitRecord& outHitRecord) const;

		void SetLeftNode(std::unique_ptr<BVHNode> node);
		void SetRightNode(std::unique_ptr<BVHNode> node);

		static std::unique_ptr<BVHNode> CreateBVHNode(ObjectUPtrVec objects);
	private:
		glm::vec3 m_Origin{};
		float m_Radius{};
		std::unique_ptr<BVHNode> m_LeftNodeUPtr{ nullptr };
		std::unique_ptr<BVHNode> m_RightNodeUPtr{ nullptr };

		std::unique_ptr<sdf::Object> m_ObjectUPtr{ nullptr };

		static glm::vec3 CalculateBVHOrigin(ObjectUPtrVec const& objects);
		static float CalculateBVHRadius(ObjectUPtrVec const& objects, glm::vec3 const& origin);
		static std::pair<ObjectUPtrVec, ObjectUPtrVec> SplitObjects(ObjectUPtrVec objects);
	};

}