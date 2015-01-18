#include "EffectTechnique.h"
#include "Effect.h"
#include "Core\Core.h"
#include "GraphicsManager.h"
#include "Core\Core.h"
#include "EffectManager.h"
#include "d3dx9.h"

CEffectTechnique::CEffectTechnique()
{
}
CEffectTechnique::~CEffectTechnique()
{
}
bool CEffectTechnique::BeginRender()
{
	if(m_UseCameraPosition)
	{
		float cameraeye[3];
		cameraeye[0] = EFFECTM->GetCameraEye().x;
		cameraeye[1] = EFFECTM->GetCameraEye().y;
		cameraeye[2] = EFFECTM->GetCameraEye().z;
		m_Effect->GetD3DEffect()->SetFloatArray(m_Effect->GetCameraPositionParameter(), cameraeye, 3);
	}
	if(m_UseInverseProjMatrix)
	{
	}
	if(m_UseInverseViewMatrix)
	{
	}
	if(m_UseInverseWorldMatrix)
	{
	}
	if(m_UseLights)
	{
	}
	if(m_NumOfLights)
	{
	}
	if(m_UseLightAmbientColor)
	{
	}
	if(m_UseProjMatrix)
	{
	}
	if(m_UseViewMatrix)
	{
	}
	if(m_UseWorldMatrix)
	{
	}
	if(m_UseWorldViewMatrix)
	{
	}
	if(m_UseWorldViewProjectionMatrix)
	{
	}
	if(m_UseViewProjectionMatrix)
	{
	}
	if(m_UseViewToLightProjectionMatrix)
	{
	}
	if(m_UseTime)
	{
	}
	return true;
	//****************************************************************************************
	//
	// --------- TODO -----------------
	//
	//****************************************************************************************
}
bool CEffectTechnique::Refresh()
{
	return false;
}
//DirectX Methods Interface
D3DXHANDLE CEffectTechnique::GetD3DTechnique()
{
	D3DXHANDLE l_EffectTechnique=m_Effect->GetTechniqueByName(m_TechniqueName.c_str());
	return l_EffectTechnique;
}