#pragma once


#include <string>

#include "Texture\Texture.h"
#include <d3dx9.h>

#define MAX_LIGHTS_BY_SHADER 10

class CEffect
{
private:
	std::string m_FileName;
	LPD3DXEFFECT m_Effect;
	//BOOL m_LightsEnabled[MAX_LIGHTS_BY_SHADER];
	int m_LightsEnabled[MAX_LIGHTS_BY_SHADER];
	int m_LightsType[MAX_LIGHTS_BY_SHADER];
	float m_LightsAngle[MAX_LIGHTS_BY_SHADER];
	float m_LightsFallOff[MAX_LIGHTS_BY_SHADER];
	float m_LightsStartRangeAttenuation[MAX_LIGHTS_BY_SHADER];
	float m_LightsEndRangeAttenuation[MAX_LIGHTS_BY_SHADER];
	/*CPoint3D*/ Vect3f m_LightsPosition[MAX_LIGHTS_BY_SHADER];
	/*CPoint3D*/ Vect3f m_LightsDirection[MAX_LIGHTS_BY_SHADER];
	/*CPoint3D*/ Vect3f m_LightsColor[MAX_LIGHTS_BY_SHADER];
	D3DXHANDLE m_WorldMatrixParameter, m_ViewMatrixParameter,
	m_ProjectionMatrixParameter;
	D3DXHANDLE m_WorldViewMatrixParameter,
	m_ViewProjectionMatrixParameter,
	m_WorldViewProjectionMatrixParameter;
	D3DXHANDLE m_ViewToLightProjectionMatrixParameter;
	D3DXHANDLE m_LightEnabledParameter, m_LightsTypeParameter,
	m_LightsPositionParameter, m_LightsDirectionParameter,
	m_LightsAngleParameter, m_LightsColorParameter;
	D3DXHANDLE m_LightsFallOffParameter,
	m_LightsStartRangeAttenuationParameter,
	m_LightsEndRangeAttenuationParameter;
	D3DXHANDLE m_CameraPositionParameter;
	D3DXHANDLE m_BonesParameter;
	D3DXHANDLE m_TimeParameter;
	void SetNullParameters();
	void GetParameterBySemantic(const std::string &SemanticName, D3DXHANDLE
	&l_Handle);
	bool LoadEffect();
	void Unload();
public:
	CEffect(){}
	~CEffect(){}
	bool SetLights(size_t NumOfLights){return false;}
	bool Load(const std::string &FileName){return false;}
	bool Reload(){return false;}
	//DirectX Methods Interface
	/*LPD3DXEFFECT*/ void GetD3DEffect() const{}
	/*D3DXHANDLE*/ void GetTechniqueByName(const std::string &TechniqueName){}
};

