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

  CRenderableObjectsManager *l_Rom = RENDLM->GetRenderableObjectsManagerByStr("poly");
  l_Rom->GetResourcesVector();
  for (int i = 0; i < l_Rom->GetResourcesVector().size(); ++i) {
    CPolyPlatform *poly = (CPolyPlatform *) l_Rom->GetResourcesVector()[i];
    if (poly->getEnabled()) {
      m_Effect->SetActivePoly(poly);
      poly->Render(&RM);
    }
  }


}