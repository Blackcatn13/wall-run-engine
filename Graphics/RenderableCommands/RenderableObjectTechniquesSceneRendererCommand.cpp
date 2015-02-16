#include "RenderableCommands\RenderableObjectTechniquesSceneRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "GraphicsManager.h"
#include "Renderable\PoolRenderableObjectTechnique.h"
#include "Renderable\RenderableObjectTechniqueManager.h"
#include "Core\Core.h"


CRenderableObjectTechniquesSceneRendererCommand::CRenderableObjectTechniquesSceneRendererCommand(CXMLTreeNode &atts)
{
    std::string l_PoolName = atts.GetPszProperty("pool", "");
    m_PoolRenderableObjectTechnique = RENDTECHM->GetPoolRenderableObjectTechniques().GetResource(l_PoolName);
}
void CRenderableObjectTechniquesSceneRendererCommand::Execute(CGraphicsManager &RM)
{
    if (m_PoolRenderableObjectTechnique != NULL)
        m_PoolRenderableObjectTechnique->Apply();
}