#include "StatTracker.h"

#include <algorithm>
#include <execution>

#include "SdEngine.h"
#include "Misc.h"

sdf::StatTracker::StatTracker() = default;

sdf::StatTracker::~StatTracker() = default;

void sdf::StatTracker::Update(std::vector<HitRecord>&& hitRecord, Engine& engine)
{
	if (m_IsTracking)
	{
		m_CurrentTrackTime += engine.GetTimer().GetElapsed();
		if (m_CurrentTrackTime >= m_TotalTrackTime)
		{
			m_IsTracking = false;
			std::cout << "Tracking finished!\n";
		}
		return;
	}
	m_CurrentFrameHitRecords = std::move(hitRecord);
}

void sdf::StatTracker::StartTracking()
{
	if (m_IsTracking)
	{
		return;
	}
	m_CurrentTrackTime = 0.0f;
	m_IsTracking = true;
}

int sdf::StatTracker::GetNrCollisions() const
{
	return std::count_if(std::execution::par_unseq, m_CurrentFrameHitRecords.begin(), m_CurrentFrameHitRecords.end(), [](HitRecord const& hitRecord)
		{
			return hitRecord.DidHit;
		});
}

int sdf::StatTracker::GetNrMisses() const
{
	return m_CurrentFrameHitRecords.size() - GetNrCollisions();
}
