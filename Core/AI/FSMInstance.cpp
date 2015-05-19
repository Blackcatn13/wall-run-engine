#include "FSMInstance.h"


CFSMInstance::CFSMInstance(FSM *fsm):
  m_States(fsm->m_States),
  m_CurrentState(fsm->m_currentState),
  m_PreviousState(fsm->m_previousState),
  m_newState(""),
  m_Changed(false) {
}


CFSMInstance::~CFSMInstance(void) {
}

void CFSMInstance::setNewState(std::string state) {
  m_newState = state;
  m_Changed = true;
}

std::string CFSMInstance::getNewState() {
  m_Changed = false;
  return m_newState;
}
