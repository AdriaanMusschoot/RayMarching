#include "Scenes.h"

#include <numbers>
#include <random>

#include "SDFObjects.h"

sdf::SceneEasyComplexity::SceneEasyComplexity()
{
    int constexpr numObjects{ 4 };
    glm::vec3 constexpr boxMinLink{ -3.f, -3.f, -3.f };
    glm::vec3 constexpr boxMaxLink{ 3.f, 3.f, 3.f };
    glm::vec3 constexpr boxMinOctahedron{ -3.f, -3.f, -3.f };
    glm::vec3 constexpr boxMaxOctahedron{ 3.f, 3.f, 3.f };

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> disLinkX(boxMinLink.x, boxMaxLink.x);
    std::uniform_real_distribution<float> disLinkY(boxMinLink.y, boxMaxLink.y);
    std::uniform_real_distribution<float> disLinkZ(boxMinLink.z, boxMaxLink.z);
    std::uniform_real_distribution<float> disOctahedronX(boxMinOctahedron.x, boxMaxOctahedron.x);
    std::uniform_real_distribution<float> disOctahedronY(boxMinOctahedron.y, boxMaxOctahedron.y);
    std::uniform_real_distribution<float> disOctahedronZ(boxMinOctahedron.z, boxMaxOctahedron.z);

    for (int i = 0; i < numObjects; ++i)
    {
        float xLink = disLinkX(gen);
        float yLink = disLinkY(gen);
        float zLink = disLinkZ(gen);

        float xOctahedron = disOctahedronX(gen);
        float yOctahedron = disOctahedronY(gen);
        float zOctahedron = disOctahedronZ(gen);

        m_SDObjectUPtrVec.emplace_back(std::make_unique<sdf::Link>(0.5f, 0.5f, 0.1f, glm::vec3{ xLink, yLink, zLink }));
        m_SDObjectUPtrVec.emplace_back(std::make_unique<sdf::Octahedron>(1.f, glm::vec3{ xOctahedron, yOctahedron, zOctahedron }));
    }

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