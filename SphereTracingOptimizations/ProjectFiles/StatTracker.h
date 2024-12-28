#pragma once
#include <vector>

namespace sdf
{

	struct HitRecord;

	class Engine;

	class StatTracker final
	{

	public:
		StatTracker();
		~StatTracker();

		StatTracker(const StatTracker&) = delete;
		StatTracker(StatTracker&&) noexcept = delete;
		StatTracker& operator=(const StatTracker&) = delete;
		StatTracker& operator=(StatTracker&&) noexcept = delete;

		void Update(std::vector<HitRecord>&& hitRecord, Engine& engine);

		void StartTracking();
		int GetNrCollisions() const;
		int GetNrMisses() const;
	private:
		bool m_IsTracking{ false };

		float m_TotalTrackTime{ 1.f };
		float m_CurrentTrackTime{ 0.f };

		std::vector<HitRecord> m_CurrentFrameHitRecords;
	};

}