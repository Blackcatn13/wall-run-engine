#include "RenderableCommands\DisableAlphaBlendCommand.h"
#include "Core_Utils/MemLeaks.h"

CDisableAlphaBlendSceneRendererCommand::CDisableAlphaBlendSceneRendererCommand(CXMLTreeNode &atts) {}
void CDisableAlphaBlendSceneRendererCommand::Execute(CGraphicsManager &RM) {
  RM.DisableAlphaBlend();
}