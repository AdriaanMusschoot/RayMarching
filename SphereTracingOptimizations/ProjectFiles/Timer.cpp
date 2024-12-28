#include "Timer.h"

#include <iostream>
#include <numeric>
#include <fstream>
#include <variant>

#include "Renderer.h"
#include "SDL.h"

sdf::GameTimer::GameTimer()
{
	m_StartTime = std::chrono::high_resolution_clock::now();
	m_PreviousTime = m_StartTime;
}

void sdf::GameTimer::StartBenchmark(std::string const& sceneName)
{
	if (m_BenchmarkActive)
	{
		std::cout << "Benchmark already active\n";
		return;
	}

	m_BenchmarkActive = true;
	m_BenchmarkTime = 0.0f;
	m_BenchmarkFrameTimeVec.clear();
	m_BenchmarkFrameTimeVec.reserve(60 * m_BenchmarkTargetTime);
	m_CurrentSceneName = sceneName;

	std::cout << "**BENCHMARK STARTED**\n";
}

void sdf::GameTimer::Update()
{
	auto currentTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> elapsed = currentTime - m_PreviousTime;
	m_ElapsedTime = elapsed.count();
	m_PreviousTime = currentTime;

	m_TotalTime += m_ElapsedTime;
	++m_TotalFPSCount;

	//reset after 1 second -> so fpS
	//if (m_OutputTimer >= 1.0f)
	//{
	//	m_TotalTime += m_OutputTimer;
	//	m_TotalFPSCount += m_OutputFPSCount;
	//	
	//	m_OutputFPSCount = 0;
	//	m_OutputTimer = 0.0f;
	//}

	if (m_BenchmarkActive)
	{
		m_BenchmarkTime += m_ElapsedTime;
		m_BenchmarkFrameTimeVec.emplace_back(m_ElapsedTime); 

		if (m_BenchmarkTime >= m_BenchmarkTargetTime)
		{
			EndBenchmark();
		}
	}
}

void sdf::GameTimer::PrintFPS() const
{
	std::cout << "FPS: " << m_OutputFPSCount << "\n";
	std::cout << "AVG FPS: " << m_TotalFPSCount / m_TotalTime << " <= (" << m_TotalFPSCount << " : " << m_TotalTime << ")\n";
}

void sdf::GameTimer::EndBenchmark()
{
	m_BenchmarkActive = false;

	OutputBenchmarkResults(m_BenchmarkFrameTimeVec, std::cout);
	
	std::ofstream fileStream("benchmark.csv", std::ios::app);
	OutputBenchmarkResults(m_BenchmarkFrameTimeVec, fileStream);
	fileStream.close();
}

void sdf::GameTimer::OutputBenchmarkResults(std::vector<float> const& frameTimeVec, std::ostream& outputStream)
{
	if (frameTimeVec.empty()) return;

	std::vector<float> sortedFrameTimes{ frameTimeVec };
	std::sort(sortedFrameTimes.begin(), sortedFrameTimes.end());

	size_t const numElementsToRemove = static_cast<size_t>(sortedFrameTimes.size() * 0.05);

	sortedFrameTimes.erase(sortedFrameTimes.begin(), sortedFrameTimes.begin() + numElementsToRemove);
	sortedFrameTimes.erase(sortedFrameTimes.end() - numElementsToRemove, sortedFrameTimes.end());

	double const benchMarkTotalTime{ std::accumulate(sortedFrameTimes.begin(), sortedFrameTimes.end(), 0.0f) };
	double const avgFrameTime{ benchMarkTotalTime / sortedFrameTimes.size() };
	double const maxFrameTime{ *std::ranges::max_element(sortedFrameTimes) };
	double const minFrameTime{ *std::ranges::min_element(sortedFrameTimes) };

	std::ifstream fileCheck("benchmark.csv");
	bool isEmpty{ fileCheck.peek() == std::ifstream::traits_type::eof() };
	fileCheck.close();

	if (isEmpty)
	{
		outputStream << "\n";
		outputStream << ";SCENE COMPLEXITY;TOTAL TIME (s);TOTAL FRAMES;LONGEST TIME;LOWEST FPS;SHORTEST TIME;HIGHEST FPS;AVG TIME;AVG FPS\n";
	}

	outputStream << ";"
		<< m_CurrentSceneName << ";"
		<< std::to_string(benchMarkTotalTime) << ";"
		<< sortedFrameTimes.size() << ";"
		<< std::to_string(maxFrameTime) << ";"
		<< 1 / maxFrameTime << ";"
		<< std::to_string(minFrameTime) << ";"
		<< 1 / minFrameTime << ";"
		<< std::to_string(avgFrameTime) << ";"
		<< 1 / avgFrameTime << "\n";
}