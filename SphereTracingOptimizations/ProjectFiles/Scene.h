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

		static bool m_UseAABBs;
	protected:		
		std::vector<std::unique_ptr<sdf::Object>> m_SDObjectUPtrVec{};
		static Camera m_Camera;

	private:
		std::pair<float, const sdf::Object*> GetDistanceToScene(const glm::vec3& point, HitRecord& outHitRecord) const;
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
