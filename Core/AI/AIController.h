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
  CAIController(  CRenderableObject *rond, float speed, float turnSpeed, float gravity);


  virtual ~CAIController();
  void Move(float dt);
  void MoveTo(float dt, Vect3f point);
  void RotateYaw(float dt, Vect3f point);
  void RotateRenderable(float dt, float angle);
  void RotateOrMove(float dt, Vect3f point);
  void OnlyRotate(float dt, Vect3f point);

  GET_SET(CPhysicController *, PhysicController);
  GET_SET(CPhysicUserData *, PhysicUserData);
  GET_SET(float, Gravity);
  GET_SET(float, Speed);
  GET_SET(float, JumpForce);
  GET_SET(bool, isJumping);
  GET_SET(float, CurrentJumpForce);
  GET_SET(std::string, Mesh);
  GET_SET(std::string, Name);

  //GET_SET(std::string, Mesh)
  //GET_SET(std::string, Name);
  GET_SET(Vect3f, Position);
  GET_SET(bool, SpeedModified);

 private:
  float getAngleDiff(Vect3f A, Vect3f B);
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
};


#endif