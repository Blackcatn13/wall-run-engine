#include <Windows.h>
#include <assert.h>
#include "Timer.h"

//Inicializamos todas las variables
CTimer::CTimer (uint32 avgSamples)
: m_uSamples(avgSamples)
, m_fElpasedTime(0.f)
, m_dLastTime(0)
, m_fFPS(0.f)
, m_fFPSTime(0.f)
, m_uFPSCount(0)
, m_fLastFps(0.f)
, m_uIndex(0)
{
	m_Deltas = new float[m_uSamples];
	for(uint32 j=0;j<m_uSamples;j++)
	{
		m_Deltas[j] = 0;
	}
}

CTimer::~CTimer( void )
{
	delete[] m_Deltas;
}


// Actualiza los valores FPS, ElapsedTime y los timers (los normales y los inputs)
void CTimer::Update( void )
{

	//-----Actualizacion de los valores FPS y ElpasedTime-----
	//Calculo de la diferencia de tiempo (m_fElpasedTime)
	double	l_dCurTime = timeGetTime();

	if( m_dLastTime == 0 ) 
		m_dLastTime = l_dCurTime;

	m_Deltas[m_uIndex] = (float)((l_dCurTime - m_dLastTime) * 0.001);
	m_dLastTime    = l_dCurTime;

	float d = 0;
	for(unsigned int j=0; j<m_uSamples; j++)
	{
		d += m_Deltas[j];
	}
	d /= (float)m_uSamples;
	m_fElpasedTime = d;
	m_uIndex = (++m_uIndex) % m_uSamples;


	//Calculo de los frames por segundo (m_fFPS)
	m_uFPSCount++;
	m_fFPSTime += m_fElpasedTime;

	m_fFPS = m_uFPSCount/m_fFPSTime;

	if( l_dCurTime - m_fLastFps > 1000 )	//Solo mostramos los fps cada segundo
	{
		m_fFPS = m_uFPSCount/m_fFPSTime;
		m_uFPSCount = 0;
		m_fFPSTime = 0;
		m_fLastFps = (float)l_dCurTime;
	}
	//---------------------------------------------//
}
