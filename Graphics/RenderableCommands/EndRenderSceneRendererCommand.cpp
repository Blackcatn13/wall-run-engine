#include "RenderableCommands\EndRenderSceneRendererCommand.h"
#include "GraphicsManager.h"

CEndRenderSceneRendererCommand::CEndRenderSceneRendererCommand (CXMLTreeNode &atts)
: CSceneRendererCommand(atts)
{
}

void CEndRenderSceneRendererCommand::Execute(CGraphicsManager &RM)
{
	RM.EndRenderCommand();
}
