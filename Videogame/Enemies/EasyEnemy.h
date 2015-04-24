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



 public:
  CEasyEnemy(CXMLTreeNode &info1);
  CEasyEnemy(CRenderableObject *renderableObject, std::vector<Vect3f> wayPoints, float speed, float speedAttack, float life, std::string fsmName);
  CEasyEnemy(std::string mesh, std::string name, Vect3f position,  float speed, float turnSpeed, float gravity, float yaw) ;
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

};

#endif
