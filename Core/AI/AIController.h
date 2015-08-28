#ifndef CAI_CONTROLLER_H
#define CAI_CONTROLLER_H

#include "Object\Object3D.h"
#include "Utils\Defines.h"
#include <string>

class CPhysicController;
class CPhysicUserData;
class CRenderableObject;

class CAIController : public CObject3D {
 public:
  CAIController();
  CAIController(std::string mesh, std::string name, Vect3f position,  float speed, float turnSpeed, float gravity);
  CAIController(  CRenderableObject *rond, float speed, float turnSpeed, float gravity, Vect2f controller_size);


  virtual ~CAIController();
  void Move(float dt);
  void MoveTo(float dt, Vect3f point);
  void RotateYaw(float dt, Vect3f point);
  void RotateRenderable(float dt, float angle);
  void RotateOrMove(float dt, Vect3f point);
  void DestruirDisparo();
  bool CheckPlayerShotCollision();
  void MoveToPosition(Vect3f pos);
  void SetShootingStats(float speedProjectile, float projectileLife, float cooldownTimer);
  Vect3f UpdtateProjectilePosition(float dt);


  GET_SET(CPhysicController *, PhysicController);
  GET_SET(CPhysicUserData *, PhysicUserData);
  GET_SET(float, Gravity);
  GET_SET(float, Speed);
  GET_SET(float, TurnSpeed);

  GET_SET(float, JumpForce);
  GET_SET(bool, isJumping);
  GET_SET(float, CurrentJumpForce);
  GET_SET(std::string, Mesh);
  GET_SET(std::string, Name);
  GET_SET(CRenderableObject *, RenderableObject)

  //GET_SET(std::string, Mesh)
  //GET_SET(std::string, Name);
  GET_SET(Vect3f, Position);
  GET_SET(bool, SpeedModified);
  GET_SET(bool, IsOnCooldown);
  GET_SET(float, CooldownTimer);
  GET_SET(float, BalaSpeed);

  GET_SET(Vect3f, PosicionBala);
  GET_SET(Vect3f, DireccionBala);
  GET_SET(float, CurrentCooldown);
  GET_SET(float, ProjectileHitbox);
  GET_SET(float, tiempoVidaDisparo);

  float getAngleDiff(Vect3f A, Vect3f B);

 protected:

  CPhysicController	*m_PhysicController;
  CPhysicUserData	*m_PhysicUserData;
  float				m_Gravity;
  float				m_Speed;
  float				m_TurnSpeed;
  float				m_JumpForce;
  float				m_CurrentJumpForce;
  bool				m_isJumping;
  std::string			m_Mesh;
  std::string			m_Name;
  CRenderableObject *m_RenderableObject;
  bool				m_SpeedModified;
  bool				m_IsOnCooldown;
  float				m_CooldownTimer;
  Vect3f			m_PosicionBala;
  Vect3f			m_DireccionBala;
  float             m_BalaSpeed;
  float				m_CurrentCooldown;
  float				m_tiempoVidaDisparo;
  float				m_minAngleDisparo;
  float				m_ProjectileHitbox;
  float				m_EnemyHitbox;
  float				m_Angle;
};

#endif