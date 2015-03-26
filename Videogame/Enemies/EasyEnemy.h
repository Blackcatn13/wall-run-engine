#ifndef _EASY_ENEMY_H
#define _EASY_ENEMY_H

#pragma once

#include "Enemy.h"

class CXMLTreeNode;

class CEasyEnemy : public CEnemy {
 public:
  CEasyEnemy(CXMLTreeNode &info1);
  CEasyEnemy(std::string mesh, std::string name, Vect3f position,  float speed, float turnSpeed, float gravity, float yaw) ;
  //~CEasyEnemy();
  virtual void Init();
  virtual void Update(float elapsedTime);
  virtual void Render();
};

#endif
