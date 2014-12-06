#include "CountDownTimer.h"


void CCountDownTimer::SetTime (float TotalTime, bool automaticReset) 
{
	m_fTotalTime		= TotalTime;
	m_fElapsedTime	= 0.f;
}

bool CCountDownTimer::Update (float deltaTime) 
{
	m_fElapsedTime += deltaTime;
	return Finished();
}

bool CCountDownTimer::Finished ()
{
	if(m_fElapsedTime>=m_fTotalTime) 
	{
		if (m_bAutomaticReset)
		{
			m_fElapsedTime = 0.f;
		}
		else
		{
			m_fElapsedTime = m_fTotalTime;
		}
		return true;
	}
	else
	{
		return false;
	}
}