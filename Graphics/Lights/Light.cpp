#include "Lights\Light.h"
#include "GraphicsManager.h"
#include "Effects\EffectManager.h"
#include "Effects\Effect.h"
#include "Renderable\RenderableObjectsManager.h"
#include "Texture\Texture.h"
#include "Core_Utils/MemLeaks.h"
#include "Renderable\RenderableObjectsLayersManager.h"
#include "Core\Core.h"
#include "Core\PlayerController.h"

CLight::CLight()
  : m_ShadowMaskTexture(NULL),
    m_GenerateDynamicShadowMap (false),
    m_GenerateStaticShadowMap (false),
    m_MustUpdateStaticShadowMap (false) {

  for (int i = 0; i < MAX_SHADOWMAPS; i++) {
    m_DynamicShadowMap[i] = NULL;
    m_StaticShadowMap[i] = NULL;
  }
}

CLight::CLight(CXMLTreeNode &Node)
  : m_Color (Node.GetVect3fProperty("color", (0.0f, 0.0f, 0.0f)))
  , m_EndRangeAttenuation (Node.GetFloatProperty("att_end_range", 0.0f))
  , m_EnableDistance (Node.GetFloatProperty("enable_distance", 5000.0f))
  , m_FormatType (Node.GetPszISOProperty("shadow_map_format_type", "", false))
  , m_GenerateDynamicShadowMap (Node.GetBoolProperty("generate_shadow_map", false))
  , m_GenerateStaticShadowMap (Node.GetBoolProperty("generate_static_shadow_map", false))
  , m_Intensity (Node.GetFloatProperty("intensity", 0.0f))
  , m_MustUpdateStaticShadowMap (Node.GetBoolProperty("update_static_shadow_map", false, false))
  , m_Name (Node.GetPszISOProperty("name", ""))
  , m_ProjectionShadowMap (Mat44f())
  , m_RenderShadows (false)
  , m_ShadowMapHeigth (Node.GetIntProperty("shadow_map_height", 0, false))
  , m_ShadowMapWidth (Node.GetIntProperty("shadow_map_width", 0, false))
  , m_StartRangeAttenuation (Node.GetFloatProperty("att_start_range", 0.0f))
  , m_nShadowmaps(0)
  , m_ViewShadowMap (Mat44f()) {

  for (int i = 0; i < MAX_SHADOWMAPS; i++) {
    m_DynamicShadowMap[i] = NULL;
    m_StaticShadowMap[i] = NULL;
  }
  m_Position = Node.GetVect3fProperty("pos", (0.0f, 0.0f, 0.0f));
  SetShadowMaskTexture(Node.GetPszISOProperty("shadow_texture_mask", "", false));
  int childs = Node.GetNumChildren();
  CRenderableObjectsManager *l_RenderableObjectManager;
  for (int i = 0; i < childs; ++i) {
    if (std::string("dynamic") == Node(i).GetName()) {
      int dynamicChilds = Node(i).GetNumChildren();
      if (dynamicChilds > 0) {
        for (int j = 0; j < dynamicChilds; ++j) {
          renderableDynamicMaps.push_back(Node(i)(j).GetPszISOProperty("renderable_objects_manager", "", false));
          l_RenderableObjectManager = RENDLM->GetResource(Node(i)(j).GetPszISOProperty("renderable_objects_manager", "", false));
          m_DynamicShadowMapRenderableObjectsManagers.push_back(l_RenderableObjectManager);
        }
      } else {
        renderableDynamicMaps.push_back(Node(i).GetPszISOProperty("renderable_objects_manager", "", false));
        l_RenderableObjectManager = RENDLM->GetResource(Node(i).GetPszISOProperty("renderable_objects_manager", "", false));
        m_DynamicShadowMapRenderableObjectsManagers.push_back(l_RenderableObjectManager);
      }
    }
    if (std::string("static") == Node(i).GetName()) {
      int dynamicChilds = Node(i).GetNumChildren();
      if (dynamicChilds > 0) {
        for (int j = 0; j < dynamicChilds; ++j) {
          renderableStaticMaps.push_back(Node(i)(j).GetPszISOProperty("renderable_objects_manager", "", false));
          l_RenderableObjectManager = RENDLM->GetResource(Node(i)(j).GetPszISOProperty("renderable_objects_manager", "", false));
          m_StaticShadowMapRenderableObjectsManagers.push_back(l_RenderableObjectManager);
        }
      } else {
        renderableStaticMaps.push_back(Node(i).GetPszISOProperty("renderable_objects_manager", "", false));
        l_RenderableObjectManager = RENDLM->GetResource(Node(i).GetPszISOProperty("renderable_objects_manager", "", false));
        m_StaticShadowMapRenderableObjectsManagers.push_back(l_RenderableObjectManager);
      }
    }
  }
}

void CLight::Init() {
  if (m_GenerateStaticShadowMap) {
    for (int i = 0; i < MAX_SHADOWMAPS; i++) {
      m_StaticShadowMap[i] = new CTexture();
      std::string l_StaticShadowMapTextureName = "Static_" + m_Name;
      m_StaticShadowMap[i]->Create(l_StaticShadowMapTextureName, m_ShadowMapWidth, m_ShadowMapHeigth, 1, CTexture::TUsageType::RENDERTARGET, CTexture::TPoolType::DEFAULT, m_StaticShadowMap[i]->GetFormatTypeFromString(m_FormatType), true);
    }
  }
  if (m_GenerateDynamicShadowMap) {
    for (int i = 0; i < MAX_SHADOWMAPS; i++) {
      m_DynamicShadowMap[i] = new CTexture();
      std::string l_DinamicShadowMapTextureName = "Dynamic_" + m_Name;
      m_DynamicShadowMap[i]->Create(l_DinamicShadowMapTextureName, m_ShadowMapWidth, m_ShadowMapHeigth, 1, CTexture::TUsageType::RENDERTARGET, CTexture::TPoolType::DEFAULT, m_DynamicShadowMap[i]->GetFormatTypeFromString(m_FormatType), true);
    }
  }
}

CLight::~CLight() {
  for (int i = 0; i < MAX_SHADOWMAPS; i++) {
    CHECKED_DELETE(m_StaticShadowMap[i]);
    CHECKED_DELETE(m_DynamicShadowMap[i]);
  }
  CHECKED_DELETE(m_ShadowMaskTexture);
}


void CLight::Render(CGraphicsManager *RM) {
  //TODO A ver que tiene que hacer esto
  //Debug. Printar un eje y alguna luz con la dirección
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

void CLight::GenerateShadowMap(CGraphicsManager *RM, int index /*=0*/) {
  SetShadowMap(RM);
  if (m_GenerateStaticShadowMap && m_MustUpdateStaticShadowMap) {
    m_StaticShadowMap[index]->SetAsRenderTarget(0);
    // RM->BeginRendering();
    RM->BeginRenderCommand();
    //   RM->Clear(true, true, true, 0xffffffff);
    RM->ClearSceneCommand(true, true, true);
    for (size_t i = 0; i < m_StaticShadowMapRenderableObjectsManagers.size(); ++i)
      m_StaticShadowMapRenderableObjectsManagers[i]->Render(RM);
    m_MustUpdateStaticShadowMap = false;
    //RM->EndRendering();
    RM->EndRenderCommand();
    m_StaticShadowMap[index]->UnsetAsRenderTarget(0);
  }
  if (m_DynamicShadowMapRenderableObjectsManagers.size() > 0) {
    m_DynamicShadowMap[index]->SetAsRenderTarget(0);
    //   RM->BeginRendering();
    RM->BeginRenderCommand();
    //RM->Clear(true, true, true, 0xffffffff);
    RM->ClearSceneCommand(true, true, true);
    for (size_t i = 0; i <
         m_DynamicShadowMapRenderableObjectsManagers.size(); ++i)
      m_DynamicShadowMapRenderableObjectsManagers[i]->Render(RM);
    //   RM->EndRendering();
    RM->EndRenderCommand();
    m_DynamicShadowMap[index]->UnsetAsRenderTarget(0);
    bool save = false;
    if ( save) {
      m_DynamicShadowMap[index]->SaveToFile("D:\\test\\test.png");
    }
  }
}

void CLight::SetShadowMaskTexture(std::string textureName) {
  if (textureName != "") {
    m_ShadowMaskTexture = new CTexture();
    m_ShadowMaskTexture->Load(textureName);
  } else {
    m_ShadowMaskTexture = NULL;
  }
}

void CLight::BeginRenderEffectManagerShadowMap(CEffect *Effect) {
  //Faltan las mascaras
  //SHADOW_MAP_MASK_STAGE=>5
  //STATIC_SHADOW_MAP_STAGE 6
  //DYNAMIC_SHADOW_MAP_STAGE 7
  CEffectManager *l_EM = CCORE->GetEffectManager();
  l_EM->SetLightViewMatrix(m_ViewShadowMap);
  l_EM->SetShadowProjectionMatrix(m_ProjectionShadowMap);
  if (m_ShadowMaskTexture != NULL)
    m_ShadowMaskTexture->Activate(5/*SHADOW_MAP_MASK_STAGE*/);

  if (m_GenerateStaticShadowMap) {
    for (int i = 0; i < m_nShadowmaps; ++i) {
      m_StaticShadowMap[0]->Activate(2 * i + 6/*STATIC_SHADOW_MAP_STAGE*/);
    }
  }
  if (m_GenerateDynamicShadowMap) {
    for (int i = 0; i < m_nShadowmaps; ++i) {
      m_DynamicShadowMap[0]->Activate(2 * i + 7/*DYNAMIC_SHADOW_MAP_STAGE*/);
    }
  }
  Effect->SetShadowMapParameters(m_ShadowMaskTexture != NULL,
                                 m_GenerateStaticShadowMap, m_GenerateDynamicShadowMap &&
                                 m_DynamicShadowMapRenderableObjectsManagers.size() != 0,
                                 m_nShadowmaps);
}


void CLight::ReloadRO() {
  m_StaticShadowMapRenderableObjectsManagers.clear();
  m_DynamicShadowMapRenderableObjectsManagers.clear();
  CRenderableObjectsManager *l_RenderableObjectManager;
  for (size_t i = 0; i < renderableDynamicMaps.size(); ++i) {
    l_RenderableObjectManager = RENDLM->GetResource(renderableDynamicMaps[i]);
    m_DynamicShadowMapRenderableObjectsManagers.push_back(l_RenderableObjectManager);
  }
  for (size_t i = 0; i < renderableStaticMaps.size(); ++i) {
    l_RenderableObjectManager = RENDLM->GetResource(renderableStaticMaps[i]);
    m_StaticShadowMapRenderableObjectsManagers.push_back(l_RenderableObjectManager);
  }
}

bool CLight::IsEnabled()
{
	bool enabled = false;
	if(PLAYC != NULL)
	{
		Vect3f playerPos = PLAYC->GetPosition();
		if(m_Position.Distance(playerPos) < m_EnableDistance)
		{
			enabled = true;
		}
	}
	return enabled;
}
