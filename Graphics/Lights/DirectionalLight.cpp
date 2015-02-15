#include "DirectionalLight.h"
#include "Core\Core.h"
#include "Effects\EffectManager.h"
#include <d3dx9.h>
#include "Camera\OrthoFixedCameraController.h"
#include "Camera\OrthoFixedCamera.h"
#include "GraphicsManager.h"


/*CDirectionalLight::CDirectionalLight() : CLight()
{
	//D3DXMatrixOrthoLH(D3DXMATRIX *pOut, FLOAT w, FLOAT h, FLOAT zn,FLOAT zf )

	D3DXMatrixOrthoLH(l_D3dMatrix, m_OrthoShadowMapSize.x, m_OrthoShadowMapSize.y, 150.f,0.1f ); //se pillan del jodio XML de la luz!! ZFar Valor de atenuacion
}*/

void CDirectionalLight::Render(CGraphicsManager *RM)
{
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

void CDirectionalLight::SetShadowMap(CGraphicsManager *RM)
{
	COrthoFixedCameraController l_OrthoFixedCameraController(m_Position - m_Direction, 
		m_Position, m_OrthoShadowMapSize.x, m_OrthoShadowMapSize.y, 1.0f, m_EndRangeAttenuation);
	CEffectManager *l_EffectManager=CCORE->GetEffectManager(); //CEffectManager &l_EffectManager=CCORE->GetEffectManager();
	COrthoFixedCamera l_Camera = l_OrthoFixedCameraController.GetCamera();
	m_ViewShadowMap= l_Camera.GetViewMatrix();  
	m_ProjectionShadowMap= l_Camera.GetProjectionMatrix();
	l_EffectManager->ActivateCamera(m_ViewShadowMap, m_ProjectionShadowMap, l_Camera.GetEye());//l_Camera.GetPosition()
		
}
