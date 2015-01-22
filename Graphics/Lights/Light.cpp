#include "Lights\Light.h"
#include "GraphicsManager.h"
#include "Core\Core.h"
#include "Effects\EffectManager.h"

class CEffectManager;

CLight::CLight()
{
	m_StaticShadowMap = NULL;
	m_DynamicShadowMap = NULL;
	m_ShadowMaskTexture = NULL;
}

CLight::~CLight()
{
	CHECKED_DELETE(m_StaticShadowMap);
	CHECKED_DELETE(m_DynamicShadowMap);
	CHECKED_DELETE(m_ShadowMaskTexture);
}


void CLight::Render(CGraphicsManager *RM)
{
	//TODO A ver que tiene que hacer esto
	//Debug. Printar un eje y alguna luz con la dirección
		Mat44f t;
	t.SetIdentity();
	CColor l_Color =  CColor(GetColor().GetRed(), GetColor().GetGreen(), GetColor().GetBlue());
	GRAPHM->SetTransform(t);
	Mat44f l_trans;
	l_trans.SetIdentity();
	l_trans.Translate(Vect3f(GetPosition()));
	GRAPHM->SetTransform(l_trans);
	GRAPHM->DrawCube(0.5f, l_Color);
	printf("I'm a light \n");
}

void CLight::GenerateShadowMap(CGraphicsManager *RM)
{
	SetShadowMap(RM);
	if(m_GenerateStaticShadowMap && m_MustUpdateStaticShadowMap)
	{
	m_StaticShadowMap->SetAsRenderTarget(0);
	RM->BeginRendering();
	//RM->Clear(true, true, true, 0xffffffff);
	for(size_t i=0;i< m_StaticShadowMapRenderableObjectsManagers.size();++i)
		m_StaticShadowMapRenderableObjectsManagers[i]->Render(RM);
		m_MustUpdateStaticShadowMap=false;
		RM->EndRendering();
		m_StaticShadowMap->UnsetAsRenderTarget(0);
	}
	if(m_DynamicShadowMapRenderableObjectsManagers.size()>0)
	{
		m_DynamicShadowMap->SetAsRenderTarget(0);
		RM->BeginRendering();
		//RM->Clear(true, true, true, 0xffffffff);
		for(size_t i=0;i<
		m_DynamicShadowMapRenderableObjectsManagers.size();++i)
		m_DynamicShadowMapRenderableObjectsManagers[i]->Render(RM);
		RM->EndRendering();
		m_DynamicShadowMap->UnsetAsRenderTarget(0);
	}
}



void CLight::BeginRenderEffectManagerShadowMap(CEffect *Effect)
{
	if(m_GenerateDynamicShadowMap)
	{
		/*
		CEffectManager * l_EM=CCORE->GetEffectManager();
		l_EM.SetLightViewMatrix(m_ViewShadowMap);
		l_EM.SetShadowProjectionMatrix(m_ProjectionShadowMap);
		if(m_ShadowMaskTexture!=NULL)
		m_ShadowMaskTexture->Activate(SHADOW_MAP_MASK_STAGE);
		if(m_GenerateStaticShadowMap)
		m_StaticShadowMap->Activate(STATIC_SHADOW_MAP_STAGE);
		m_DynamicShadowMap->Activate(DYNAMIC_SHADOW_MAP_STAGE);
		Effect->SetShadowMapParameters(m_ShadowMaskTexture!=NULL,
		m_GenerateStaticShadowMap, m_GenerateDynamicShadowMap &&
		m_DynamicShadowMapRenderableObjectsManagers.size()!=0);*/
	}


}



