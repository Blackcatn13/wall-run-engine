#ifndef _RUNNER_ENEMY_H
#define _RUNNER_ENEMY_H

#pragma once

#include "Enemy.h"
#include <string.h>
#include "Utils\Defines.h"
class CRenderableObject;

class CRunnerEnemy : public CEnemy {
 private:

  //float					m_AttackSpeed;
  bool					m_Appeared;

 public:

  CRunnerEnemy(CRenderableObject *renderableObject, float speed, std::string fsmName, Vect2f characterSize, float AttackDistance, float zone);

  //~CEasyEnemy();

  virtual void Update(float elapsedTime);
  GET_SET(bool, Appeared);
  //float GetAttackSpeed() {
  //  return m_AttackSpeed;
  //}


};

#endif
