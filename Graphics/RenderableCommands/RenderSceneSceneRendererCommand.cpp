#include "RenderableCommands\RenderSceneSceneRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "GraphicsManager.h"
#include "Renderable\RenderableObjectsLayersManager.h"
#include "Core\Core.h"
#include "Core\Process.h"
#include "Core_Utils/MemLeaks.h"

CRenderSceneSceneRendererCommand::CRenderSceneSceneRendererCommand(CXMLTreeNode &atts) {
  m_Layer = atts.GetPszProperty("layer", "solid");
  m_Active = atts.GetBoolProperty("active", true);
}
void CRenderSceneSceneRendererCommand::Execute(CGraphicsManager &RM) {
  RENDLM->setCurrentLayer(m_Layer);
  if (m_Active) {
    CCORE->Render();
    RENDLM->Render(GRAPHM, RENDLM->getCurrentLayer());
  }
}