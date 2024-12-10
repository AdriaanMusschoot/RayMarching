#pragma once

//Standard includes
#include <chrono>
#include <cstdint>
#include <vector>

namespace VM
{
	class Timer
	{
	public:
		Timer();
		virtual ~Timer() = default;

		Timer(const Timer&) = delete;
		Timer(Timer&&) noexcept = delete;
		Timer& operator=(const Timer&) = delete;
		Timer& operator=(Timer&&) noexcept = delete;

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
