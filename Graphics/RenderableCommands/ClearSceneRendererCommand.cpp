#include "RenderableCommands\ClearSceneRendererCommand.h"
#include "GraphicsManager.h"
#include "Core_Utils/MemLeaks.h"

CClearSceneRendererCommand::CClearSceneRendererCommand(CXMLTreeNode &atts)
  : m_Color(atts.GetBoolProperty("color", false))
  , m_Depth(atts.GetBoolProperty("depth", false))
  , m_Stencil(atts.GetBoolProperty("stencil", false))
  , CSceneRendererCommand(atts) {
}

void CClearSceneRendererCommand::Execute(CGraphicsManager &RM) {
  RM.ClearSceneCommand(m_Color, m_Depth, m_Stencil);
}
