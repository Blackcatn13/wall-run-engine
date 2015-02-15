#include "RenderableCommands\GenerateShadowMapsSceneRendererCommand.h"
#include "Core\Core.h"
#include "GraphicsManager.h"
#include "Lights\LightManager.h"
#include "Lights\Light.h"
#include "Effects\Effect.h"
#include "Effects\EffectManager.h"



CGenerateShadowMapsSceneRendererCommand::CGenerateShadowMapsSceneRendererCommand(CXMLTreeNode &atts)
{
}

void CGenerateShadowMapsSceneRendererCommand::Execute(CGraphicsManager &RM)
{
    CLightManager::TMapResource::iterator it = LIGHTM->GetResources().begin();
    std::map<std::string, CLight*> resources = LIGHTM->GetResources();
    //CRenderableObjectTechniqueManager::TMapResource::iterator it2 = RENDTECHM->GetResources().begin();
    while (it != LIGHTM->GetResources().end()) {
        if (it->second->GetGenerateDynamicShadowMap()) {
            //while (it2 != RENDTECHM->GetResources().end())
            //{
            //it->second->BeginRenderEffectManagerShadowMap(it2->second->GetEffectTechnique()->GetEffect());
            it->second->BeginRenderEffectManagerShadowMap(EFFECTM->GetEffect("shadowMapEffect"));
            it->second->SetShadowMap(&RM);
            it->second->GenerateShadowMap(&RM);
            //}
        }
    }
}