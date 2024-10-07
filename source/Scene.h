#pragma once
#include <string>
#include <vector>

#include "Math.h"
#include "DataTypes.h"
#include "Camera.h"

namespace geo
{
	//Forward Declarations
	class Timer;
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

		float GetClosestHit(const Ray& ray) const;

		virtual void Update(geo::Timer* pTimer)
		{
			m_Camera.Update(pTimer);
		}

		Camera& GetCamera() { return m_Camera; }

		const std::vector<Light>& GetLights() const { return m_Lights; }
		const std::vector<Material*>& GetMaterials() const { return m_Materials; }
	protected:
		std::string	m_SceneName;

		
		std::vector<std::unique_ptr<ISDObject>> m_SDObjectUPtrVec{};
		std::vector<Material*> m_Materials{};
		std::vector<Light> m_Lights{};
		Camera m_Camera{};

		void AddSphere(const Vector3& origin, float radius, unsigned char materialIndex = 0);
		void AddPlane(const Vector3& origin, const Vector3& normal, unsigned char materialIndex = 0);
		void AddBox(const Vector3& origin, const Vector3& boxExtent, unsigned char materialIndex = 0);
		
		Light* AddPointLight(const Vector3& origin, float intensity, const ColorRGB& color);
		Light* AddDirectionalLight(const Vector3& direction, float intensity, const ColorRGB& color);
		unsigned char AddMaterial(Material* pMaterial);
	private:
		float GetDistanceToSphere(const Vector3& rayOrigin) const;
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

		void Update(Timer* pTimer) override;
	};
	
}
