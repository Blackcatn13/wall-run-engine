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
class CFSMInstance;

enum ENEMY_TYPE {
  UNDEFINED,
  MIKMIK,
  PUMPUM
};

class CEnemy : public CAIController { //CAIController
 protected:

  int				m_Life;
  float				m_TimeToSpawn;
  float				m_TimeToShoot;
  float				m_ShootAccuracy;
  //std::string			m_Mesh;
  //std::string			m_Name;
  CFSMInstance		*m_Fsm;
// CRenderableObject *m_RenderableObject;
  //float				m_Speed;
  float				m_CurrentTime;
  std::string		m_FsmName;
  Vect3f			m_OriginalPosition;
  bool				m_isAlive;
  ENEMY_TYPE		m_enemyType;
  float				m_AttackPlayerDistance;
  float				m_Zone;
  bool				m_isAttacking;
  bool				m_time_to_fly;
  Vect3f			m_flyVec;
  bool				m_MovedToDiePosition;
  bool				m_IsDying;
  std::string		m_ProjectileName;


  CEnemy();
 public:
  CEnemy(CXMLTreeNode &info1);
  CEnemy(std::string mesh, std::string name, Vect3f position,  float speed, float turnSpeed, float gravity, float yaw, std::string projectile = "");
  CEnemy(CRenderableObject *renderableObject, float speed, float turnSpeed, int life, Vect2f controller_size, float AttackDistance, float zone, std::string projectile = "");
  virtual ~CEnemy();


  //virtual ~CEnemy();
  virtual void Init(std::string fsmName) /*= 0*/;
  virtual void Update(float elapsedTime) /*= 0*/;
  virtual void Render() /*= 0*/;

  void UpdateFSM(float elapsedTime);
  Vect3f GetOriginalPosition();
  void SetOriginalPosition(Vect3f position);
  std::string GetEnemyName() {
    return getName();
  }

  void AddDamagePlayer();
  void AddDamageEnemyMikMik();
  void AddDamageEnemyPumPum();
  int ActualizarHitboxEnemigo();
  void ActualizarDisparo(float dt);
  void ShotToVector(float dt, Vect3f point);
  void OnlyRotate(float dt, Vect3f point);
  int CheckPlayerCollision();

  //void SetMesh(std::string mesh);
  GET_SET(int, Life);
  /*GET_SET(float, Yaw);
  GET_SET_REF(std::string, Name);
  GET_SET_REF(Vect3f, Position);*/
  GET_SET(float, TimeToSpawn);
  GET_SET(float, TimeToShoot);
  GET_SET(float, ShootAccuracy);
// GET_SET(float, Speed);
  GET_SET(float, CurrentTime);
  CFSMInstance *getFsm() { return m_Fsm;}
  GET_SET(bool, isAlive);
  GET_SET(float, AttackPlayerDistance);
  GET_SET(float, Zone);
  GET_SET(bool, isAttacking);
  GET_SET(bool, time_to_fly);
  GET_SET(bool, MovedToDiePosition);
  GET_SET(Vect3f, flyVec);
  GET_SET(ENEMY_TYPE, enemyType);
  GET_SET(bool, IsDying);
  GET_SET(std::string, ProjectileName);
  // get & set
  // set(const <type> &name)
  // const <type> & get

};

#endif