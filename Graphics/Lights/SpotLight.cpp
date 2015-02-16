#include "Lights\SpotLight.h"
#include "Camera\FPSCamera.h"
#include "Object\Object3D.h"
#include "Core/Core.h"
#include "Effects/EffectManager.h"
#include "GraphicsManager.h"
#include "Effects\EffectManager.h"
#include "Core\Core.h"


CSpotLight::CSpotLight() {}


void CSpotLight::SetShadowMap(CGraphicsManager *RM)
{
    CObject3D *l_Object = new CObject3D(GetPosition(), 0.f, 0.f);
    m_Camera = CFPSCamera(0.1, GetEndRangeAttenuation(), 45.0f * D3DX_PI / 180.0f, 1.f, l_Object);
    CEffectManager *l_EffectManager = CCORE->GetEffectManager(); //CEffectManager &l_EffectManager=CCORE->GetEffectManager();
    D3DXMATRIX m_matView;
    D3DXVECTOR3 l_Eye(m_Camera.GetEye().x, m_Camera.GetEye().y, m_Camera.GetEye().z);
    D3DXVECTOR3 l_LookAt(m_Camera.GetLookAt().x, m_Camera.GetLookAt().y, m_Camera.GetLookAt().z);
    D3DXVECTOR3 l_VUP(m_Camera.GetVecUp().x, m_Camera.GetVecUp().y, m_Camera.GetVecUp().z);
    D3DXMatrixLookAtLH( &m_matView, &l_Eye, &l_LookAt, &l_VUP);
    m_ViewShadowMap = (Mat44f) m_matView;
    D3DXMATRIX m_matProject;
    D3DXMatrixPerspectiveFovLH(	&m_matProject, m_Camera.GetFov(), m_Camera.GetAspectRatio(),/*(float)(m_uWidth / m_uHeight),*/
                                /*m_Camera.GetZn()*/0.1f, m_Camera.GetZf());
    m_ProjectionShadowMap = (Mat44f)m_matProject;
//	l_EffectManager->ActivateCamera(m_ViewShadowMap, m_ProjectionShadowMap, l_Camera.GetEye());//l_Camera.GetPosition()
    //D3DXMatrixLookAtLH( &m_matView, &l_Eye, &l_LookAt, &l_VUP);
    //      //Setup Matrix projection
    //      D3DXMatrixPerspectiveFovLH(	&m_matProject, camera->GetFov(), camera->GetAspectRatio(),/*(float)(m_uWidth / m_uHeight),*/
    //                                  camera->GetZn(), camera->GetZf());
}


