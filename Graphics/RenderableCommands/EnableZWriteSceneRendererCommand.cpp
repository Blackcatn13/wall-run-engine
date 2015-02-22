#include "RenderableCommands\EnableZWriteSceneRendererCommand.h"
#include "GraphicsManager.h"
#include "Core_Utils/MemLeaks.h"

CEnableZWriteSceneRendererCommand::CEnableZWriteSceneRendererCommand(CXMLTreeNode &atts) {}

void CEnableZWriteSceneRendererCommand::Execute(CGraphicsManager &RM)
{
    RM.GetDevice()->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
}