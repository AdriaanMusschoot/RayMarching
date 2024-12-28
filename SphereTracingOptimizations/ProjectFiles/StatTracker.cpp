#include "StatTracker.h"

#include <algorithm>
#include <execution>

#include "SdEngine.h"
#include "Misc.h"

sdf::StatTracker::StatTracker() = default;

sdf::StatTracker::~StatTracker() = default;

void sdf::StatTracker::Update(std::vector<HitRecord>const& hitRecordVec, GameTimer& timer)
{
	m_CurrentFrameHitRecordVec = hitRecordVec;

	if (m_IsTracking)
	{
		m_CurrentTrackTime += timer.GetElapsed();
		m_TrackedHitRecordVecVec.emplace_back(hitRecordVec);
		if (m_CurrentTrackTime >= m_TotalTrackTime)
		{
			m_IsTracking = false;
			std::cout << "Tracking finished!\n";
		}
	}
}

void sdf::StatTracker::StartTracking()
{
	if (m_IsTracking)
	{
		return;
	}
	m_IsTracking = true;
	m_CurrentTrackTime = 0.0f;
	m_TrackedHitRecordVecVec.clear();
	m_TrackedHitRecordVecVec.reserve(static_cast<int>(60 * m_TotalTrackTime));
	std::cout << "Tracking started!\n";
}

int sdf::StatTracker::GetNrCollisions() const
{
	return std::count_if(std::execution::par_unseq, m_CurrentFrameHitRecordVec.begin(), m_CurrentFrameHitRecordVec.end(), 
		[](HitRecord const& hitRecord)
		{
			return hitRecord.DidHit;
		});
}

int sdf::StatTracker::GetNrMisses() const
{
	return m_CurrentFrameHitRecordVec.size() - GetNrCollisions();
}