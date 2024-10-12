#include "Scene.h"
#include "Material.h"

namespace geo
{
#pragma region Base Scene
	Scene::Scene()
		: m_Camera{ Vector3{ 0.f,0,0.f }, 45 }
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

	std::pair<float,int> geo::Scene::GetClosestHit(const Ray& ray) const
	{
		float currentDistance{ 0.f };
		Vector3 const& rayOrigin{ ray.origin };

		int i{};
		for (i = 0; i < 80; ++i)
		{
			Vector3 const newPoint{ rayOrigin + ray.direction * currentDistance };
			float const distanceAbleToTravel{ GetDistanceToScene(newPoint) };
			currentDistance += distanceAbleToTravel;

			if (distanceAbleToTravel < 0.001f)
			{
				break;
			}
			if (currentDistance > 100.f)
			{
				break;
			}
		}
		return { currentDistance, i };
	}

#pragma region Scene Helpers

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

	float Scene::GetDistanceToScene(const Vector3& rayOrigin) const
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
		m_SDObjectUPtrVec.emplace_back(std::make_unique<SDSmoothSphereBoxPlane>
			(
				SDBox{ Vector3{0, 0, 0 }, Vector3{ 0.1f, 0.1f, 0.1f } },
				SDSphere{ Vector3{ 0, 0, 5 }, 0.4f },
				SDPlane{ Vector3{ 0, -0.6, 0 }, Vector3{ 0, 1, 0 } },
				0.5
			));
	}
	
	void RayMarchingScene::Update(Timer* pTimer)
	{
		Scene::Update(pTimer);

		for (auto const& object : m_SDObjectUPtrVec)
		{
			object->Update(pTimer->GetElapsed());
		}
	}
	
#pragma endregion BunnyScene
}