#include "Effect.h"
#include "Lights\LightManager.h"
#include "Lights\Light.h"
#include "Lights\SpotLight.h"
#include "Core\Core.h"
#include "d3dx9.h"
#include <string>
#include <map>

void CEffect::SetNullParameters()
{
	m_Effect = NULL;
	
	m_WorldMatrixParameter = NULL;
	m_ViewMatrixParameter = NULL;
	m_ProjectionMatrixParameter = NULL;
	m_WorldViewMatrixParameter = NULL;
	m_ViewProjectionMatrixParameter = NULL;
	m_WorldViewProjectionMatrixParameter = NULL;
	m_ViewToLightProjectionMatrixParameter = NULL;
	m_LightEnabledParameter = NULL;
	m_LightsTypeParameter = NULL;
	m_LightsPositionParameter = NULL;
	m_LightsDirectionParameter = NULL;
	m_LightsAngleParameter = NULL;
	m_LightsColorParameter = NULL;
	m_LightsFallOffParameter = NULL;
	m_LightsStartRangeAttenuationParameter = NULL;
	m_LightsEndRangeAttenuationParameter = NULL;
	m_CameraPositionParameter = NULL;
	m_BonesParameter = NULL;
	m_TimeParameter = NULL;
}

void CEffect::GetParameterBySemantic(const std::string &SemanticName, D3DXHANDLE &l_Handle)
{
	l_Handle=m_Effect->GetParameterBySemantic(NULL,SemanticName.c_str());
	if(l_Handle==NULL)
		LOGGER->AddNewLog(ELL_ERROR, "Parameter by semantic '%s' wasn't found on effect '%s'", SemanticName.c_str(), m_FileName.c_str());
}
bool CEffect::LoadEffect()
{
	bool isOK = false;
	if(!m_FileName.compare(""))
	{
		isOK = Load(m_FileName);
	}else
	
		return isOK;
}
void CEffect::Unload()
{
	SetNullParameters();
}

CEffect::CEffect()
{
}

CEffect::~CEffect()
{
}

bool CEffect::SetLights(size_t NumOfLights)
{
	std::map<std::string, CLight*>::iterator it = LIGHTM->GetResources().begin();
	int l_lightIndex = 0;
	while(it != LIGHTM->GetResources().end())
	{
		m_LightsEnabled[l_lightIndex] = 1;
		int l_type = it->second->GetType();
		m_LightsType[l_lightIndex] = l_type;
		float l_angle = 0;
		if(l_type == CLight::TLightType::SPOT)
		{
			l_angle = ((CSpotLight*)it->second)->GetAngle();
		}
		m_LightsAngle[l_lightIndex] = l_angle;
		float l_falloff = 0;
		if(l_type == CLight::TLightType::SPOT)
		{
			l_falloff = ((CSpotLight*)it->second)->GetFallOff();
		}
		m_LightsFallOff[l_lightIndex] = l_falloff;
		float l_startRangeAtten = it->second->GetStartRangeAttenuation();
		m_LightsStartRangeAttenuation[l_lightIndex] = l_startRangeAtten;
		float l_endRangeAtten = it->second->GetEndRangeAttenuation();
		m_LightsEndRangeAttenuation[l_lightIndex] = l_endRangeAtten;
		float l_intensity = it->second->GetIntensity();
		m_LightsIntensity[l_lightIndex] = l_intensity;
		Vect3f l_pos = it->second->GetPosition();
		m_LightsPosition[l_lightIndex] = l_pos;
		Vect3f l_direction;
		if(l_type == CLight::TLightType::OMNI)
		{
			l_direction = Vect3f(0,0,0);
		}else
		{
			l_direction = ((CDirectionalLight*)it->second)->GetDirection();
		}
		m_LightsDirection[l_lightIndex] = l_direction;
		Vect3f l_color = Vect3f(it->second->GetColor().GetRed(),it->second->GetColor().GetBlue(), it->second->GetColor().GetGreen()) ;
		m_LightsColor[l_lightIndex] = l_color;
	}

	return true;
}
bool CEffect::Load(const std::string &FileName)
{
	m_FileName = FileName;
	LPD3DXBUFFER l_ErrorBuffer=NULL;
	HRESULT l_HR=D3DXCreateEffectFromFile(GRAPHM->GetDevice(), FileName.c_str(), NULL, NULL, D3DXSHADER_USE_LEGACY_D3DX9_31_DLL, NULL, &m_Effect, &l_ErrorBuffer);
	if(l_ErrorBuffer)
	{
		LOGGER->AddNewLog(ELL_ERROR, "Error creating effect '%s':\n%s", FileName.c_str(), l_ErrorBuffer->GetBufferPointer());
		CHECKED_RELEASE(l_ErrorBuffer);
		return false;
	}
	return true;
}
bool CEffect::Reload()
{
	Unload();
	bool isOK = LoadEffect();
	return isOK;
}
//DirectX Methods Interface
LPD3DXEFFECT CEffect::GetD3DEffect() const
{
	return m_Effect;
}
D3DXHANDLE CEffect::GetTechniqueByName(const std::string &TechniqueName)
{
	D3DXHANDLE l_EffectTechnique=m_Effect->GetTechniqueByName(TechniqueName.c_str());
	return l_EffectTechnique;
}