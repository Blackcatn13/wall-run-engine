#pragma once

#ifndef CINEMATIC_PLAYER_H
#define CINEMATIC_PLAYER_H

class CCinematicPlayer
{
protected:
	bool	m_Playing;
	float	m_CurrentTime;
	float	m_Duration;
	bool	m_Cycle;
public:
	CCinematicPlayer();
	virtual ~CCinematicPlayer();
	void Init(float Duration);
	virtual void Update(float ElapsedTime);
	virtual void Stop();
	virtual void Play(bool Cycle);
	virtual void Pause();
	bool IsFinished() {return m_CurrentTime>=m_Duration;}
	float GetDuration() {return m_Duration;}
	float GetCurrentTimes() {return m_CurrentTime;}
	virtual void OnRestartCycle();
};


#endif