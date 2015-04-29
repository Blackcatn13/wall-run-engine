#include "RenderableCommands\GenerateCascadeShadowMapsSceneRendererCommand.h"
#include "Core\Core.h"
#include "GraphicsManager.h"
#include "Lights\LightManager.h"
#include "Lights\Light.h"
#include "Lights\DirectionalLight.h"
#include "Effects\Effect.h"
#include "Effects\EffectManager.h"
#include "Core_Utils/MemLeaks.h"
#include "Renderable\RenderableObjectTechniqueManager.h"
#include "Renderable\RenderableObjectTechnique.h"

CGenerateCascadeShadowMapsSceneRendererCommand::CGenerateCascadeShadowMapsSceneRendererCommand(CXMLTreeNode &atts)
{
	m_orthoScale = atts.GetFloatProperty("ortho_scale_per_loop", 1.0, false);
	m_textureScale = atts.GetFloatProperty("texture_scale_per_loop", 1.0, false);
	m_loops = atts.GetIntProperty("loops", 1, false);
}

void CGenerateCascadeShadowMapsSceneRendererCommand::Execute(CGraphicsManager &RM)
{
    CLightManager::TMapResource::iterator it = LIGHTM->GetResources().begin();
    std::map<std::string, CLight*> resources = LIGHTM->GetResources();
    //CRenderableObjectTechniqueManager::TMapResource::iterator it2 = RENDTECHM->GetResources().begin();
    CEffect *eff = EFFECTM->GetEffect("ShadowMapGenerate");
	Vect2f lastOrthoSize = Vect2f(-1,-1);
	int lastShadowWidth = -1;
	int lastShadowHeight = -1;
	float scaleOrthoLoop = m_orthoScale;
	float scaleTextureLoop = m_textureScale;
	for(int i = 0; i<m_loops; i++){
		while (it != LIGHTM->GetResources().end()) {
			if (it->second->GetGenerateDynamicShadowMap()) {
				scaleOrthoLoop *= i;
				scaleTextureLoop *= i;
				if(scaleOrthoLoop > 0 && scaleTextureLoop > 0)
				{
					if(lastOrthoSize.x == -1 && lastOrthoSize == -1)
					{
						lastOrthoSize.x = ((CDirectionalLight*)(it->second))->GetOrthoShadowMapSize().x;
						lastOrthoSize.y = ((CDirectionalLight*)(it->second))->GetOrthoShadowMapSize().y;
					}
					if(lastShadowWidth == -1)
					{
						lastShadowWidth = it->second->getShadowMapWidth();
					}
					if(lastShadowHeight == -1)
					{
						lastShadowHeight = it->second->getShadowMapHeigth();
					}
					((CDirectionalLight*)(it->second))->SetOrthoShadowMapSize(lastOrthoSize * scaleOrthoLoop);
					it->second->setShadowMapHeigth(lastShadowHeight * scaleTextureLoop);
					it->second->setShadowMapWidth(lastShadowWidth * scaleTextureLoop);

				}
				eff->ChangeLight(it->second);
				it->second->GenerateShadowMap(&RM);
			}
			it++;
		}
	}
	if(lastOrthoSize.x != -1 && lastOrthoSize != -1)
	{
		((CDirectionalLight*)(it->second))->SetOrthoShadowMapSize(lastOrthoSize);
	}
	if(lastShadowWidth != -1)
	{
		it->second->setShadowMapWidth(lastShadowWidth);
	}
	if(lastShadowHeight != -1)
	{
		it->second->setShadowMapHeigth(lastShadowHeight);
	}
}