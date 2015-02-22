#include "Lights\Light.h"
#include "GraphicsManager.h"
#include "Effects\EffectManager.h"
#include "Effects\Effect.h"
#include "Renderable\RenderableObjectsManager.h"
#include "Texture\Texture.h"
#include "Core_Utils/MemLeaks.h"


CLight::CLight()
    : m_StaticShadowMap(NULL),
      m_DynamicShadowMap(NULL),
      m_ShadowMaskTexture(NULL)
{
}

void CLight::Init()
{
    m_StaticShadowMap = new CTexture();
    std::string l_StaticShadowMapTextureName = "Static_" + m_Name;
    m_StaticShadowMap->Create(l_StaticShadowMapTextureName, m_ShadowMapWidth, m_ShadowMapHeigth, 0, CTexture::TUsageType::RENDERTARGET, CTexture::TPoolType::DEFAULT, m_StaticShadowMap->GetFormatTypeFromString(m_FormatType));
    m_DynamicShadowMap = new CTexture();
    std::string l_DinamicShadowMapTextureName = "Dynamic_" + m_Name;
    m_DynamicShadowMap->Create(l_DinamicShadowMapTextureName, m_ShadowMapWidth, m_ShadowMapHeigth, 0, CTexture::TUsageType::RENDERTARGET, CTexture::TPoolType::DEFAULT, m_DynamicShadowMap->GetFormatTypeFromString(m_FormatType));
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
    //Debug. Printar un eje y alguna luz con la direcci�n
    Mat44f t;
    t.SetIdentity();
    CColor l_Color =  CColor(GetColor().GetRed(), GetColor().GetGreen(), GetColor().GetBlue());
    RM->SetTransform(t);
    Mat44f l_trans;
    l_trans.SetIdentity();
    l_trans.Translate(Vect3f(GetPosition()));
    RM->SetTransform(l_trans);
    RM->DrawCube(0.5f, l_Color);
    printf("I'm a light \n");
}

void CLight::GenerateShadowMap(CGraphicsManager *RM)
{
    SetShadowMap(RM);
    if (m_GenerateStaticShadowMap && m_MustUpdateStaticShadowMap) {
        m_StaticShadowMap->SetAsRenderTarget(0);
        // RM->BeginRendering();
        RM->BeginRenderCommand();
        //   RM->Clear(true, true, true, 0xffffffff);
        RM->ClearSceneCommand(true, true, true);
        for (size_t i = 0; i < m_StaticShadowMapRenderableObjectsManagers.size(); ++i)
            m_StaticShadowMapRenderableObjectsManagers[i]->Render(RM);
        m_MustUpdateStaticShadowMap = false;
        //RM->EndRendering();
        RM->EndRenderCommand();
        m_StaticShadowMap->UnsetAsRenderTarget(0);
    }
    if (m_DynamicShadowMapRenderableObjectsManagers.size() > 0) {
        m_DynamicShadowMap->SetAsRenderTarget(0);
        //   RM->BeginRendering();
        RM->BeginRenderCommand();
        //RM->Clear(true, true, true, 0xffffffff);
        RM->ClearSceneCommand(true, true, true);
        for (size_t i = 0; i <
             m_DynamicShadowMapRenderableObjectsManagers.size(); ++i)
            m_DynamicShadowMapRenderableObjectsManagers[i]->Render(RM);
        //   RM->EndRendering();
        RM->EndRenderCommand();
        m_DynamicShadowMap->UnsetAsRenderTarget(0);
    }
}

void CLight::SetShadowMaskTexture(std::string textureName)
{
    m_ShadowMaskTexture = new CTexture();
    m_ShadowMaskTexture->Load(textureName);
}

void CLight::BeginRenderEffectManagerShadowMap(CEffect *Effect)
{
    //Faltan las mascaras
    //SHADOW_MAP_MASK_STAGE=>5
    //STATIC_SHADOW_MAP_STAGE 6
    //DYNAMIC_SHADOW_MAP_STAGE 7
    CEffectManager * l_EM = CCORE->GetEffectManager();
    l_EM->SetLightViewMatrix(m_ViewShadowMap);
    l_EM->SetShadowProjectionMatrix(m_ProjectionShadowMap);
    if (m_ShadowMaskTexture != NULL)
        m_ShadowMaskTexture->Activate(5/*SHADOW_MAP_MASK_STAGE*/);
    if (m_GenerateStaticShadowMap)
        m_StaticShadowMap->Activate(6/*STATIC_SHADOW_MAP_STAGE*/);
    if (m_GenerateDynamicShadowMap)
        m_DynamicShadowMap->Activate(7/*DYNAMIC_SHADOW_MAP_STAGE*/);
    Effect->SetShadowMapParameters(m_ShadowMaskTexture != NULL,
                                   m_GenerateStaticShadowMap, m_GenerateDynamicShadowMap &&
                                   m_DynamicShadowMapRenderableObjectsManagers.size() != 0);
}



