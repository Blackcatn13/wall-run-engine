#include "FSMInstance.h"


CFSMInstance::CFSMInstance(FSM *fsm):
m_States(fsm->m_States),
	m_CurrentState(fsm->m_currentState),
	m_PreviousState(fsm->m_previousState)
{
}


CFSMInstance::~CFSMInstance(void)
{
}
