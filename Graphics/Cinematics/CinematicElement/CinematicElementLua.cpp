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
    m_withTime(node.GetBoolProperty("withTime", false)),
    m_ExecuteOnUpdate(node.GetBoolProperty("at_update", false)),
    m_time(node.GetFloatProperty("atTime", 0.0f)),
    m_onlyUpdate(node.GetBoolProperty("onlyUpdate", false)),
    m_auxTime(0.0f) {
}

bool CCinematicElementLua::Execute() {
  if (!m_onlyUpdate)
    SCRIPTM->RunCode(m_code);
  return true;
}

bool CCinematicElementLua::Update(float dt) {
  if (m_withTime) {
    m_auxTime += dt;
    if (m_auxTime >= m_time) {
      SCRIPTM->RunCode(m_code);
      m_auxTime = 0;
    }
  }
  if (m_ExecuteOnUpdate)
    SCRIPTM->RunCode(m_code);
  return CCORE->GetCinematicController()->getUpdateLuaFinish();
}

CCinematicElementLua::~CCinematicElementLua() {
}