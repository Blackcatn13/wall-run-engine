#include "AI\FSMActor.h"
#include "AI\FSMManager.h"

CFSMActor::CFSMActor()
	: m_currentState("")
	, m_previousState("")
	, m_StateMachine("")
	, m_Manager(NULL)
{
}

CFSMActor::~CFSMActor()
{
}

void CFSMActor::Update(float dt)
{
	if(m_Manager != NULL) {
		std::string state = m_Manager->Update(dt, m_StateMachine, m_currentState);
		if (state != m_currentState) {
			m_previousState = m_currentState;
			m_currentState = state;
		}
	}
}

void CFSMActor::setFSMManager(CFSMManager* fsm)
{
	m_Manager = fsm;
}