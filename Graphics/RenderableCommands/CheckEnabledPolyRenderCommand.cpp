#include "RenderableCommands\CheckEnabledPolyRenderCommand.h"
#include "Effects\Effect.h"
#include "Effects\EffectManager.h"
#include "Core\Core.h"
#include "Core_Utils/MemLeaks.h"
#include "SceneElements\PolyPlatform.h"
#include "Core\Core.h"
#include "Renderable\RenderableObjectsLayersManager.h"

CCheckEnabledPolyRenderCommand::CCheckEnabledPolyRenderCommand(CXMLTreeNode &atts) {
  m_Effect = EFFECTM->GetEffect("PolyEffect");
}
void CCheckEnabledPolyRenderCommand::Execute(CGraphicsManager &RM) {

  CRenderableObjectsManager *l_RomPoly = RENDLM->GetRenderableObjectsManagerByStr("poly");
  CRenderableObjectsManager *l_RomPolyEnabled = RENDLM->GetRenderableObjectsManagerByStr("enabled_poly");
  CheckLayerChange(l_RomPoly, l_RomPolyEnabled, true );

  CheckLayerChange(l_RomPolyEnabled, l_RomPoly, false);

}


void CCheckEnabledPolyRenderCommand::CheckLayerChange(CRenderableObjectsManager *rolm1, CRenderableObjectsManager *rolm2, bool polyEnabled) {

  if (rolm1 != NULL) {
    for (int i = 0; i < rolm1->GetResourcesVector().size(); ++i) {
      CPolyPlatform *poly = (CPolyPlatform *) rolm1->GetResourcesVector()[i];
      if (poly->getEnabled() == polyEnabled) {
        // m_Effect->SetActivePoly(poly);
        //poly->Render(&RM);
        if (rolm2->GetResource(poly->getName()) == NULL) {
          rolm2->AddResource(poly->getName(), poly);
          rolm1->RemoveFromResource(poly->getName());
        }
      }
    }
  }
}