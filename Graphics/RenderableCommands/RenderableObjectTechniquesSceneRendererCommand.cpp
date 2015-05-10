#include "RenderableCommands\RenderableObjectTechniquesSceneRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "GraphicsManager.h"
#include "Renderable\PoolRenderableObjectTechnique.h"
#include "Renderable\RenderableObjectTechniqueManager.h"
#include "Core\Core.h"
#include "Core_Utils/MemLeaks.h"

CRenderableObjectTechniquesSceneRendererCommand::CRenderableObjectTechniquesSceneRendererCommand(CXMLTreeNode &atts) {
  m_PoolName = atts.GetPszProperty("pool", "");
  m_PoolRenderableObjectTechnique = RENDTECHM->GetPoolRenderableObjectTechniques().GetResource(m_PoolName);
}
void CRenderableObjectTechniquesSceneRendererCommand::Execute(CGraphicsManager &RM) {
  if (m_PoolRenderableObjectTechnique != NULL)
    m_PoolRenderableObjectTechnique->Apply();
}

void CRenderableObjectTechniquesSceneRendererCommand::Reload() {
  m_PoolRenderableObjectTechnique = RENDTECHM->GetPoolRenderableObjectTechniques().GetResource(m_PoolName);
}