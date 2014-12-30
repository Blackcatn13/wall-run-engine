#ifndef CSPOTLIGHT_H
#define CSPOTLIGHT_H
#include "DirectionalLight.h"

class CSpotLight : public CDirectionalLight
{
protected:
	float m_Angle;
	float m_FallOff;
public:
	CSpotLight();
	void SetAngle(float Angle)
	{
		m_Angle = Angle;
	}
	float GetAngle() const
	{
		return m_Angle;
	}
	void SetFallOff(const float FallOff)
	{
		m_FallOff = FallOff;
	}
	float GetFallOff() const
	{
		return m_FallOff;
	}
};


#endif
