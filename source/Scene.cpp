#include "Scene.h"

#include <algorithm>

#include "Material.h"

namespace VM
{
#pragma region Base Scene
	Scene::Scene()
		: m_Camera{ Vector3{ 0.f,0,-4.f }, 45 }
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

	std::pair<float,int> VM::Scene::GetClosestHit(const SDF::Ray& ray) const
	{
		float currentDistance{ 0.f };
		Vector3 const& rayOrigin{ ray.origin };

		int i{};
		for (i = 0; i < 10000; ++i)
		{
			Vector3 newPoint{ rayOrigin + ray.direction * currentDistance };
			// const float sinDist{ std::sin(currentDistance * 0.3f) };
			// const float sinTime{ std::sin(m_TotalTime * 0.4f) };
			// newPoint = Matrix::CreateRotationZ(currentDistance * sinTime * 0.14).TransformPoint(newPoint);
			// newPoint += Vector3{ 0.f, sinDist * sinTime * 10, 0.f } * 0.3f;
			float const distanceAbleToTravel{ GetDistanceToScene(newPoint) };
			currentDistance += distanceAbleToTravel;

			if (distanceAbleToTravel < 0.001f)
			{
				break;
			}
			if (currentDistance > 50.f)
			{
				break;
			}
		}
		return { currentDistance, i };
	}

#pragma region Scene Helpers

	SDF::Light* Scene::AddPointLight(const Vector3& origin, float intensity, const VM::ColorRGB& color)
	{
		SDF::Light l;
		l.origin = origin;
		l.intensity = intensity;
		l.color = color;
		l.type = SDF::LightType::Point;

		m_Lights.emplace_back(l);
		return &m_Lights.back();
	}

	SDF::Light* Scene::AddDirectionalLight(const Vector3& direction, float intensity, const VM::ColorRGB& color)
	{
		SDF::Light l;
		l.direction = direction;
		l.intensity = intensity;
		l.color = color;
		l.type = SDF::LightType::Directional;

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
		[&rayOrigin](const std::unique_ptr<SDF::Object>& a, const std::unique_ptr<SDF::Object>& b)
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
		m_SDObjectUPtrVec.emplace_back(std::make_unique<SDF::MandelBulb>());
	}
	
	void RayMarchingScene::Update(float ElapsedSec)
	{
		Scene::Update(ElapsedSec);

		for (auto const& object : m_SDObjectUPtrVec)
		{
			object->Update(ElapsedSec);
		}
	}
	
#pragma endregion BunnyScene
}