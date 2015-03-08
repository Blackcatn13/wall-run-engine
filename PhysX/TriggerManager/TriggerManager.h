#ifndef TRIGGER_MANAGER_H
#define TRIGGER_MANAGER_H

#include "Reports\PhysicTriggerReport.h"
#include "Utils\TemplatedVectorMapManager.h"
#include "Trigger.h"
#include <string>
#include <map>
#include "Math\Vector3.h"

class CTriggerManager : public CTemplatedVectorMapManager<CTrigger>, public CPhysicTriggerReport
{
public:
    virtual ~CTriggerManager(void);
    CTriggerManager(void);
    void	Clear();
    bool	LoadTriggers( std::string FileName);
    virtual void	OnEnter		( CPhysicUserData* _Entity_Trigger1, CPhysicUserData* _Other_Shape);
    virtual void	OnLeave		( CPhysicUserData* _Entity_Trigger1, CPhysicUserData* _Other_Shape);
    virtual void	OnStay		( CPhysicUserData* _Entity_Trigger1, CPhysicUserData* _Other_Shape);
    virtual void	Update		(float dt);
    bool	Reload(const std::string &FileName);
    bool	Reload();
    CPhysicTriggerReport* getTriggerReport()
    {
        return this;
    }
private:
    std::string							m_FileName;
    std::vector	<CPhysicUserData *>		m_UserData;
    float								ElapsedTime;
};


#endif