#include "DirectionalLight.h"


void CDirectionalLight::Render(CGraphicsManager *RM)
{
	//TODO A ver que hace este tb
	printf("I'm a directional Light \n");
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
