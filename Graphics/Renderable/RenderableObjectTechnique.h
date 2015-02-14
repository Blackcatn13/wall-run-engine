#pragma once
#ifndef RENDERABLE_OBJECT_TECHNIQUE_H
#define RENDERABLE_OBJECT_TECHNIQUE_H

#include <string>
#include "Utils\Named.h"
#include "Effects\EffectTechnique.h"

class CRenderableObjectTechnique : public CNamed
{
private:
    CEffectTechnique *m_EffectTechnique;
public:
    CRenderableObjectTechnique();
    CRenderableObjectTechnique(const std::string &Name, CEffectTechnique*EffectTechnique);
    void SetEffectTechnique(CEffectTechnique *EffectTechnique);
    CEffectTechnique * GetEffectTechnique() const;
};

/*class CEffectTechnique
{
private:
	bool m_UseCameraPosition;
	bool m_UseInverseProjMatrix;
	bool m_UseInverseViewMatrix;
	bool m_UseInverseWorldMatrix;
	bool m_UseLights;
	int m_NumOfLights;
	bool m_UseLightAmbientColor;
	bool m_UseProjMatrix;
	bool m_UseViewMatrix;
	bool m_UseWorldMatrix;
	bool m_UseWorldViewMatrix;
	bool m_UseWorldViewProjectionMatrix;
	bool m_UseViewProjectionMatrix;
	bool m_UseViewToLightProjectionMatrix;
	bool m_UseTime;
	CEffect *m_Effect;
	D3DXHANDLE m_D3DTechnique;
	std::string m_TechniqueName;
public:
	CEffectTechnique ();
	~CEffectTechnique ();
	inline CEffect * GetEffect() const {return m_Effect;}
	bool BeginRender();
	bool Refresh();

	//DirectX Methods Interface
	D3DXHANDLE GetD3DTechnique();
};*/

#endif