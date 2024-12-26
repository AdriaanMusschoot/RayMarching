#include "Scenes.h"
#include "SDFObjects.h"

sdf::SceneEasyComplexity::SceneEasyComplexity()
{
	m_SDObjectUPtrVec.emplace_back(std::make_unique<sdf::Link>(0.5f, 0.5f, 0.1f, glm::vec3{ 1.f, 0.f, 0.f }));
	m_SDObjectUPtrVec.emplace_back(std::make_unique<sdf::Octahedron>(1.f, glm::vec3{ -1.f, 0.f, 0.f }));
	CreateBVHStructure();
}

sdf::SceneMediumComplexity::SceneMediumComplexity()
{
	m_SDObjectUPtrVec.emplace_back(std::make_unique<sdf::BoxFrame>(glm::vec3{ 0.7f, 0.7f, 0.7f }, 0.05f, glm::vec3{ 2.f, 1.f, 0.f }));
	m_SDObjectUPtrVec.emplace_back(std::make_unique<sdf::HexagonalPrism>(0.7f, 0.7f, glm::vec3{ -2.f, 1.f, 0.f }));
	m_SDObjectUPtrVec.emplace_back(std::make_unique<sdf::Pyramid>(2.f, glm::vec3{ 0.f, -1.8f, 0.f }));
	CreateBVHStructure();
}

sdf::SceneHighComplexity::SceneHighComplexity()
{
	m_SDObjectUPtrVec.emplace_back(std::make_unique<sdf::MandelBulb>(glm::vec3{ 2.f, 0.f, 0.f }));
	m_SDObjectUPtrVec.emplace_back(std::make_unique<sdf::MandelBulb>(glm::vec3{ -2.f, 0.f, 0.f }));
	CreateBVHStructure();
}