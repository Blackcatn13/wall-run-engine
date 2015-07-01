#ifndef TRIGGER_H
#define TRIGGER_H

#include "Reports\PhysicTriggerReport.h"
#include "Actor\PhysicActor.h"
#include "XML\XMLTreeNode.h"
#include <string>
#include <map>
#include "Math\Vector3.h"
#include "Utils\Defines.h"


class CPhysicUserData;

class CTrigger: public CPhysicActor {

 private:
  std::string		m_OnEnter;
  std::string		m_OnStay;
  std::string		m_OnExit;
  std::string		m_UpdateFunction;
  float			m_ElapsedTime;
  bool			m_UseElapsedTime ;
  float				m_Time;
  float				m_MaxTime;
  bool			m_Update;
  std::string		m_ScriptParam;
  std::string		m_PlatformName;
  bool			m_IsSwitched;
  CPhysicUserData *m_UserData;
  std::string	m_Name;

 public:
  CTrigger(CXMLTreeNode &node, CPhysicUserData *_pUserData);
  CTrigger( std::string type, Vect3f position, Vect3f size, float radius, Vect3f color, std::string triggerEvent, std::string scriptFunction, std::string param1, std::string param2, CPhysicUserData *_pUserData);


  ~CTrigger(void);

  void ExecuteOnEnter	();
  void ExecuteOnStay	();
  void ExecuteOnExit	();
  void Update(float dt);


  std::string SetString(std::string function, std::string param1);
  std::string SetString(std::string function, std::string param1, std::string param2);

  float  GetElapsedTime() {
    return m_ElapsedTime;
  }

// GET_SET(bool, Update)
  bool getUpdate() {
    return m_Update;
  }
  void setUpdate(bool update);
  GET_SET(bool, IsSwitched)
  GET_SET(float, Time)
  GET_SET(std::string, Name)
};

#endif