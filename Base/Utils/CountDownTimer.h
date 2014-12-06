//-----------------------------------------------------------------------
// CountDownTimer Class
// Utilidades para el control del tiempo
//-----------------------------------------------------------------------
#ifndef __COUNT_DOWN_TIMER_H__
#define __COUNT_DOWN_TIMER_H__


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// CountDownTimer Class
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CCountDownTimer
{
public:
	CCountDownTimer(float TotalTime, bool automaticReset = false): m_fTotalTime(TotalTime), m_fElapsedTime(0.f), m_bAutomaticReset(automaticReset) {}
	CCountDownTimer():								m_fTotalTime(0.f), m_fElapsedTime(0.f), m_bAutomaticReset(false) {}
	virtual ~CCountDownTimer() {/*Nothing*/}

	void	SetTime									(float TotalTime, bool automaticReset = false);
	void	Reset										()				{ m_fElapsedTime	= 0.f; }
	bool	Update									(float deltaTime); 
	bool	Finished								();
	
	float	GetTotalTime						() const	{return m_fTotalTime;}
	float	GetElapsedTime					() const	{return m_fElapsedTime;}
	float	GetLeftoverTime					() const	{return m_fTotalTime - m_fElapsedTime;}
	float	GetElapsedTimeInPercent	() const	{return (m_fElapsedTime/m_fTotalTime)*100.f;}

private:
	float	m_fTotalTime;
	float	m_fElapsedTime;
	bool	m_bAutomaticReset;
};

#endif //__COUNT_DOWN_TIMER_H__
