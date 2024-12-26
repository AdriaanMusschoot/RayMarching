#pragma once
#include "glm/glm.hpp"

#include <memory>
#include <string>
#include <vector>

#include "SDFObjects.h"

namespace sdf
{
	struct HitRecord;
	struct Camera;

	class BVHNode;

	class Scene
	{
	public:
		Scene();
		virtual ~Scene();

		Scene(const Scene&) = delete;
		Scene(Scene&&) noexcept = delete;
		Scene& operator=(const Scene&) = delete;
		Scene& operator=(Scene&&) noexcept = delete;

		//returns the distance and the number of steps
		HitRecord GetClosestHit(const glm::vec3& origin, const glm::vec3& direction, float minDistance, float maxDistance, int maxSteps) const;

		void Update(float ElapsedSec);

		Camera const& GetCamera() const { return m_Camera; }

		void CreateBVHStructure();

		static bool m_UseAABBs;
	protected:		
		//vector needs full definition upon construction
		std::vector<std::unique_ptr<Object>> m_SDObjectUPtrVec{};
		static Camera m_Camera;

	private:
		std::unique_ptr<BVHNode> m_BVHRoot{ nullptr };

		std::pair<float, const sdf::Object*> GetDistanceToScene(const glm::vec3& point, HitRecord& outHitRecord) const;

	};

}
