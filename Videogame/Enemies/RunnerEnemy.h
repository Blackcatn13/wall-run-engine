#ifndef _RUNNER_ENEMY_H
#define _RUNNER_ENEMY_H

#pragma once

#include "Enemy.h"
#include <string.h>

class CRenderableObject;

class CRunnerEnemy : public CEnemy {
 private:

  //float					m_AttackSpeed;

 public:

  CRunnerEnemy(CRenderableObject *renderableObject, float speed, std::string fsmName, Vect2f characterSize);

  //~CEasyEnemy();

  virtual void Update(float elapsedTime);

  //float GetAttackSpeed() {
  //  return m_AttackSpeed;
  //}


};

#endif
