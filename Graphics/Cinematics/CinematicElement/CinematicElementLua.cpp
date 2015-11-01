#include "CinematicElementLua.h"
#include "XML\XMLTreeNode.h"
#include "Core\Core.h"
#include "Cinematics\Cinematic.h"
#include "Core\ScriptManager.h"
#include "Core_Utils/MemLeaks.h"
#include "Core\Core.h"
#include "Cinematics\CinematicController.h"

CCinematicElementLua::CCinematicElementLua(const CXMLTreeNode &node)
  : CCinematicElement(node),
    m_code(node.GetPszISOProperty("code", "")),
    m_ExecuteOnUpdate(node.GetBoolProperty("at_update", false)) {
}

bool CCinematicElementLua::Execute() {
  SCRIPTM->RunCode(m_code);
  return true;
}

bool CCinematicElementLua::Update(float dt) {
  if (m_ExecuteOnUpdate)
    SCRIPTM->RunCode(m_code);
  return CCORE->GetCinematicController()->getUpdateLuaFinish();
}

CCinematicElementLua::~CCinematicElementLua() {
}