#pragma once
#include <memory>
#include <string>
#include <vector>
#include "DataTypes.h"
#include "Camera.h"
#include "SDFObjects.h"

namespace VM
{
	//Forward Declarations
	class GameTimer;
	class Material;
	struct SDPlane;
	struct SDSphere;
	struct Light;

	//Scene Base Class
	class Scene
	{
	public:
		Scene();
		virtual ~Scene();

		Scene(const Scene&) = delete;
		Scene(Scene&&) noexcept = delete;
		Scene& operator=(const Scene&) = delete;
		Scene& operator=(Scene&&) noexcept = delete;

		std::pair<float,int> GetClosestHit(const sdf::Ray& ray) const;

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

		sdf::Light* AddPointLight(const Vector3& origin, float intensity, const VM::ColorRGB& color);
		sdf::Light* AddDirectionalLight(const Vector3& direction, float intensity, const VM::ColorRGB& color);
		unsigned char AddMaterial(Material* pMaterial);
	private:
		float GetDistanceToScene(const Vector3& rayOrigin) const;
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
