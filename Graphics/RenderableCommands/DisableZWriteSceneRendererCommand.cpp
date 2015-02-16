#include "RenderableCommands\DisableZWriteSceneRendererCommand.h"

CDisableZWriteSceneRendererCommand::CDisableZWriteSceneRendererCommand(CXMLTreeNode &atts) {}

void CDisableZWriteSceneRendererCommand::Execute(CGraphicsManager &RM)
{
    RM.GetDevice()->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
}