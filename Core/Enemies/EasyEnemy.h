#ifndef _EASY_ENEMY_H
#define _EASY_ENEMY_H

#pragma once

#include "Enemy.h"


class CXMLTreeNode;
class CRenderableObject;

class CEasyEnemy : public CEnemy {
 private:
  std::vector<Vect3f>	m_WpVector;
  Vect3f				m_CurrentWp;
  int					m_CurrentWpId;
  float					m_AttackSpeed;
  float					m_OriginalSpeed;
  bool					m_Returning;
  bool					m_Static;
  bool					m_Hide;
  bool					m_Hiding;
  float					m_OffsetHide;



 public:
  CEasyEnemy(CXMLTreeNode &info1);
  CEasyEnemy(CRenderableObject *renderableObject, std::vector<Vect3f> wayPoints, float speed, float turnSpeed, float speedAttack, int life, std::string fsmName, Vect2f characterSize, float AttackDistance, float zone, bool isStatic, std::string projectile);
  CEasyEnemy(std::string mesh, std::string name, Vect3f position,  float speed, float turnSpeed, float gravity, float yaw, std::string projectile) ;
  //~CEasyEnemy();

  virtual void Update(float elapsedTime);
  virtual void Render();
  void MoveEnemy(float ElapsedTime, Vect3f wp);
  Vect3f GetNextWp();
// void InitWpVector(int numWp, int max_distance);

  void SetCurrentWp(Vect3f CurrentWp) {
    m_CurrentWp = CurrentWp;
  }

  Vect3f GetCurrentWp() {
    return m_CurrentWp;
  }

  float GetAttackSpeed() {
    return m_AttackSpeed;
  }

  float GetOriginalSpeed() {
    return m_OriginalSpeed;
  }

  int GetWPVectorSize();

  GET_SET(bool, Returning)
  GET_SET(bool, Static)
  GET_SET(bool, Hiding)
  GET_SET(bool, Hide)
  GET_SET(float, OffsetHide)

  bool Is_Static() {
    return m_Static;
  }

};

#endif
