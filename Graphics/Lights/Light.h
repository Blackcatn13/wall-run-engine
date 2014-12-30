#ifndef CLIGHT_H
#define CLIGHT_H

#include "Object\Object3D.h"
#include "Math\Color.h"
#include <string>

class CGraphicsManager;

class CLight : public CObject3D
{
	public:
		enum TLightType
		{
			OMNI=0,
			DIRECTIONAL,
			SPOT
		};
	protected:
		CColor m_Color;
		TLightType m_Type;
		std::string m_Name;
		bool m_RenderShadows;
		float m_StartRangeAttenuation;
		float m_EndRangeAttenuation;
		static TLightType GetLightTypeByName(const std::string &StrLightType);
public:
	CLight();
	virtual ~CLight();
	void SetName(const std::string &Name)
	{
		m_Name = Name;
	}
	const std::string &GetName()
	{
		return m_Name;
	}
	void SetColor(const CColor &Color)
	{
		m_Color = Color;
	}
	const CColor & GetColor() const
	{
		return m_Color;
	}
	void SetStartRangeAttenuation(const float StartRangeAttenuation)
	{
		m_StartRangeAttenuation = StartRangeAttenuation;
	}
	float GetStartRangeAttenuation() const
	{
		return m_StartRangeAttenuation;
	}
	void SetEndRangeAttenuation(const float EndRangeAttenuation)
	{
		m_EndRangeAttenuation = EndRangeAttenuation;
	}
	float GetEndRangeAttenuation() const
	{
		return m_EndRangeAttenuation;
	}
	bool RenderShadows() const
	{
		return m_RenderShadows;
	}
	void SetType(const TLightType Type)
	{
		m_Type = Type;
	}
	TLightType GetType() const
	{
		return m_Type;
	}

	virtual void Render(CGraphicsManager *RM);
};


#endif
