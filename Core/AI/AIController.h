#ifndef CAI_CONTROLLER_H
#define CAI_CONTROLLER_H

#include "Object\Object3D.h"
#include "Utils\Defines.h"

class CPhysicController;
class CPhysicUserData;

class CAIController : public CObject3D
{
public:
    CAIController();
    ~CAIController();
    void Move(float dt);
	void MoveTo(float dt, Vect3f point);
    GET_SET(CPhysicController*, PhysicController);
    GET_SET(CPhysicUserData*, PhysicUserData);
    GET_SET(float, Gravity);
    GET_SET(float, Speed);
    GET_SET(float, JumpForce);
    GET_SET(bool, isJumping);
    GET_SET(float, CurrentJumpForce);

private:
	float getAngleDiff(Vect3f A, Vect3f B);
    CPhysicController*	m_PhysicController;
    CPhysicUserData*	m_PhysicUserData;
    float				m_Gravity;
    float				m_Speed;
	float				m_TurnSpeed;
    float				m_JumpForce;
    float				m_CurrentJumpForce;
    bool				m_isJumping;
};


#endif