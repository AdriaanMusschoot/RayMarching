#pragma once
#include "Scene.h"

namespace sdf
{

	class SceneEasyComplexity final : public Scene
	{
	public:
		SceneEasyComplexity();
		~SceneEasyComplexity() override = default;

		SceneEasyComplexity(const SceneEasyComplexity&) = delete;
		SceneEasyComplexity(SceneEasyComplexity&&) noexcept = delete;
		SceneEasyComplexity& operator=(const SceneEasyComplexity&) = delete;
		SceneEasyComplexity& operator=(SceneEasyComplexity&&) noexcept = delete;
	};

	class SceneMediumComplexity final : public Scene
	{
	public:
		SceneMediumComplexity();
		~SceneMediumComplexity() override = default;

		SceneMediumComplexity(const SceneMediumComplexity&) = delete;
		SceneMediumComplexity(SceneMediumComplexity&&) noexcept = delete;
		SceneMediumComplexity& operator=(const SceneMediumComplexity&) = delete;
		SceneMediumComplexity& operator=(SceneMediumComplexity&&) noexcept = delete;
	};

	class SceneHighComplexity final : public Scene
	{
	public:
		SceneHighComplexity();
		~SceneHighComplexity() override = default;

		SceneHighComplexity(const SceneHighComplexity&) = delete;
		SceneHighComplexity(SceneHighComplexity&&) noexcept = delete;
		SceneHighComplexity& operator=(const SceneHighComplexity&) = delete;
		SceneHighComplexity& operator=(SceneHighComplexity&&) noexcept = delete;
	};

}