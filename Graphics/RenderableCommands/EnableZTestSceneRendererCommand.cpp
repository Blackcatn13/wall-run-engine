#include "RenderableCommands\EnableZTestSceneRendererCommand.h"
#include "GraphicsManager.h"
#include "Core_Utils/MemLeaks.h"
//#include "XML\XMLTreeNode.h"

CEnableZTestSceneRendererCommand::CEnableZTestSceneRendererCommand(CXMLTreeNode &atts) {}
void CEnableZTestSceneRendererCommand::Execute(CGraphicsManager &RM) {
  RM.GetDevice()->SetRenderState( D3DRS_ZENABLE, TRUE );
}