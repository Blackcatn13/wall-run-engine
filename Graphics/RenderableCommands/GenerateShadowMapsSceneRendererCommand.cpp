#include "RenderableCommands\GenerateShadowMapsSceneRendererCommand.h"
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



CGenerateShadowMapsSceneRendererCommand::CGenerateShadowMapsSceneRendererCommand(CXMLTreeNode &atts)
{
}

void CGenerateShadowMapsSceneRendererCommand::Execute(CGraphicsManager &RM)
{
    CLightManager::TMapResource::iterator it = LIGHTM->GetResources().begin();
    std::map<std::string, CLight*> resources = LIGHTM->GetResources();
    //CRenderableObjectTechniqueManager::TMapResource::iterator it2 = RENDTECHM->GetResources().begin();
    CEffect *eff = EFFECTM->GetEffect("ShadowMapGenerate");
    while (it != LIGHTM->GetResources().end()) {
        if (it->second->GetGenerateDynamicShadowMap()) {
            eff->ChangeLight(it->second);
			it->second->SetNShadowmap(1);
            it->second->GenerateShadowMap(&RM);
        }
        it++;
    }
}