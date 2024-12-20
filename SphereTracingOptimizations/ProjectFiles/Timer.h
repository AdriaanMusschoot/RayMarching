#pragma once

//Standard includes
#include <chrono>
#include <cstdint>
#include <vector>
#include "iostream"


namespace VM
{
	class GameTimer
	{
	public:
		GameTimer();
		virtual ~GameTimer() = default;

		GameTimer(const GameTimer&) = delete;
		GameTimer(GameTimer&&) noexcept = delete;
		GameTimer& operator=(const GameTimer&) = delete;
		GameTimer& operator=(GameTimer&&) noexcept = delete;

		void StartBenchmark(int numFrames = 10);
		void Update();
		
		float GetElapsed() const { return m_ElapsedTime; }
		float GetTotalTime() const { return m_TotalTime; }
	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTime;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_PreviousTime;

		uint32_t m_FPS{ 0 };
		float m_CurrentFrameFPS{ 0.0f };
		uint32_t m_OutputFPSCount{ 0 };
		uint32_t m_TotalFPSCount{ 0 };
		float m_OutputTimer{ 0.0f };

		float m_ElapsedTime{ 0.0f };
		float m_TotalTime{ 0.0f };

		bool m_BenchmarkActive{ false };
		int m_BenchmarkTargetFrames{ 0 };
		std::vector<float> m_BenchmarkFrameTimeVec{};
		float m_AccumulatedBenchmarkTime;

		void PrintFPS() const;
		void EndBenchmark();
		static void OutputBenchmarkResults(std::vector<float> const& frameTimeVec, std::ostream& outputStream);
	};
}
