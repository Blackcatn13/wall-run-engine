#ifndef TRIGGER_MANAGER_H
#define TRIGGER_MANAGER_H

#include "Reports\PhysicTriggerReport.h"

class CTriggerManager : public CPhysicTriggerReport
{
public:
  virtual ~CTriggerManager(void);
  CTriggerManager(void);
  void	OnEnter		( CPhysicUserData* _Entity_Trigger1, CPhysicUserData* _Other_Shape);
  void	OnLeave		( CPhysicUserData* _Entity_Trigger1, CPhysicUserData* _Other_Shape);
  void	OnStay		( CPhysicUserData* _Entity_Trigger1, CPhysicUserData* _Other_Shape);
private:
	
};


#endif