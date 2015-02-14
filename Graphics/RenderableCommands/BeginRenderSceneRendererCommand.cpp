#include "RenderableCommands\BeginRenderSceneRendererCommand.h"
#include "GraphicsManager.h"

CBeginRenderSceneRendererCommand::CBeginRenderSceneRendererCommand(CXMLTreeNode &atts)
    : CSceneRendererCommand(atts)
{
}

void CBeginRenderSceneRendererCommand::Execute(CGraphicsManager &RM)
{
    RM.BeginRenderCommand();
}
