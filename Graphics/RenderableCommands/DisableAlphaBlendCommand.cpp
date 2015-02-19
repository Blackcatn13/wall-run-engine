#include "RenderableCommands\DisableAlphaBlendCommand.h"

CDisableAlphaBlendSceneRendererCommand::CDisableAlphaBlendSceneRendererCommand(CXMLTreeNode &atts) {}
void CDisableAlphaBlendSceneRendererCommand::Execute(CGraphicsManager &RM)
{
    RM.DisableAlphaBlend();
}