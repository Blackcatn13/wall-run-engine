#include "RenderableCommands\PresentSceneRenderCommand.h"
#include "GraphicsManager.h"

CPresentSceneRendererCommand::CPresentSceneRendererCommand(CXMLTreeNode &atts)
    : CSceneRendererCommand(atts)
{
}

void CPresentSceneRendererCommand::Execute(CGraphicsManager &RM)
{
    RM.PresentSceneCommand();
}
