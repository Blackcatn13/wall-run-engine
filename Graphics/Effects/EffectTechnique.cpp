#include "EffectTechnique.h"
#include "Effect.h"
#include "Core\Core.h"
#include "GraphicsManager.h"
#include "Core\Core.h"
#include "EffectManager.h"
#include <d3dx9.h>
#include "Core_Utils/MemLeaks.h"

CEffectTechnique::CEffectTechnique()
    : m_Effect(NULL)
    , m_D3DTechnique(NULL)
{
}

CEffectTechnique::~CEffectTechnique()
{
}
bool CEffectTechnique::BeginRender()
{
    if (m_Effect == NULL)
        return false;
    CEffectManager *l_EM = EFFECTM;
    LPD3DXEFFECT l_Effect = m_Effect->GetD3DEffect();
    if (m_UseCameraPosition) {
        float cameraeye[3];
        cameraeye[0] = EFFECTM->GetCameraEye().x;
        cameraeye[1] = EFFECTM->GetCameraEye().y;
        cameraeye[2] = EFFECTM->GetCameraEye().z;
        l_Effect->SetFloatArray(m_Effect->GetCameraPositionParameter(), &l_EM->GetCameraEye().x, 3);
    }
    if (m_UseInverseProjMatrix) {
        Mat44f l_InvertedProjectionMatrix = EFFECTM->GetProjectionMatrix();
        l_InvertedProjectionMatrix.Invert();
        l_Effect->SetMatrix(m_Effect->GetInverseProjectionMatrixParameter(), &l_InvertedProjectionMatrix.GetD3DXMatrix());
    }
    if (m_UseInverseViewMatrix) {
        Mat44f l_InvertedViewMatrix = EFFECTM->GetViewMatrix();
        l_InvertedViewMatrix.Invert();
        l_Effect->SetMatrix(m_Effect->GetInverseViewMatrixParameter(), &l_InvertedViewMatrix.GetD3DXMatrix());
    }
    if (m_UseInverseWorldMatrix) {
        Mat44f l_InvertedWorldMatrix = EFFECTM->GetWorldMatrix();
        l_InvertedWorldMatrix.Invert();
        l_Effect->SetMatrix(m_Effect->GetInverseWorldMatrixParameter(), &l_InvertedWorldMatrix.GetD3DXMatrix());
    }
    if (m_UseLights) {
        l_Effect->SetBoolArray(m_Effect->GetLightEnabledParameter(), &m_Effect->GetLightsEnabled()[0], MAX_LIGHTS_BY_SHADER);
    }
    //if(m_UseIntensity)
    //{
    //l_Effect->
    //}
    if (m_UseLightAmbientColor) {
        l_Effect->SetFloatArray(m_Effect->GetLightsColorParameter(), &m_Effect->GetLightsColor()[0].x, MAX_LIGHTS_BY_SHADER * 3);
    }
    if (m_UseProjMatrix) {
        l_Effect->SetMatrix(m_Effect->GetProjectionMatrixParameter(), &l_EM->GetProjectionMatrix().GetD3DXMatrix());
    }
    if (m_UseViewMatrix) {
        l_Effect->SetMatrix(m_Effect->GetViewMatrixParameter(), &l_EM->GetViewMatrix().GetD3DXMatrix());
    }
    if (m_UseWorldMatrix) {
        l_Effect->SetMatrix(m_Effect->GetWorldMatrixParameter(), &l_EM->GetWorldMatrix().GetD3DXMatrix());
    }
    if (m_UseWorldViewMatrix) {
        Mat44f l_WorldMatrix = EFFECTM->GetWorldMatrix();
        Mat44f l_ViewMatrix = EFFECTM->GetViewMatrix();
        Mat44f l_Transform = l_ViewMatrix * l_WorldMatrix;
        l_Effect->SetMatrix(m_Effect->GetWorldViewMatrixParameter(), &l_Transform.GetD3DXMatrix());
    }
    if (m_UseWorldViewProjectionMatrix) {
        Mat44f l_World = l_EM->GetWorldMatrix();
        Mat44f l_View = l_EM->GetViewMatrix();
        Mat44f l_Projection = l_EM->GetProjectionMatrix();
        Mat44f l_Transform = l_Projection * l_View * l_World;
        l_Effect->SetMatrix(m_Effect->GetWorldViewProjectionMatrixParameter(), &l_Transform.GetD3DXMatrix());
    }
    if (m_UseViewProjectionMatrix) {
        Mat44f l_View = l_EM->GetViewMatrix();
        Mat44f l_Projection = l_EM->GetProjectionMatrix();
        Mat44f l_Transform = l_Projection * l_View;
        l_Effect->SetMatrix(m_Effect->GetViewProjectionMatrixParameter(), &l_Transform.GetD3DXMatrix());
    }
    if (m_UseViewToLightProjectionMatrix) {
        //ShadowMap
        //Mat44f l_ViewToLightProjectionMatrix=l_ViewMatrix;
        Mat44f l_ViewToLightProjectionMatrix = l_EM->GetViewMatrix();
        //l_ViewToLightProjectionMatrix.Inverse();
        l_ViewToLightProjectionMatrix.GetInverted();
        l_ViewToLightProjectionMatrix = l_ViewToLightProjectionMatrix * l_EM->GetLightViewMatrix();
        l_ViewToLightProjectionMatrix = l_ViewToLightProjectionMatrix * l_EM->GetShadowProjectionMatrix();
        //l_Effect->SetMatrix(m_Effect->m_ViewToLightProjectionMatrixParameter, &l_ViewToLightProjectionMatrix.GetD3DXMatrix());
        l_Effect->SetMatrix(m_Effect->GetViewToLightProjectionMatrixParameter(), &l_ViewToLightProjectionMatrix.GetD3DXMatrix());
    }
    if (m_UseTime) {
        //TODO
    }
    if (m_NumOfLights > 0) {
        if (m_NumOfLights < 4) {
            m_Effect->SetLights(m_NumOfLights);
        } else {
            m_Effect->SetLights(MAX_LIGHTS_BY_SHADER);
        }
    }
    return true;
}
bool CEffectTechnique::Refresh()
{
    m_D3DTechnique = m_Effect->GetTechniqueByName(m_TechniqueName);
    return false;
}
//DirectX Methods Interface
D3DXHANDLE CEffectTechnique::GetD3DTechnique()
{
    D3DXHANDLE l_EffectTechnique = m_Effect->GetTechniqueByName(m_TechniqueName.c_str());
    return l_EffectTechnique;
}