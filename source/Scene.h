#pragma once
#include <string>
#include <vector>

#include "Math.h"
#include "DataTypes.h"
#include "Camera.h"

namespace dae
{
	//Forward Declarations
	class Timer;
	class Material;
	struct Plane;
	struct Sphere;
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

		virtual void Initialize() = 0;
		virtual void Update(dae::Timer* pTimer)
		{
			m_Camera.Update(pTimer);
		}

		Camera& GetCamera() { return m_Camera; }
		float GetClosestHit(const Ray& ray) const;
		bool DoesHit(const Ray& ray) const;

		const std::vector<Plane>& GetPlaneGeometries() const { return m_PlaneGeometries; }
		const std::vector<Sphere>& GetSphereGeometries() const { return m_SphereGeometries; }
		const std::vector<Light>& GetLights() const { return m_Lights; }
		const std::vector<Material*>& GetMaterials() const { return m_Materials; }

	protected:
		std::string	sceneName;

		std::vector<Plane> m_PlaneGeometries{};
		std::vector<Sphere> m_SphereGeometries{};
		std::vector<TriangleMesh> m_TriangleMeshGeometries{};
		std::vector<Light> m_Lights{};
		std::vector<Material*> m_Materials{};
		std::vector<Triangle> m_Triangles{};
		Camera m_Camera{};

		Sphere* AddSphere(const Vector3& origin, float radius, unsigned char materialIndex = 0);
		Plane* AddPlane(const Vector3& origin, const Vector3& normal, unsigned char materialIndex = 0);
		TriangleMesh* AddTriangleMesh(TriangleCullMode cullMode, unsigned char materialIndex = 0);

		Light* AddPointLight(const Vector3& origin, float intensity, const ColorRGB& color);
		Light* AddDirectionalLight(const Vector3& direction, float intensity, const ColorRGB& color);
		unsigned char AddMaterial(Material* pMaterial);
	private:
		float GetDistanceToSphere(const Vector3& sphereOrigin, float sphereRadius, const Vector3& rayOrigin) const;
	};

	//+++++++++++++++++++++++++++++++++++++++++
	//WEEK 1 Test Scene
	class Scene_W1 final : public Scene
	{
	public:
		Scene_W1() = default;
		~Scene_W1() override = default;

		Scene_W1(const Scene_W1&) = delete;
		Scene_W1(Scene_W1&&) noexcept = delete;
		Scene_W1& operator=(const Scene_W1&) = delete;
		Scene_W1& operator=(Scene_W1&&) noexcept = delete;

		void Initialize() override;
	};
	//+++++++++++++++++++++++++++++++++++++++++
	//WEEK 2 Test Scene
	class Scene_W2 final : public Scene
	{
	public:
		Scene_W2() = default;
		~Scene_W2() override = default;

		Scene_W2(const Scene_W2&) = delete;
		Scene_W2(Scene_W2&&) noexcept = delete;
		Scene_W2& operator=(const Scene_W2&) = delete;
		Scene_W2& operator=(Scene_W2&&) noexcept = delete;

		void Initialize() override;
	};
	//+++++++++++++++++++++++++++++++++++++++++
	//WEEK 3 Test Scene
	class Scene_W3 final : public Scene
	{
	public:
		Scene_W3() = default;
		~Scene_W3() override = default;

		Scene_W3(const Scene_W3&) = delete;
		Scene_W3(Scene_W3&&) noexcept = delete;
		Scene_W3& operator=(const Scene_W3&) = delete;
		Scene_W3& operator=(Scene_W3&&) noexcept = delete;

		void Initialize() override;
	};
	//+++++++++++++++++++++++++++++++++++++++++
	//WEEK 4 Test Scene
	class Scene_W4BunnyScene final : public Scene
	{
	public:
		Scene_W4BunnyScene() = default;
		~Scene_W4BunnyScene() override = default;

		Scene_W4BunnyScene(const Scene_W4BunnyScene&) = delete;
		Scene_W4BunnyScene(Scene_W4BunnyScene&&) noexcept = delete;
		Scene_W4BunnyScene& operator=(const Scene_W4BunnyScene&) = delete;
		Scene_W4BunnyScene& operator=(Scene_W4BunnyScene&&) noexcept = delete;

		void Initialize() override;
		void Update(Timer* pTimer) override;
	};
	//+++++++++++++++++++++++++++++++++++++++++
	//WEEK 4 Test Scene
	class Scene_W4ReferenceScene final : public Scene
	{
	public:
		Scene_W4ReferenceScene() = default;
		~Scene_W4ReferenceScene() override = default;

		Scene_W4ReferenceScene(const Scene_W4ReferenceScene&) = delete;
		Scene_W4ReferenceScene(Scene_W4ReferenceScene&&) noexcept = delete;
		Scene_W4ReferenceScene& operator=(const Scene_W4ReferenceScene&) = delete;
		Scene_W4ReferenceScene& operator=(Scene_W4ReferenceScene&&) noexcept = delete;

		void Initialize() override;
		void Update(Timer* pTimer) override;
	};
	//+++++++++++++++++++++++++++++++++++++++++
	//WEEK 4 Test Scene
	class Scene_W4F1Scene final : public Scene
	{
	public:
		Scene_W4F1Scene() = default;
		~Scene_W4F1Scene() override = default;

		Scene_W4F1Scene(const Scene_W4F1Scene&) = delete;
		Scene_W4F1Scene(Scene_W4F1Scene&&) noexcept = delete;
		Scene_W4F1Scene& operator=(const Scene_W4F1Scene&) = delete;
		Scene_W4F1Scene& operator=(Scene_W4F1Scene&&) noexcept = delete;

		void Initialize() override;
		void Update(Timer* pTimer) override;

	private:
		bool m_FlippedForwards{ false };
		bool m_FlippedBackwards{ false };
	};
}
