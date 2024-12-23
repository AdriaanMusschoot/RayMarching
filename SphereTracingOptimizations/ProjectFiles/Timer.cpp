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

void sdf::GameTimer::StartBenchmark()
{
	if (m_BenchmarkActive)
	{
		std::cout << "Benchmark already active\n";
		return;
	}

	m_BenchmarkActive = true;
	m_BenchmarkFrameTimeVec.clear();
	m_BenchmarkFrameTimeVec.reserve(m_BenchmarkTargetFrames);

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
	//	//PrintFPS();

	//	m_OutputFPSCount = 0;
	//	m_OutputTimer = 0.0f;
	//}

	if (m_BenchmarkActive)
	{
		m_BenchmarkFrameTimeVec.emplace_back(m_ElapsedTime); // Record the instantaneous FPS for the frame
		if (m_BenchmarkFrameTimeVec.size() >= m_BenchmarkTargetFrames)
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
	
	std::ofstream fileStream("benchmark.txt");
	OutputBenchmarkResults(m_BenchmarkFrameTimeVec, fileStream);
	fileStream.close();
}

void sdf::GameTimer::OutputBenchmarkResults(std::vector<float> const& frameTimeVec, std::ostream& outputStream)
{
	if (frameTimeVec.empty()) return;

	float benchMarkTotalTime{ std::accumulate(frameTimeVec.begin(), frameTimeVec.end(), 0.0f) };
	float avgFrameTime{ benchMarkTotalTime / frameTimeVec.size() }; 
	float maxFrameTime{ *std::ranges::max_element(frameTimeVec) };
	float minFrameTime{ *std::ranges::min_element(frameTimeVec) };

	outputStream << "**BENCHMARK FINISHED**\n";
	outputStream << ">> TOTAL TIME = " << benchMarkTotalTime << " seconds\n";
	outputStream << ">> TOTAL FRAMES = " << frameTimeVec.size() << "\n\n";
	outputStream << ">> LONGEST TIME = " << maxFrameTime << "\n";
	outputStream << ">> LOWEST FPS = " << 1 / maxFrameTime << "\n\n";
	outputStream << ">> SHORTEST TIME = " << minFrameTime << "\n";
	outputStream << ">> HIGHEST FPS = " << 1 / minFrameTime << "\n\n";
	outputStream << ">> AVG TIME = " << avgFrameTime << "\n";
	outputStream << ">> AVG FPS = " << 1 / avgFrameTime << "\n\n";
}