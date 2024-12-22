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
		std::pair<float,int> GetClosestHit(const glm::vec3& origin, const glm::vec3& direction, float minDistance, float maxDistance, int maxSteps) const;

		void Update(float ElapsedSec)
		{
			m_Camera.Update(ElapsedSec);
		}

		const Camera& GetCamera() const { return m_Camera; }

	protected:
		float m_TotalTime{};
		float m_DeltaTime{};
		
		std::vector<std::unique_ptr<sdf::Object>> m_SDObjectUPtrVec{};
		static Camera m_Camera;

	private:
		float GetDistanceToScene(const glm::vec3& point) const;
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

	class SceneMaxComplexity final : public Scene
	{
	public:
		SceneMaxComplexity();
		~SceneMaxComplexity() override = default;

		SceneMaxComplexity(const SceneMaxComplexity&) = delete;
		SceneMaxComplexity(SceneMaxComplexity&&) noexcept = delete;
		SceneMaxComplexity& operator=(const SceneMaxComplexity&) = delete;
		SceneMaxComplexity& operator=(SceneMaxComplexity&&) noexcept = delete;
	};
}
