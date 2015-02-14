#include "RenderableCommands\RenderDebugLightsSceneRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "GraphicsManager.h"
#include "Lights\LightManager.h"

CRenderDebugLightsSceneRenderCommand::CRenderDebugLightsSceneRenderCommand(CXMLTreeNode &atts)
{
    if (atts.Exists())
        m_Active = atts.GetBoolProperty("active", false);
    else
        m_Active = false;
}
void CRenderDebugLightsSceneRenderCommand::Execute(CGraphicsManager &RM)
{
    if (m_Active)
        LIGHTM->Render(&RM);
}
