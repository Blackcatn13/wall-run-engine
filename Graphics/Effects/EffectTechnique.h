#pragma once
#ifndef INC_EFFECT_TECHNIQUE_H_
#define INC_EFFECT_TECHNIQUE_H_

#include <string>
#include "d3dx9shader.h"

class CEffect;

class CEffectTechnique
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
	bool m_UseScreenSize;
    CEffect *m_Effect;
    D3DXHANDLE m_D3DTechnique;
    std::string m_TechniqueName;
public:
    CEffectTechnique ();
    ~CEffectTechnique ();
    inline CEffect * GetEffect() const
    {
        return m_Effect;
    }
    bool BeginRender();
    bool Refresh();
    //DirectX Methods Interface
    D3DXHANDLE GetD3DTechnique();
    //Getters and Setters
    void SetEffect(CEffect * effect)
    {
        m_Effect = effect;
    }

    bool GetUseCameraPosition()
    {
        return m_UseCameraPosition;
    }
    bool GetUseInverseProjMatrix()
    {
        return m_UseInverseProjMatrix;
    }
    bool GetUseInverseViewMatrix()
    {
        return m_UseInverseViewMatrix;
    }
    bool GetUseInverseWorldMatrix()
    {
        return m_UseInverseWorldMatrix;
    }
    bool GetUseLights()
    {
        return m_UseLights;
    }
    int GetNumOfLights()
    {
        return m_NumOfLights;
    }
    bool GetUseLightAmbientColor()
    {
        return m_UseLightAmbientColor;
    }
    bool GetUseProjMatrix()
    {
        return m_UseProjMatrix;
    }
    bool GetUseViewMatrix()
    {
        return m_UseViewMatrix;
    }
    bool GetUseWorldMatrix()
    {
        return m_UseWorldMatrix;
    }
    bool GetUseWorldViewMatrix()
    {
        return m_UseWorldViewMatrix;
    }
    bool GetUseWorldViewProjectionMatrix()
    {
        return m_UseWorldViewProjectionMatrix;
    }
    bool GetUseViewProjectionMatrix()
    {
        return m_UseViewProjectionMatrix;
    }
    bool GetUseViewToLightProjectionMatrix()
    {
        return m_UseViewToLightProjectionMatrix;
    }
    bool GetUseTime()
    {
        return m_UseTime;
    }
	bool GetUseScreenSize()
    {
        return m_UseScreenSize;
    }

    void SetUseCameraPosition(bool value)
    {
        m_UseCameraPosition = value;
    }
    void SetUseInverseProjMatrix(bool value)
    {
        m_UseInverseProjMatrix = value;
    }
    void SetUseInverseViewMatrix(bool value)
    {
        m_UseInverseViewMatrix = value;
    }
    void SetUseInverseWorldMatrix(bool value)
    {
        m_UseInverseWorldMatrix = value;
    }
    void SetUseLights(bool value)
    {
        m_UseLights = value;
    }
    void SetNumOfLights(int nLights)
    {
        m_NumOfLights = nLights;
    }
    void SetUseLightAmbientColor(bool value)
    {
        m_UseLightAmbientColor = value;
    }
    void SetUseProjMatrix(bool value)
    {
        m_UseProjMatrix = value;
    }
    void SetUseViewMatrix(bool value)
    {
        m_UseViewMatrix = value;
    }
    void SetUseWorldMatrix(bool value)
    {
        m_UseWorldMatrix = value;
    }
    void SetUseWorldViewMatrix(bool value)
    {
        m_UseWorldViewMatrix = value;
    }
    void SetUseWorldViewProjectionMatrix(bool value)
    {
        m_UseWorldViewProjectionMatrix = value;
    }
    void SetUseViewProjectionMatrix(bool value)
    {
        m_UseViewProjectionMatrix = value;
    }
    void SetUseViewToLightProjectionMatrix(bool value)
    {
        m_UseViewToLightProjectionMatrix = value;
    }
    void SetUseTime(bool value)
    {
        m_UseTime = value;
    }
	void SetUseScreenSize(bool value)
    {
        m_UseScreenSize = value;
    }
	void SetTechniqueName(const std::string &TechniqueName)
	{
		m_TechniqueName=TechniqueName;
	}
	const std::string & GetTechniqueName()
	{
		return m_TechniqueName;
	}

    //apaño para default vertex repetidos
    int GetDefaultVertex();
};

#endif //INC_EFFECT_TECHNIQUE_H_