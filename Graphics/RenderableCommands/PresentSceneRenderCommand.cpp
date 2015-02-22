#include "RenderableCommands\PresentSceneRenderCommand.h"
#include "GraphicsManager.h"
#include "Core_Utils/MemLeaks.h"

CPresentSceneRendererCommand::CPresentSceneRendererCommand(CXMLTreeNode &atts)
    : CSceneRendererCommand(atts)
{
}

void CPresentSceneRendererCommand::Execute(CGraphicsManager &RM)
{
    RM.PresentSceneCommand();
}
