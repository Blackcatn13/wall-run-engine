#include "RenderableCommands\SetPolyPhisics.h"
#include "GraphicsManager.h"
#include "XML\XMLTreeNode.h"
#include "Core\Core.h"
#include "Core\ScriptManager.h"
#include <strstream>


CSetPolyPhisicsCommand::CSetPolyPhisicsCommand(CXMLTreeNode &atts)
  : CSceneRendererCommand(atts) {
  m_Room = atts.GetPszISOProperty("room", "", false);
  std::stringstream ss;
  ss << "init_polis()"; //<< "(" << m_Room << ")";
  std::string toRun = ss.str();
  SCRIPTM->RunCode("init_polis()");
}

void CSetPolyPhisicsCommand::Execute(CGraphicsManager &RM) {

}
