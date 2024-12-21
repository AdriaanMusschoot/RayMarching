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
		float GetDistanceToScene(const glm::vec3& rayOrigin) const;
	};
	
	class PrismLinkScene final : public Scene
	{
	public:
		PrismLinkScene();
		~PrismLinkScene() override = default;

		PrismLinkScene(const PrismLinkScene&) = delete;
		PrismLinkScene(PrismLinkScene&&) noexcept = delete;
		PrismLinkScene& operator=(const PrismLinkScene&) = delete;
		PrismLinkScene& operator=(PrismLinkScene&&) noexcept = delete;
	};

	class PyramidMandelBulbScene final : public Scene
	{
	public:
		PyramidMandelBulbScene();
		~PyramidMandelBulbScene() override = default;

		PyramidMandelBulbScene(const PyramidMandelBulbScene&) = delete;
		PyramidMandelBulbScene(PyramidMandelBulbScene&&) noexcept = delete;
		PyramidMandelBulbScene& operator=(const PyramidMandelBulbScene&) = delete;
		PyramidMandelBulbScene& operator=(PyramidMandelBulbScene&&) noexcept = delete;
	};

	class OctahedronTorusScene final : public Scene
	{
	public:
		OctahedronTorusScene();
		~OctahedronTorusScene() override = default;

		OctahedronTorusScene(const OctahedronTorusScene&) = delete;
		OctahedronTorusScene(OctahedronTorusScene&&) noexcept = delete;
		OctahedronTorusScene& operator=(const OctahedronTorusScene&) = delete;
		OctahedronTorusScene& operator=(OctahedronTorusScene&&) noexcept = delete;
	};

}
