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
		Scene();
		virtual ~Scene();

		Scene(const Scene&) = delete;
		Scene(Scene&&) noexcept = delete;
		Scene& operator=(const Scene&) = delete;
		Scene& operator=(Scene&&) noexcept = delete;

		//returns the distance and the number of steps
		std::pair<float,int> GetClosestHit(const vm::Vector3& origin, const vm::Vector3& direction, float minDistance, float maxDistance, int maxSteps) const;

		virtual void Update(float ElapsedSec)
		{
			m_Camera.Update(ElapsedSec);
		}

		const Camera& GetCamera() const { return m_Camera; }

	protected:
		std::string	m_SceneName;

		float m_TotalTime{};
		float m_DeltaTime{};
		
		std::vector<std::unique_ptr<sdf::Object>> m_SDObjectUPtrVec{};
		Camera m_Camera{};

	private:
		float GetDistanceToScene(const vm::Vector3& rayOrigin) const;
	};
	
	class RayMarchingScene final : public Scene
	{
	public:
		RayMarchingScene();
		~RayMarchingScene() override = default;

		RayMarchingScene(const RayMarchingScene&) = delete;
		RayMarchingScene(RayMarchingScene&&) noexcept = delete;
		RayMarchingScene& operator=(const RayMarchingScene&) = delete;
		RayMarchingScene& operator=(RayMarchingScene&&) noexcept = delete;

		void Update(float ElapsedSec) override;
	};
	
}
