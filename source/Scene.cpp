#include "Scene.h"
#include "Material.h"

namespace geo
{
#pragma region Base Scene
	Scene::Scene()
	{
		m_Materials.reserve(32);
		m_Lights.reserve(32);
	}

	Scene::~Scene()
	{
		for (auto& pMaterial : m_Materials)
		{
			delete pMaterial;
			pMaterial = nullptr;
		}

		m_Materials.clear();
	}

	float geo::Scene::GetClosestHit(const Ray& ray) const
	{
		float currentDistance{ 0.f };
		Vector3 const& rayOrigin{ ray.origin };

		for (int i = 0; i < 50; ++i)
		{
			Vector3 newPoint{ rayOrigin + ray.direction * currentDistance };
			float distanceAbleToTravel{	GetDistanceToSphere(newPoint) };
			currentDistance += distanceAbleToTravel;

			if (distanceAbleToTravel < 0.01)
			{
				break;
			}
		}
		return currentDistance;
	}

#pragma region Scene Helpers
	void Scene::AddSphere(const Vector3& origin, float radius, unsigned char materialIndex)
	{
		m_SDObjectUPtrVec.emplace_back(std::make_unique<SDSphere>(origin, radius));
	}

	void Scene::AddPlane(const Vector3& origin, const Vector3& normal, unsigned char materialIndex)
	{
		m_SDObjectUPtrVec.emplace_back(std::make_unique<SDPlane>(origin, normal));
	}

	void Scene::AddBox(const Vector3& origin, const Vector3& boxExtent, unsigned char materialIndex)
	{
		m_SDObjectUPtrVec.emplace_back(std::make_unique<SDBox>(origin, boxExtent));
	}

	Light* Scene::AddPointLight(const Vector3& origin, float intensity, const ColorRGB& color)
	{
		Light l;
		l.origin = origin;
		l.intensity = intensity;
		l.color = color;
		l.type = LightType::Point;

		m_Lights.emplace_back(l);
		return &m_Lights.back();
	}

	Light* Scene::AddDirectionalLight(const Vector3& direction, float intensity, const ColorRGB& color)
	{
		Light l;
		l.direction = direction;
		l.intensity = intensity;
		l.color = color;
		l.type = LightType::Directional;

		m_Lights.emplace_back(l);
		return &m_Lights.back();
	}

	unsigned char Scene::AddMaterial(Material* pMaterial)
	{
		m_Materials.push_back(pMaterial);
		return static_cast<unsigned char>(m_Materials.size() - 1);
	}

	float Scene::GetDistanceToSphere(const Vector3& rayOrigin) const
	{
		auto minDistanceIt =
			std::min_element(m_SDObjectUPtrVec.begin(), m_SDObjectUPtrVec.end(),
		[&rayOrigin](const std::unique_ptr<ISDObject>& a, const std::unique_ptr<ISDObject>& b)
			{
				return a->GetDistance(rayOrigin) < b->GetDistance(rayOrigin);
			});

		if (minDistanceIt != m_SDObjectUPtrVec.end())
		{
			return (*minDistanceIt)->GetDistance(rayOrigin);
		}
		return -1;
	}

#pragma endregion
#pragma endregion
	
#pragma region BunnyScene
	RayMarchingScene::RayMarchingScene()
	{
		m_Camera.origin = { 0.f,0,1.f };
		m_Camera.fovAngle = 45.f;

		AddSphere(Vector3{ 0, 0, 5 }, 1);
		AddPlane(Vector3{ 0, -1, 0 }, Vector3{ 0, 1, 0 });
		AddBox(Vector3{ 0, 2, 5 }, Vector3{ 2, 1, 1 });
	}
	
	void RayMarchingScene::Update(Timer* pTimer)
	{
		Scene::Update(pTimer);
	}
	
#pragma endregion BunnyScene
}