#ifndef TRIGGER_MANAGER_H
#define TRIGGER_MANAGER_H

#include "Reports\PhysicTriggerReport.h"
#include <string>
#include <map>

class Vect3f;

enum TRIGGER_SHAPE{
BOX, SPHERE
};

enum TRIGGER_EVENT_TYPE{
ENTER,STAY,LEAVE
};

class CTrigger
{
public:
	std::string m_name;
	Vect3f m_position;
	TRIGGER_SHAPE m_shape;
	Vect3f m_size;
	TRIGGER_EVENT_TYPE m_event;
	std::string m_script;
	int m_group;
	bool m_paint;
	Vect3f m_color;
};

class CTriggerManager : public CPhysicTriggerReport
{
public:
  virtual ~CTriggerManager(void);
  CTriggerManager(void);
  bool	LoadTriggers( std::string FileName);
  void	OnEnter		( CPhysicUserData* _Entity_Trigger1, CPhysicUserData* _Other_Shape);
  void	OnLeave		( CPhysicUserData* _Entity_Trigger1, CPhysicUserData* _Other_Shape);
  void	OnStay		( CPhysicUserData* _Entity_Trigger1, CPhysicUserData* _Other_Shape);
private:
  std::string m_FileName;
  std::map<std::string,CTrigger*> m_TriggerMap;
	
};


#endif