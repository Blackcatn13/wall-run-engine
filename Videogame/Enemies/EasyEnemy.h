#ifndef _EASY_ENEMY_H
#define _EASY_ENEMY_H

#pragma once

#include "Enemy.h"

class CXMLTreeNode;
class CRenderableObject;

class CEasyEnemy : public CEnemy {
 public:
  CEasyEnemy(CXMLTreeNode &info1);
  CEasyEnemy(CRenderableObject *renderableObject);
  CEasyEnemy(std::string mesh, std::string name, Vect3f position,  float speed, float turnSpeed, float gravity, float yaw) ;
  //~CEasyEnemy();

  virtual void Update(float elapsedTime);
  virtual void Render();
  void MoveEnemy(float ElapsedTime, Vect3f wp);
};

#endif
