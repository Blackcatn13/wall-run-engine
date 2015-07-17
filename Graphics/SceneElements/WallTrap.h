#pragma once
#ifndef WALL_TRAP_H
#define WALL_TRAP_H

#include <string>
#include "Math\Vector3.h"
#include "StaticPlatform.h"
#include <vector>
#include "AI\FSMManager.h"
#include "AI\FSMInstance.h"

class CXMLTreeNode;
class CPhysicController;

class CWallTrap : public CStaticPlatform {
 protected:
  float				m_SpeedClosing;
  float				m_SpeedOpening;
  Vect3f				m_Direction;
  bool				m_Activated;
  std::string		m_Side;
  float				m_TimeOut;
  float				m_CurrentTime;
  Vect3f				m_FinalPosition;
  Vect3f				m_OriginalPosition;
  //CPhysicController*	m_PhysicController;

 public:
  CWallTrap(std::string platformName, std::string coreName, float speedOpening, float speedClosing, std::string side);
  CWallTrap(CXMLTreeNode &node);
  virtual ~CWallTrap();

  void MoveToPoint(float dt, Vect3f point, float minDistance,  float speed);

  void Update(float ElapsedTime);

  //bool isInside(Vect3f vector1, Vect3f vector2);
  //bool isAround(Vect3f vector1, Vect3f vector2);
  GET_SET(bool, Activated);
  GET_SET(float, SpeedClosing);
  GET_SET(float, SpeedOpening);
  GET_SET(std::string, Side);
  GET_SET(float, TimeOut);
  GET_SET(float, CurrentTime);
  GET_SET(Vect3f, FinalPosition);
  GET_SET(Vect3f, OriginalPosition);
};

#endif