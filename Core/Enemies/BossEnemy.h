#ifndef _BOSS_ENEMY_H
#define _BOSS_ENEMY_H

#pragma once

#include "Enemy.h"

class CXMLTreeNode;


class CBossEnemy : public CEnemy {
 private:
  int m_Phases;
  bool m_BossRunning;



 public:
  CBossEnemy(CXMLTreeNode &info1);
  ~CBossEnemy();
  virtual void Update(float elapsedTime);
  virtual void Render();
  GET_SET(int, Phases)
  GET_SET(bool, BossRunning)
};

#endif
