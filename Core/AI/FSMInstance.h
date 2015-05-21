#pragma once
#include <string>
#include "FSMManager.h"
#include "Utils\Defines.h"

class CFSMInstance {
 private:
  CMapManager<STATE>	m_States;
  std::string			m_PreviousState;
  std::string			m_CurrentState;
  std::string     m_newState;
  bool            m_Changed;
  bool            m_onEnter;
 public:
  CFSMInstance(FSM *fsm);
  ~CFSMInstance(void);

  GET_SET(CMapManager<STATE>,	States)
  GET_SET( std::string, PreviousState)
  GET_SET( std::string, CurrentState)
  GET_SET( bool, Changed)
  GET_SET( bool, onEnter)
  void setNewState(std::string);
  std::string getNewState();
};

