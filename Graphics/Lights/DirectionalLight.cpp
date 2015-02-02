#include "DirectionalLight.h"


void CDirectionalLight::Render(CGraphicsManager *RM)
{
	//Debug
	/*Mat44f t;
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
	
	printf("I'm a directional Light \n");*/
	
}

void CDirectionalLight::SetShadowMap(CGraphicsManager *RM)
{
	/*COrthoFixedCameraController l_OrthoFixedCameraController(m_Position - m_Direction, 
		m_Position, m_OrthoShadowMapSize.x, m_OrthoShadowMapSize.y, 1.0f, m_EndRangeAttenuation);
	CEffectManager &l_EffectManager=CORE->GetEffectManager();
	m_ViewShadowMap= l_Camera.GetViewMatrix();
	m_ProjectionShadowMap= l_Camera.GetProjectionMatrix();
	l_EffectManager.ActivateCamera(m_ViewShadowMap, m_ProjectionShadowMap,
	l_Camera.GetPosition());*/
}
