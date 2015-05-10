#include "RenderableCommands\DisableZTestSceneRendererCommand.h"
#include "Core_Utils/MemLeaks.h"

CDisableZTestSceneRendererCommand::CDisableZTestSceneRendererCommand(CXMLTreeNode &atts) {}
void CDisableZTestSceneRendererCommand::Execute(CGraphicsManager &RM) {
  RM.GetDevice()->SetRenderState( D3DRS_ZENABLE, FALSE );
}