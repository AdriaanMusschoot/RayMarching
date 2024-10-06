#include "Scene.h"
#include "Utils.h"
#include "Material.h"

namespace dae {

#pragma region Base Scene
	//Initialize Scene with Default Solid Color Material (RED)
	Scene::Scene() :
		m_Materials({ new Material_SolidColor({1,0,0}) })
	{
		m_Camera.origin = { 0, 0, 1 };
		m_SphereGeometries.reserve(32);
		m_PlaneGeometries.reserve(32);
		m_TriangleMeshGeometries.reserve(32);
		m_Triangles.reserve(32);
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

	float dae::Scene::GetClosestHit(const Ray& ray) const
	{
		float currentDistance{ 0.f };
		Vector3 const& rayOrigin{ ray.origin };

		std::string steps{};
		
		for (int i = 0; i < 50; ++i)
		{
			Vector3 newPoint{ rayOrigin + ray.direction * currentDistance };
			float distanceAbleToTravel{	GetDistanceToSphere(Vector3{ 0, 0, 5 }, 1, newPoint) };
			currentDistance += distanceAbleToTravel;

			if (distanceAbleToTravel < 0.01)
			{
				break;
			}
		}
		return currentDistance;
	}


	bool Scene::DoesHit(const Ray& ray) const
	{
		for (int idx{}; idx < m_SphereGeometries.size(); ++idx)
		{
			if (GeometryUtils::HitTest_Sphere(m_SphereGeometries[idx], ray))
			{
				return true;
			}
		}
		for (int idx{}; idx < m_PlaneGeometries.size(); ++idx)
		{
			if (GeometryUtils::HitTest_Plane(m_PlaneGeometries[idx], ray))
			{
				return true;
			}
		}
		for (int idx{}; idx < m_TriangleMeshGeometries.size(); ++idx)
		{
			if (GeometryUtils::HitTest_TriangleMesh(m_TriangleMeshGeometries[idx], ray))
			{
				return true;
			}
		}
		for (int idx{}; idx < m_Triangles.size(); ++idx)
		{
			if (GeometryUtils::HitTest_Triangle(m_Triangles[idx], ray))
			{
				return true;
			}
		}
		return false;
	}

#pragma region Scene Helpers
	Sphere* Scene::AddSphere(const Vector3& origin, float radius, unsigned char materialIndex)
	{
		Sphere s;
		s.origin = origin;
		s.radius = radius;
		s.materialIndex = materialIndex;

		m_SphereGeometries.emplace_back(s);
		return &m_SphereGeometries.back();
	}

	Plane* Scene::AddPlane(const Vector3& origin, const Vector3& normal, unsigned char materialIndex)
	{
		Plane p;
		p.origin = origin;
		p.normal = normal;
		p.materialIndex = materialIndex;

		m_PlaneGeometries.emplace_back(p);
		return &m_PlaneGeometries.back();
	}

	TriangleMesh* Scene::AddTriangleMesh(TriangleCullMode cullMode, unsigned char materialIndex)
	{
		TriangleMesh m{};
		m.cullMode = cullMode;
		m.materialIndex = materialIndex;

		m_TriangleMeshGeometries.emplace_back(m);
		return &m_TriangleMeshGeometries.back();
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

	float Scene::GetDistanceToSphere(const Vector3& sphereOrigin, float sphereRadius, const Vector3& rayOrigin) const
	{
		return math::Distance(sphereOrigin, rayOrigin) - sphereRadius;
	}
#pragma endregion
#pragma endregion

#pragma region SCENE W1
	void Scene_W1::Initialize()
	{
		//default: Material id0 >> SolidColor Material (RED)
		constexpr unsigned char matId_Solid_Red = 0;
		const unsigned char matId_Solid_Blue = AddMaterial(new Material_SolidColor{ colors::Blue });

		const unsigned char matId_Solid_Yellow = AddMaterial(new Material_SolidColor{ colors::Yellow });
		const unsigned char matId_Solid_Green = AddMaterial(new Material_SolidColor{ colors::Green });
		const unsigned char matId_Solid_Magenta = AddMaterial(new Material_SolidColor{ colors::Magenta });

		//Spheres
		AddSphere({ -25.f, 0.f, 100.f }, 50.f, matId_Solid_Blue);
		AddSphere({ 25.f, 0.f, 100.f }, 50.f, matId_Solid_Red);

		//Plane
		AddPlane({ -75.f, 0.f, 0.f }, { 1.f, 0.f,0.f }, matId_Solid_Green);
		AddPlane({ 75.f, 0.f, 0.f }, { -1.f, 0.f,0.f }, matId_Solid_Green);
		AddPlane({ 0.f, -75.f, 0.f }, { 0.f, 1.f,0.f }, matId_Solid_Yellow);
		AddPlane({ 0.f, 75.f, 0.f }, { 0.f, -1.f,0.f }, matId_Solid_Yellow);
		AddPlane({ 0.f, 0.f, 125.f }, { 0.f, 0.f,-1.f }, matId_Solid_Magenta);
	}
#pragma region SCENE W2
	void Scene_W2::Initialize()
	{
		m_Camera.origin = { 0.f, 0.f, 0.f };

		// default: Material id0 >> SolidColor Material (RED)
		constexpr unsigned char matId_Solid_Red{ 0 };
		const unsigned char matId_Solid_Blue = AddMaterial(new Material_SolidColor{ colors::Blue });

		const unsigned char matId_Solid_Yellow = AddMaterial(new Material_SolidColor{ colors::Yellow });
		const unsigned char matId_Solid_Green = AddMaterial(new Material_SolidColor{ colors::Green });
		const unsigned char matId_Solid_Magenta = AddMaterial(new Material_SolidColor{ colors::Magenta });

		// Plane
		AddPlane({ -5.f, 0.f, 0.f }, { 1.f, 0.f, 0.f }, matId_Solid_Green);
		AddPlane({ 5.f, 0.f, 0.f }, { -1.f, 0.f, 0.f }, matId_Solid_Green);
		AddPlane({ 0.f, 0.f, 0.f }, { 0.f, 1.f, 0.f }, matId_Solid_Yellow);
		AddPlane({ 0.f, 10.f, 0.f }, { 0.f, -1.f, 0.f }, matId_Solid_Yellow);
		AddPlane({ 0.f, 0.f, 10.f }, { 0.f, 0.f, -1.f }, matId_Solid_Magenta);

		// Spheres
		AddSphere({ -1.75f, 1.f, 0.f }, .75f, matId_Solid_Red);
		AddSphere({ 0.f, 1.f, 0.f }, .75f, matId_Solid_Blue);
		AddSphere({ 1.75f, 1.f, 0.f }, .75f, matId_Solid_Red);
		AddSphere({ -1.75f, 3.f, 0.f }, .75f, matId_Solid_Blue);
		AddSphere({ 0.f, 3.f, 0.f }, .75f, matId_Solid_Red);
		AddSphere({ 1.75f, 3.f, 0.f }, .75f, matId_Solid_Blue);

		// Light
		AddPointLight({ 0.f, 5.f, -5.f }, 70.f, colors::White);
	}
#pragma endregion
#pragma region SCENE W3
	void Scene_W3::Initialize()
	{
		sceneName = "Week 3";
		m_Camera.origin = { 0,0,0 };
		m_Camera.fovAngle = 45.f;

		const auto matCT_GrayRoughMetal = AddMaterial(new Material_CookTorrence({ .972f, .960f, .915f }, 1.f, 1.f));
		const auto matCT_GrayMediumMetal = AddMaterial(new Material_CookTorrence({ .972f, .960f, .915f }, 1.f, .6f));
		const auto matCT_GraySmoothMetal = AddMaterial(new Material_CookTorrence({ .972f, .960f, .915f }, 1.f, .1f));
		const auto matCT_GrayRoughPlastic = AddMaterial(new Material_CookTorrence({ .75f, .75f, .75f }, .0f, 1.f));
		const auto matCT_GrayMediumPlastic = AddMaterial(new Material_CookTorrence({ .75f, .75f, .75f }, .0f, .6f));
		const auto matCT_GraySmoothPlastic = AddMaterial(new Material_CookTorrence({ .75f, .75f, .75f }, .0f, .1f));

		const auto matLambert_GrayBlue = AddMaterial(new Material_Lambert({ .49f, 0.57f, 0.57f }, 1.f));
		const auto matLambert_White = AddMaterial(new Material_Lambert(colors::White, 1.f));

		AddPlane(Vector3{ 0.f, 0.f, 10.f }, Vector3{ 0.f, 0.f, -1.f }, matLambert_GrayBlue); //BACK
		AddPlane(Vector3{ 0.f, 0.f, 0.f }, Vector3{ 0.f, 1.f, 0.f }, matLambert_GrayBlue); //BOTTOM
		AddPlane(Vector3{ 0.f, 10.f, 0.f }, Vector3{ 0.f, -1.f, 0.f }, matLambert_GrayBlue); //TOP
		AddPlane(Vector3{ 5.f, 0.f, 0.f }, Vector3{ -1.f, 0.f, 0.f }, matLambert_GrayBlue); //RIGHT
		AddPlane(Vector3{ -5.f, 0.f, 0.f }, Vector3{ 1.f, 0.f, 0.f }, matLambert_GrayBlue); //LEFT

		AddSphere(Vector3{ -1.75f, 1.f, 0.f }, .75f, matCT_GrayRoughMetal);
		AddSphere(Vector3{ 0.f, 1.f, 0.f }, .75f, matCT_GrayMediumMetal);
		AddSphere(Vector3{ 1.75f, 1.f, 0.f }, .75f, matCT_GraySmoothMetal);
		AddSphere(Vector3{ -1.75f, 3.f, 0.f }, .75f, matCT_GrayRoughPlastic);
		AddSphere(Vector3{ 0.f, 3.f, 0.f }, .75f, matCT_GrayMediumPlastic);
		AddSphere(Vector3{ 1.75f, 3.f, 0.f }, .75f, matCT_GraySmoothPlastic);



		AddPointLight(Vector3{ 0.f, 5.f, 5.f }, 50.f, ColorRGB{ 1.f, .61f, .45f }); //Backlight
		AddPointLight(Vector3{ -2.5f, 5.f, -5.f }, 70.f, ColorRGB{ 1.f, .8f, .45f }); //Front Light Left
		AddPointLight(Vector3{ 2.5f, 2.5f, -5.f }, 50.f, ColorRGB{ .34f, .47f, .68f });
	}
#pragma endregion

#pragma region BunnyScene
	void Scene_W4BunnyScene::Initialize()
	{
		m_Camera.origin = { 0.f,0,1.f };
		m_Camera.fovAngle = 45.f;

		//Materials
		const auto matLambert_GrayBlue = AddMaterial(new Material_Lambert({ .49f, 0.57f, 0.57f }, 1.f));
		const auto matLambert_White = AddMaterial(new Material_Lambert(colors::White, 1.f));

		//Planes
		AddPlane(Vector3{ 0.f, 0.f, 10.f }, Vector3{ 0.f, 0.f, -1.f }, matLambert_GrayBlue); //BACK
		AddPlane(Vector3{ 0.f, 0.f, 0.f }, Vector3{ 0.f, 1.f, 0.f }, matLambert_GrayBlue); //BOTTOM
		AddPlane(Vector3{ 0.f, 10.f, 0.f }, Vector3{ 0.f, -1.f, 0.f }, matLambert_GrayBlue); //TOP
		AddPlane(Vector3{ 5.f, 0.f, 0.f }, Vector3{ -1.f, 0.f, 0.f }, matLambert_GrayBlue); //RIGHT
		AddPlane(Vector3{ -5.f, 0.f, 0.f }, Vector3{ 1.f, 0.f, 0.f }, matLambert_GrayBlue); //LEFT

		//OBJ
		AddTriangleMesh(TriangleCullMode::BackFaceCulling, matLambert_White);
		Utils::ParseOBJ("Resources/lowpoly_bunny.obj", m_TriangleMeshGeometries[0].positions,	m_TriangleMeshGeometries[0].normals, m_TriangleMeshGeometries[0].indices);
		m_TriangleMeshGeometries[0].cullMode = TriangleCullMode::BackFaceCulling;

		m_TriangleMeshGeometries[0].Scale({ 2, 2, 2 });

		m_TriangleMeshGeometries[0].UpdateAABB();
		m_TriangleMeshGeometries[0].UpdateTransforms();

		//Light
		AddPointLight(Vector3{ 0.f, 5.f, 5.f }, 50.f, ColorRGB{ 1.f, .61f, .45f }); //Backlight
		AddPointLight(Vector3{ -2.5f, 5.f, -5.f }, 70.f, ColorRGB{ 1.f, .8f, .45f }); //Front Light Left
		AddPointLight(Vector3{ 2.5f, 2.5f, -5.f }, 50.f, ColorRGB{ .34f, .47f, .68f });
	}
	void Scene_W4BunnyScene::Update(Timer* pTimer)
	{
		Scene::Update(pTimer);

		const float angle{ cos(pTimer->GetTotal() + 1) / 2.f * PI_2 };
		for (int idx{}; idx < m_TriangleMeshGeometries.size(); ++idx)
		{
			m_TriangleMeshGeometries[idx].RotateY(angle);
			m_TriangleMeshGeometries[idx].UpdateTransforms();
		}
	}
#pragma endregion BunnyScene

#pragma region ReferenceScene
	void Scene_W4ReferenceScene::Initialize()
	{
		sceneName = "Reference Scene";
		m_Camera.origin = { 0,0,-0 };
		m_Camera.fovAngle = 45.f;

		const auto matCT_GrayRoughMetal = AddMaterial(new Material_CookTorrence({ .972f, .960f, .915f }, 1.f, 1.f));
		const auto matCT_GrayMediumMetal = AddMaterial(new Material_CookTorrence({ .972f, .960f, .915f }, 1.f, .6f));
		const auto matCT_GraySmoothMetal = AddMaterial(new Material_CookTorrence({ .972f, .960f, .915f }, 1.f, .1f));
		const auto matCT_GrayRoughPlastic = AddMaterial(new Material_CookTorrence({ .75f, .75f, .75f }, .0f, 1.f));
		const auto matCT_GrayMediumPlastic = AddMaterial(new Material_CookTorrence({ .75f, .75f, .75f }, .0f, .6f));
		const auto matCT_GraySmoothPlastic = AddMaterial(new Material_CookTorrence({ .75f, .75f, .75f }, .0f, .1f));

		const auto matLambert_GrayBlue = AddMaterial(new Material_Lambert({ .49f, 0.57f, 0.57f }, 1.f));
		const auto matLambert_White = AddMaterial(new Material_Lambert(colors::White, 1.f));

		AddPlane(Vector3{ 0.f, 0.f, 10.f }, Vector3{ 0.f, 0.f, -1.f }, matLambert_GrayBlue); //BACK
		AddPlane(Vector3{ 0.f, 0.f, 0.f }, Vector3{ 0.f, 1.f, 0.f }, matLambert_GrayBlue); //BOTTOM
		AddPlane(Vector3{ 0.f, 10.f, 0.f }, Vector3{ 0.f, -1.f, 0.f }, matLambert_GrayBlue); //TOP
		AddPlane(Vector3{ 5.f, 0.f, 0.f }, Vector3{ -1.f, 0.f, 0.f }, matLambert_GrayBlue); //RIGHT
		AddPlane(Vector3{ -5.f, 0.f, 0.f }, Vector3{ 1.f, 0.f, 0.f }, matLambert_GrayBlue); //LEFT

		AddSphere(Vector3{ -1.75f, 1.f, 0.f }, .75f, matCT_GrayRoughMetal);
		AddSphere(Vector3{ 0.f, 1.f, 0.f }, .75f, matCT_GrayMediumMetal);
		AddSphere(Vector3{ 1.75f, 1.f, 0.f }, .75f, matCT_GraySmoothMetal);
		AddSphere(Vector3{ -1.75f, 3.f, 0.f }, .75f, matCT_GrayRoughPlastic);
		AddSphere(Vector3{ 0.f, 3.f, 0.f }, .75f, matCT_GrayMediumPlastic);
		AddSphere(Vector3{ 1.75f, 3.f, 0.f }, .75f, matCT_GraySmoothPlastic);

		//CW Winding Order!
		const Triangle baseTriangle = { Vector3(-.75f, 1.5f, 0.f), Vector3(.75f, 0.f, 0.f), Vector3(-.75f, 0.f, 0.f) };

		AddTriangleMesh(TriangleCullMode::BackFaceCulling, matLambert_White);
		m_TriangleMeshGeometries[0].AppendTriangle(baseTriangle, true);
		m_TriangleMeshGeometries[0].Translate({ -1.75f,4.5f,0.f });
		m_TriangleMeshGeometries[0].UpdateAABB();
		m_TriangleMeshGeometries[0].UpdateTransforms();

		AddTriangleMesh(TriangleCullMode::FrontFaceCulling, matLambert_White);
		m_TriangleMeshGeometries[1].AppendTriangle(baseTriangle, true);
		m_TriangleMeshGeometries[1].Translate({ 0.f,4.5f,0.f });
		m_TriangleMeshGeometries[1].UpdateAABB();
		m_TriangleMeshGeometries[1].UpdateTransforms();

		AddTriangleMesh(TriangleCullMode::NoCulling, matLambert_White);
		m_TriangleMeshGeometries[2].AppendTriangle(baseTriangle, true);
		m_TriangleMeshGeometries[2].Translate({ 1.75f,4.5f,0.f });
		m_TriangleMeshGeometries[2].UpdateAABB();
		m_TriangleMeshGeometries[2].UpdateTransforms();

		AddPointLight(Vector3{ 0.f, 5.f, 5.f }, 50.f, ColorRGB{ 1.f, .61f, .45f }); //Backlight
		AddPointLight(Vector3{ -2.5f, 5.f, -5.f }, 70.f, ColorRGB{ 1.f, .8f, .45f }); //Front Light Left
		AddPointLight(Vector3{ 2.5f, 2.5f, -5.f }, 50.f, ColorRGB{ .34f, .47f, .68f });
	}
	void Scene_W4ReferenceScene::Update(Timer* pTimer)
	{
		Scene::Update(pTimer);

		const float angle{ cos(pTimer->GetTotal() + 1) / 2.f * PI_2 };
		for (int idx{}; idx < m_TriangleMeshGeometries.size(); ++idx)
		{
			m_TriangleMeshGeometries[idx].RotateY(angle);
			m_TriangleMeshGeometries[idx].UpdateTransforms();
		}
	}
#pragma endregion ReferenceScene

#pragma region F1Scene
void Scene_W4F1Scene::Initialize()
{
	m_Camera.origin = { 0.f,5,-40 };
	m_Camera.fovAngle = 45;

	//Materials
	const auto matCT_f1Car = AddMaterial(new Material_CookTorrence({ 0.4, 0, 0 }, 0.1, 0.5f));
	const auto matCT_GraySmoothPlastic = AddMaterial(new Material_CookTorrence({ .75f, .75f, .75f }, .0f, .1f));

	//Planes
	//AddPlane(Vector3{ 0.f, 0.f, 10.f }, Vector3{ 0.f, 0.f, -1.f }, matCT_GraySmoothPlastic); //BACK
	AddPlane(Vector3{ 0.f, 0.f, 0.f }, Vector3{ 0.f, 1.f, 0.f }, matCT_GraySmoothPlastic); //BOTTOM
	//AddPlane(Vector3{ 0.f, 10.f, 0.f }, Vector3{ 0.f, -1.f, 0.f }, matCT_GraySmoothPlastic); //TOP
	AddPlane(Vector3{ 5.f, 0.f, 0.f }, Vector3{ -1.f, 0.f, 0.f }, matCT_GraySmoothPlastic); //RIGHT
	AddPlane(Vector3{ -5.f, 0.f, 0.f }, Vector3{ 1.f, 0.f, 0.f }, matCT_GraySmoothPlastic); //LEFT

	//OBJ
	AddTriangleMesh(TriangleCullMode::BackFaceCulling, matCT_f1Car);
	Utils::ParseOBJ("Resources/complicated_car.obj", m_TriangleMeshGeometries[0].positions, m_TriangleMeshGeometries[0].normals, m_TriangleMeshGeometries[0].indices);
	m_TriangleMeshGeometries[0].cullMode = TriangleCullMode::BackFaceCulling;

	m_TriangleMeshGeometries[0].Scale({ 2, 2, 2 });
	m_TriangleMeshGeometries[0].Rotate(0, 190 * TO_RADIANS, 0);
	m_TriangleMeshGeometries[0].UpdateAABB();
	m_TriangleMeshGeometries[0].UpdateTransforms();

	//Light
	AddPointLight(Vector3{ 0.f, 5.f, 10.f }, 100.f, ColorRGB{ 1.f, .61f, .45f }); //Backlight
	AddPointLight(Vector3{ -2.5f, 5.f, -10.f }, 100.f, ColorRGB{ 1.f, .8f, .45f }); //Front Light Left
	AddPointLight(Vector3{ 2.5f, 2.5f, 0.f }, 100.f, ColorRGB{ .34f, .47f, .68f });
}
void Scene_W4F1Scene::Update(Timer* pTimer)
{
	Scene::Update(pTimer);

	const float angle{ cos(pTimer->GetTotal() + 1) / 2.f * (PI) };
	if (!m_FlippedBackwards && angle >= 1.56)
	{
		m_FlippedBackwards = true;
		m_FlippedForwards = false;
		m_TriangleMeshGeometries[0].RotateY(190 * TO_RADIANS);
	}
	if (!m_FlippedForwards && angle <= -1.56)
	{
		m_FlippedForwards = true;
		m_FlippedBackwards = false;
		m_TriangleMeshGeometries[0].RotateY(10 * TO_RADIANS);
	}

	m_TriangleMeshGeometries[0].Translate({ 0, 0, angle * 7 });
	m_TriangleMeshGeometries[0].UpdateTransforms();
}
#pragma endregion F1Scene
}
#pragma endregion

