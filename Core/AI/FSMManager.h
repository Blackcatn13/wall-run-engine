#ifndef FSM_MANAGER_H
#define FSM_MANAGER_H

#include <string>
#include "Utils\MapManager.h"

typedef struct State {
    std::string		onEnter;
    std::string		onExit;
    std::string		onUpdate;
    float			m_UpdateTime;
    float			m_ElapsedTime;
    bool            m_onEnter;
} STATE;

typedef struct Fsm {
    CMapManager<STATE>	m_States;
} FSM;

class CFSMManager : public CMapManager<FSM>
{
public:
    CFSMManager();
    ~CFSMManager();
    void Load(std::string file);
    void Reload();
    std::string Update(float dt, std::string fsm, std::string state);
private:
    void Load();
    std::string m_fileName;
};

#endif