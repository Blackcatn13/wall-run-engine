#include "AI\Puzzle.h"
#include "TriggerManager\Trigger.h"
#include "Core\ScriptManager.h"
#include "Core\Core.h"
CPuzzle::CPuzzle() :
  m_ActivatedSwitches(0),
  m_Done(false) {
}

CPuzzle::CPuzzle(std::string LuaCode, int MinActivatedSwitches):
  m_LuaCode(LuaCode),
  m_MinActivatedSwitches(MinActivatedSwitches),
  m_ActivatedSwitches(0) {
}
CPuzzle::~CPuzzle() {

  m_Switches.clear();
}


void CPuzzle::Update(float dt) {

  if (m_ActivatedSwitches == m_MinActivatedSwitches && !m_Done) {
    std::stringstream ss;
    ss << m_LuaCode << "(\"" << m_SceneElement << "\"";
    if (m_SceneElement2 != "")
      ss << ", \"" << m_SceneElement2 << "\"";
    ss << ")";
    std::string l_Code = ss.str();
    SCRIPTM->RunCode(l_Code);
    m_Done = true;
  }
}

