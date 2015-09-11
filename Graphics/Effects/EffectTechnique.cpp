#include "EffectTechnique.h"
#include "Effect.h"
#include "Core\Core.h"
#include "GraphicsManager.h"
#include "Core\Core.h"
#include "EffectManager.h"
#include <d3dx9.h>
#include "Core_Utils/MemLeaks.h"
#include "Core_Utils/Timer.h"
#include "XML\XMLTreeNode.h"

CEffectTechnique::CEffectTechnique()
  : m_Effect(NULL)
  , m_D3DTechnique(NULL)
  , m_UseCameraPosition(false)
  , m_UseInverseProjMatrix(false)
  , m_UseInverseViewMatrix(false)
  , m_UseInverseWorldMatrix(false)
  , m_UseLights(false)
  , m_NumOfLights(0)
  , m_UseLightAmbientColor(false)
  , m_UseProjMatrix(false)
  , m_UseViewMatrix(false)
  , m_UseWorldMatrix(false)
  , m_UseWorldViewMatrix(false)
  , m_UseWorldViewProjectionMatrix(false)
  , m_UseViewProjectionMatrix(false)
  , m_UseViewToLightProjectionMatrix(false)
  , m_UseTime(false)
  , m_UseScreenSize(false)
  , m_UseTick(false)
  , m_tick(0.f)
  , m_ChangeUV(false)
  , m_UV(false)
  , m_Vanish(false) {
}

CEffectTechnique::CEffectTechnique(const CXMLTreeNode &node)
  : m_TechniqueName(node.GetPszISOProperty("name", ""))
  , m_UseCameraPosition(node.GetBoolProperty("use_camera_position", false, false))
  , m_UseInverseProjMatrix(node.GetBoolProperty("use_inverse_projection_matrix", false, false))
  , m_UseInverseViewMatrix(node.GetBoolProperty("use_inverse_view_matrix", false, false))
  , m_UseInverseWorldMatrix(node.GetBoolProperty("use_inverse_world_matrix", false, false))
  , m_UseLights(node.GetBoolProperty("use_lights", false, false))
  , m_NumOfLights(node.GetIntProperty("num_of_lights", 0, false))
  , m_UseLightAmbientColor(node.GetBoolProperty("use_light_ambient_color", false, false))
  , m_UseProjMatrix(node.GetBoolProperty("use_projection_matrix", false, false))
  , m_UseViewMatrix(node.GetBoolProperty("use_view_matrix", false, false))
  , m_UseWorldMatrix(node.GetBoolProperty("use_world_matrix", false, false))
  , m_UseWorldViewMatrix(node.GetBoolProperty("use_world_view_matrix", false, false))
  , m_UseWorldViewProjectionMatrix(node.GetBoolProperty("use_world_view_projection_matrix", false, false))
  , m_UseViewProjectionMatrix(node.GetBoolProperty("use__view_projection_matrix", false, false))
  , m_UseViewToLightProjectionMatrix(node.GetBoolProperty("use_view_to_light_projection_matrix", false, false))
  , m_UseTime(node.GetBoolProperty("use_time", false, false))
  , m_UseScreenSize(node.GetBoolProperty("use_screen_size", false, false))
  , m_UseTick(node.GetBoolProperty("use_tick", false, false))
  , m_ChangeUV(node.GetBoolProperty("change_uv", false, false))
  , m_tick(0.f)
  , m_UV(false)
  , m_Vanish(node.GetBoolProperty("vanish", false, false)) {
}

CEffectTechnique::~CEffectTechnique() {
}

bool CEffectTechnique::BeginRender() {
  if (m_Effect == NULL)
    return false;
  CEffectManager *l_EM = EFFECTM;
  LPD3DXEFFECT l_Effect = m_Effect->GetD3DEffect();
  if (m_UseCameraPosition) {
    /*float cameraeye[3];
    cameraeye[0] = EFFECTM->GetCameraEye().x;
    cameraeye[1] = EFFECTM->GetCameraEye().y;
    cameraeye[2] = EFFECTM->GetCameraEye().z;*/
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
  //if (m_UseLightAmbientColor) {
  //    l_Effect->SetFloatArray(m_Effect->GetLightsColorParameter(), &m_Effect->GetLightsColor()[0].x, MAX_LIGHTS_BY_SHADER * 3);
  //}
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
  m_Effect->SetLight();
  if (m_UseViewToLightProjectionMatrix) {
    //ShadowMap
    //Mat44f l_ViewToLightProjectionMatrix=l_ViewMatrix;
    Mat44f l_ViewToLightProjectionMatrix = l_EM->GetViewMatrix();
    l_ViewToLightProjectionMatrix.Invert();
    l_ViewToLightProjectionMatrix = l_EM->GetLightViewMatrix() * l_ViewToLightProjectionMatrix;
    l_ViewToLightProjectionMatrix = l_EM->GetShadowProjectionMatrix() * l_ViewToLightProjectionMatrix;
    //l_Effect->SetMatrix(m_Effect->m_ViewToLightProjectionMatrixParameter, &l_ViewToLightProjectionMatrix.GetD3DXMatrix());
    l_Effect->SetMatrix(m_Effect->GetViewToLightProjectionMatrixParameter(), &l_ViewToLightProjectionMatrix.GetD3DXMatrix());
  }
  if (m_UseTime) {
    l_Effect->SetFloat(m_Effect->GetTimeParameter(), CCORE->GetElapsedTime());
  }
  if (m_UseTick) {
    l_Effect->SetFloat(m_Effect->GetTickParameter(), m_tick);
  }
  if (m_ChangeUV) {
    l_Effect->SetBool(m_Effect->GetChangeUVParameter(), m_UV ? TRUE : FALSE);
  }
  if (m_Vanish) {
    m_Effect->SetVanishingModifier();
  }
  if (m_UseScreenSize) {
    int l_Width = 0;
    int l_Height = 0;
    D3DVIEWPORT9 *l_viewPort = new D3DVIEWPORT9();
    GRAPHM->GetDevice()->GetViewport(l_viewPort);
    l_Width = l_viewPort->Width;
    l_Height = l_viewPort->Height;
    int l_screenSize[2] = {l_Width, l_Height};
    l_Effect->SetIntArray(m_Effect->GetScreenSizeParameter(), l_screenSize, 2);
    CHECKED_DELETE(l_viewPort);
  }
  return true;
}
bool CEffectTechnique::Refresh() {
  m_D3DTechnique = m_Effect->GetTechniqueByName(m_TechniqueName);
  return false;
}
//DirectX Methods Interface
D3DXHANDLE CEffectTechnique::GetD3DTechnique() {
  D3DXHANDLE l_EffectTechnique = m_Effect->GetTechniqueByName(m_TechniqueName.c_str());
  return l_EffectTechnique;
}