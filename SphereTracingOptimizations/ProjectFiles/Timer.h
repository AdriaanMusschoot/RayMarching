#pragma once

//Standard includes
#include <chrono>
#include <cstdint>
#include <vector>
#include <iostream>

#include "Singleton.h"


namespace sdf
{
	class GameTimer final : public Singleton<GameTimer>
	{
	public:
		virtual ~GameTimer() = default;

		GameTimer(const GameTimer&) = delete;
		GameTimer(GameTimer&&) noexcept = delete;
		GameTimer& operator=(const GameTimer&) = delete;
		GameTimer& operator=(GameTimer&&) noexcept = delete;

		void StartBenchmark();
		void Update();
		
		float GetElapsed() const { return m_ElapsedTime; }

		int& SetBenchmarkTargetFrames() { return m_BenchmarkTargetFrames; }
	private:
		friend Singleton<GameTimer>;
		GameTimer();

		std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTime;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_PreviousTime;

		float m_OutputTimer{ 0.0f };
		uint32_t m_OutputFPSCount{ 0 };
		
		float m_TotalTime{ 0.0f };
		uint32_t m_TotalFPSCount{ 0 };

		float m_ElapsedTime{ 0.0f };

		bool m_BenchmarkActive{ false };
		int m_BenchmarkTargetFrames{ 1000 };
		std::vector<float> m_BenchmarkFrameTimeVec{};

		void PrintFPS() const;
		void EndBenchmark();
		static void OutputBenchmarkResults(std::vector<float> const& frameTimeVec, std::ostream& outputStream);
	};
}
