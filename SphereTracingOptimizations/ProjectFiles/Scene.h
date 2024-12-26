#pragma once
#include <memory>
#include <string>
#include <vector>
#include "Camera.h"
#include "SDFObjects.h"

namespace sdf
{
	class GameTimer;
	struct SDPlane;
	struct SDSphere;
	struct Light;

	using ObjectUPtrVec = std::vector<std::unique_ptr<sdf::Object>>;

	struct BVHNode
	{
		Sphere sphere{};
		std::unique_ptr<BVHNode> leftUPtr{ nullptr };
		std::unique_ptr<BVHNode> rightUPtr{ nullptr };

		std::unique_ptr<sdf::Object> objectUPtr{ nullptr };

		BVHNode(glm::vec3 origin, float radius)
			: sphere{ radius, origin }
		{
		}
	};

	class Scene
	{
	public:
		Scene() = default;
		virtual ~Scene() = default;

		Scene(const Scene&) = delete;
		Scene(Scene&&) noexcept = delete;
		Scene& operator=(const Scene&) = delete;
		Scene& operator=(Scene&&) noexcept = delete;

		//returns the distance and the number of steps
		HitRecord GetClosestHit(const glm::vec3& origin, const glm::vec3& direction, float minDistance, float maxDistance, int maxSteps) const;

		void Update(float ElapsedSec)
		{
			m_Camera.Update(ElapsedSec);
		}

		Camera const& GetCamera() const { return m_Camera; }

		void CreateBVHStructure();

		static bool m_UseAABBs;
	protected:		
		ObjectUPtrVec m_SDObjectUPtrVec{};
		static Camera m_Camera;

	private:
		std::unique_ptr<BVHNode> m_BVHRoot{ nullptr };

		std::pair<float, const sdf::Object*> GetDistanceToScene(const glm::vec3& point, HitRecord& outHitRecord) const;

		std::unique_ptr<BVHNode> CreateBVHNode(ObjectUPtrVec objects) const;
		std::pair<ObjectUPtrVec, ObjectUPtrVec> SplitObjects(ObjectUPtrVec objects) const;

		static glm::vec3 CalculateBVHOrigin(ObjectUPtrVec const& objects);
		static float CalculateBVHRadius(ObjectUPtrVec const& objects, glm::vec3 const& origin);
	};
	
	class SceneEasyComplexity final : public Scene
	{
	public:
		SceneEasyComplexity();
		~SceneEasyComplexity() override = default;

		SceneEasyComplexity(const SceneEasyComplexity&) = delete;
		SceneEasyComplexity(SceneEasyComplexity&&) noexcept = delete;
		SceneEasyComplexity& operator=(const SceneEasyComplexity&) = delete;
		SceneEasyComplexity& operator=(SceneEasyComplexity&&) noexcept = delete;
	};

	class SceneMediumComplexity final : public Scene
	{
	public:
		SceneMediumComplexity();
		~SceneMediumComplexity() override = default;

		SceneMediumComplexity(const SceneMediumComplexity&) = delete;
		SceneMediumComplexity(SceneMediumComplexity&&) noexcept = delete;
		SceneMediumComplexity& operator=(const SceneMediumComplexity&) = delete;
		SceneMediumComplexity& operator=(SceneMediumComplexity&&) noexcept = delete;
	};

	class SceneHighComplexity final : public Scene
	{
	public:
		SceneHighComplexity();
		~SceneHighComplexity() override = default;

		SceneHighComplexity(const SceneHighComplexity&) = delete;
		SceneHighComplexity(SceneHighComplexity&&) noexcept = delete;
		SceneHighComplexity& operator=(const SceneHighComplexity&) = delete;
		SceneHighComplexity& operator=(SceneHighComplexity&&) noexcept = delete;
	};
}
