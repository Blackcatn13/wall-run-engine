#include "RenderableCommands\EnableAlphaBlendCommand.h"
#include "GraphicsManager.h"
#include "XML\XMLTreeNode.h"

CEnableAlphaBlendSceneRendererCommand::CEnableAlphaBlendSceneRendererCommand(CXMLTreeNode &atts) {}
void CEnableAlphaBlendSceneRendererCommand::Execute(CGraphicsManager &RM)

{
    RM.EnableAlphaBlend();
}