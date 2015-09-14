#ifndef CPLAYER_CONTROLLER_H
#define CPLAYER_CONTROLLER_H

#include "Object\Object3D.h"
#include "Utils\Defines.h"

class CPhysicController;
class CPhysicUserData;
class CRenderableObject;

class CPlayerController : public CObject3D {
 public:
  CPlayerController();
  ~CPlayerController();
  void Move(Vect3f direction, float dt);
  void IsGrounded(Vect3f direction, float dt, bool cae = false);
  GET_SET(CPhysicController *, PhysicController);
  GET_SET(CPhysicUserData *, PhysicUserData);
  GET_SET(bool, isGrounded);
  GET_SET(bool, isTurned);
  GET_SET(float, Gravity);
  GET_SET(float, GravityJump);
  GET_SET(float, Speed);
  GET_SET(float, JumpForce);
  GET_SET(bool, isJumping);
  GET_SET(bool, isDoubleJumping);
  GET_SET(bool, executeDoubleJump);
  GET_SET(float, isOnPlatform);
  GET_SET(bool, isJumpingMoving);
  GET_SET(float, CurrentJumpForce);
  GET_SET(bool, is3D);
  GET_SET(bool, isAttack);
  GET_SET(bool, isFalling);
  GET_SET(float, AttackForce);
  GET_SET(float, CurrentAttackForce);
  GET_SET(float, JumpingTime);
  GET_SET(int, JumpType);
  GET_SET(Vect3f, Direction3D);
  GET_SET(CRenderableObject *, mesh);
  GET_SET(int, Room);

 private:
  CPhysicController	*m_PhysicController;
  CPhysicUserData	*m_PhysicUserData;
  bool				m_isGrounded;
  bool				m_isTurned;
  float				m_Gravity;
  float				m_GravityJump;
  float				m_Speed;
  float				m_JumpForce;
  float				m_CurrentJumpForce;
  float				m_isOnPlatform;
  bool				m_isJumping;
  bool				m_isJumpingMoving;
  bool				m_isDoubleJumping;
  bool				m_executeDoubleJump;
  bool				m_is3D;
  bool				m_isAttack;
  float				m_AttackForce;
  float				m_CurrentAttackForce;
  int					m_JumpType;
  Vect3f			m_Direction3D;
  float       m_JumpingTime;
  bool        m_isFalling;
  int				m_Room;
  CRenderableObject *m_mesh;
};


#endif