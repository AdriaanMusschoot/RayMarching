#include "Scenes.h"

#include <numbers>
#include <random>

#include "SDFObjects.h"

sdf::SceneEasyComplexity::SceneEasyComplexity()
{
    constexpr float spacing{ 1.5f };
    m_SDObjectUPtrVec.emplace_back(std::make_unique<sdf::Link>(0.8f, 0.8f, 0.2f, glm::vec3{ 0.f, 0.f, spacing }));
    m_SDObjectUPtrVec.emplace_back(std::make_unique<sdf::Link>(0.4f, 0.4f, 0.1f, glm::vec3{ spacing, spacing, 0.f }));
    m_SDObjectUPtrVec.emplace_back(std::make_unique<sdf::Link>(0.2f, 0.2f, 0.05f, glm::vec3{ -spacing, -spacing, -spacing }));
    m_SDObjectUPtrVec.emplace_back(std::make_unique<sdf::Link>(0.3f, 0.3f, 0.08f, glm::vec3{ spacing, -spacing, spacing }));
    m_SDObjectUPtrVec.emplace_back(std::make_unique<sdf::Link>(0.5f, 0.5f, 0.13f, glm::vec3{ -spacing, spacing, 0.f }));
    
    m_SDObjectUPtrVec.emplace_back(std::make_unique<sdf::Octahedron>(0.8f, glm::vec3{ 0.f, 0.f, -spacing }));
    m_SDObjectUPtrVec.emplace_back(std::make_unique<sdf::Octahedron>(1.f, glm::vec3{ spacing, spacing, spacing }));
    m_SDObjectUPtrVec.emplace_back(std::make_unique<sdf::Octahedron>(0.6f, glm::vec3{ -spacing, -spacing, 0.f}));
    m_SDObjectUPtrVec.emplace_back(std::make_unique<sdf::Octahedron>(0.7f, glm::vec3{ spacing, -spacing, -spacing }));
    m_SDObjectUPtrVec.emplace_back(std::make_unique<sdf::Octahedron>(1.1f, glm::vec3{ -spacing, spacing, spacing }));
    
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