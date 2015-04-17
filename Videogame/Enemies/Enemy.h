#ifndef _ENEMY_H
#define _ENEMY_H

#pragma once

#include <string>
#include "Math\Vector3.h"
#include "Utils\Defines.h"
#include "AI\AIController.h"
//#include "Object\Object3D.h"
#include "AI\FSMManager.h"
#include "Mesh\MeshInstance.h"

class CXMLTreeNode;
class CPhysicController;
class CPhysicUserData;
class CRenderableObject;

class CEnemy : public CAIController { //CAIController
 protected:

  float				m_Life;
  float				m_TimeToSpawn;
  float				m_TimeToShoot;
  float				m_ShootAccuracy;
  //std::string			m_Mesh;
  //std::string			m_Name;
  FSM				*m_Fsm;
  CRenderableObject *m_RenderableObject;

  CEnemy();
 public:
  CEnemy(CXMLTreeNode &info1);
  CEnemy(std::string mesh, std::string name, Vect3f position,  float speed, float turnSpeed, float gravity, float yaw);
  CEnemy(CRenderableObject *renderableObject);
  //~CEnemy();


  //virtual ~CEnemy();
  virtual void Init() /*= 0*/;
  virtual void Update(float elapsedTime) /*= 0*/;
  virtual void Render() /*= 0*/;
  void UpdateFSM(float elapsedTime);
  //void SetMesh(std::string mesh);
  GET_SET(float, Life);
  /*GET_SET(float, Yaw);
  GET_SET_REF(std::string, Name);
  GET_SET_REF(Vect3f, Position);*/
  GET_SET(float, TimeToSpawn);
  GET_SET(float, TimeToShoot);
  GET_SET(float, ShootAccuracy);
  // get & set
  // set(const <type> &name)
  // const <type> & get

};

#endif