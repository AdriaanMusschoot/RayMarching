#pragma once
#include <memory>
#include <string>
#include <vector>
#include "DataTypes.h"
#include "Camera.h"
#include "SDFObjects.h"
#include "Material.h"

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
		std::pair<float,int> GetClosestHit(const sdf::Ray& ray, float minDistance, float maxDistance, int maxSteps) const;

		virtual void Update(float ElapsedSec)
		{
			m_Camera.Update(ElapsedSec);
		}

		const Camera& GetCamera() const { return m_Camera; }

		const std::vector<sdf::Light>& GetLights() const { return m_Lights; }
		const std::vector<Material*>& GetMaterials() const { return m_Materials; }
	protected:
		std::string	m_SceneName;

		float m_TotalTime{};
		float m_DeltaTime{};
		
		std::vector<std::unique_ptr<sdf::Object>> m_SDObjectUPtrVec{};
		std::vector<Material*> m_Materials{};
		std::vector<sdf::Light> m_Lights{};
		Camera m_Camera{};

		sdf::Light* AddPointLight(const vm::Vector3& origin, float intensity, const ColorRGB& color);
		sdf::Light* AddDirectionalLight(const vm::Vector3& direction, float intensity, const ColorRGB& color);
		unsigned char AddMaterial(Material* pMaterial);
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
