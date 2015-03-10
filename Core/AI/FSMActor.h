#ifndef FSM_ACTOR_H
#define FSM_ACTOR_H

#include <string>
#include "Utils\MapManager.h"

class CFSMManager;

class CFSMActor
{
public:
    CFSMActor();
    ~CFSMActor();
    void Update(float dt);
	void setFSMManager(CFSMManager* fsm);
	void setStateMachine(std::string fsm) { m_StateMachine = fsm; }
	void setInitialState(std::string state) { m_currentState = state; }
private:
	CFSMManager* m_Manager;
    std::string m_StateMachine;
    std::string m_previousState;
    std::string m_currentState;
};

#endif