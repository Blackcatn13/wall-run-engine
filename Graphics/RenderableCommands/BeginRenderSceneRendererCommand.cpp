#include "RenderableCommands\BeginRenderSceneRendererCommand.h"
#include "GraphicsManager.h"
#include "Core_Utils/MemLeaks.h"

CBeginRenderSceneRendererCommand::CBeginRenderSceneRendererCommand(CXMLTreeNode &atts)
    : CSceneRendererCommand(atts)
{
}

void CBeginRenderSceneRendererCommand::Execute(CGraphicsManager &RM)
{
    RM.BeginRenderCommand();
}
