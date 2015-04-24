#include "DirectionalLight.h"
#include "Core\Core.h"
#include "Effects\EffectManager.h"
#include <d3dx9.h>
#include "Camera\OrthoFixedCameraController.h"
#include "Camera\OrthoFixedCamera.h"
#include "GraphicsManager.h"
#include "Core_Utils/MemLeaks.h"

/*CDirectionalLight::CDirectionalLight() : CLight()
{
	//D3DXMatrixOrthoLH(D3DXMATRIX *pOut, FLOAT w, FLOAT h, FLOAT zn,FLOAT zf )

	D3DXMatrixOrthoLH(l_D3dMatrix, m_OrthoShadowMapSize.x, m_OrthoShadowMapSize.y, 150.f,0.1f ); //se pillan del jodio XML de la luz!! ZFar Valor de atenuacion
}*/

CDirectionalLight::CDirectionalLight(CXMLTreeNode &Node)
  : CLight(Node)
  , m_Direction(Node.GetVect3fProperty("dir", (0.0f, 0.0f, 0.0f)))
  , m_OrthoShadowMapSize(Vect2f(Node.GetIntProperty("ortho_shadow_map_width", 0, false), Node.GetIntProperty("ortho_shadow_map_height", 0, false))) {
  m_Type = TLightType::DIRECTIONAL;
}

void CDirectionalLight::Render(CGraphicsManager *RM) {
  //Debug
  Mat44f t;
  t.SetIdentity();
  CColor l_Color =  CColor(GetColor().GetRed(), GetColor().GetGreen(), GetColor().GetBlue());
  Vect3f l_FinalPosition = GetDirection().Normalize() * GetStartRangeAttenuation();
  RM->SetTransform(t);
  RM->DrawLine(GetPosition(), l_FinalPosition, l_Color);
  Mat44f l_trans;
  l_trans.SetIdentity();
  l_trans.Translate(Vect3f(GetPosition()));
  RM->SetTransform(l_trans);
  RM->DrawAxis(1.0f);
  printf("I'm a directional Light \n");
}

void CDirectionalLight::SetShadowMap(CGraphicsManager *RM) {
  /*COrthoFixedCameraController l_OrthoFixedCameraController(m_Position - m_Direction,
          m_Position, m_OrthoShadowMapSize.x, m_OrthoShadowMapSize.y, 1.0f, m_EndRangeAttenuation);

  COrthoFixedCamera l_Camera = l_OrthoFixedCameraController.GetCamera();
  m_ViewShadowMap = l_Camera.GetViewMatrix();
  m_ProjectionShadowMap = l_Camera.GetProjectionMatrix();*/
  D3DXMATRIX l_view;
  D3DXMATRIX l_Ortho;
  D3DXVECTOR3 l_Eye(m_Position.x, m_Position.y, m_Position.z);
  Vect3f lookAt = m_Position + m_Direction;
  m_Direction.Normalize();
  D3DXVECTOR3 l_LookAt(lookAt.x, lookAt.y, lookAt.z);
  Vect3f l_vRight = (m_Direction ^ Vect3f(0, 1, 0)).Normalize();
  Vect3f l_vUP = (l_vRight ^ m_Direction).Normalize();
  D3DXVECTOR3 l_UP = D3DXVECTOR3(l_vUP.x, l_vUP.y, l_vUP.z);
  D3DXMatrixLookAtLH(&l_view, &l_Eye, &l_LookAt, &l_UP);
  m_ViewShadowMap = Mat44f(l_view);
  D3DXMatrixOrthoLH(&l_Ortho, m_OrthoShadowMapSize.x, m_OrthoShadowMapSize.y, 0.01f, m_EndRangeAttenuation);
  m_ProjectionShadowMap = Mat44f(l_Ortho);
  CEffectManager *l_EffectManager = CCORE->GetEffectManager();
  l_EffectManager->ActivateCamera(m_ViewShadowMap, m_ProjectionShadowMap, m_Position);//l_Camera.GetPosition()
  RM->GetDevice()->SetTransform( D3DTS_VIEW, &l_view );
  RM->GetDevice()->SetTransform( D3DTS_PROJECTION, &l_Ortho );
  RM->UpdateFrustum(l_view * l_Ortho);
}
