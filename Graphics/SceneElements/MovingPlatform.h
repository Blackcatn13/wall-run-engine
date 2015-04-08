#pragma once
#ifndef MOVING_PLATFORM_H
#define MOVING_PLATFORM_H

#include <string>
#include "Math\Vector3.h"
#include "StaticPlatform.h"
#include <vector>
#include "AI\FSMManager.h"



class CPhysicController;
class CMovingPlatform : public CStaticPlatform {
 protected:
  float				m_Speed;
  Vect3f				m_Direction;
  std::vector<Vect3f> m_WayPointsVector;
  int					m_CurrentWpId;
  FSM			*m_Fsm;
  Vect3f		m_NextWP;
  //CPhysicController*	m_PhysicController;

 public:
  CMovingPlatform(std::string platformName, std::string coreName, float speed);

  ~ CMovingPlatform ();

  void MoveToPoint(float dt, Vect3f point, float minDistance);
  Vect3f GetNextWP();
  std::vector<Vect3f> &GetWayPointsVector() {
    return m_WayPointsVector;
  }

  void Update(float ElapsedTime);
  void AddBoxController(Vect3f size, float slope, float skinwidth, float offset, float gravity);
  void UpdateFSM(float elapsedTime);
  void SetNextWPVector(Vect3f NextWp) {
    m_NextWP = NextWp;
  }

  Vect3f GetNextWPVector() {
    return m_NextWP;
  }
};

#endif