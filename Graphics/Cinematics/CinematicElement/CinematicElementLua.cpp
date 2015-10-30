#include "CinematicElementLua.h"
#include "XML\XMLTreeNode.h"
#include "Core\Core.h"
#include "Cinematics\Cinematic.h"
#include "Core\ScriptManager.h"
#include "Core_Utils/MemLeaks.h"


CCinematicElementLua::CCinematicElementLua(const CXMLTreeNode &node)
  : CCinematicElement(node),
    m_code(node.GetPszISOProperty("code", "")) {
}

bool CCinematicElementLua::Execute() {
  SCRIPTM->RunCode(m_code);
  return true;
}

bool CCinematicElementLua::Update(float dt) {
  return false;
}

CCinematicElementLua::~CCinematicElementLua() {
}