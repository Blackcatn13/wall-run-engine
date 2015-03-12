#ifndef CPLAYER_CONTROLLER_H
#define CPLAYER_CONTROLLER_H

#include "Object\Object3D.h"
#include "Utils\Defines.h"

class CPhysicController;
class CPhysicUserData;

class CPlayerController : public CObject3D
{
public:
    CPlayerController();
    ~CPlayerController();
    void Move(float dt);
    GET_SET(CPhysicController*, PhysicController);
    GET_SET(CPhysicUserData*, PhysicUserData);
    GET_SET(float, isGrounded);
	GET_SET(float, Gravity);
	GET_SET(float, GravityJump);
    GET_SET(float, Speed);
    GET_SET(float, JumpForce);
    GET_SET(bool, isJumping);
    GET_SET(float, CurrentJumpForce);
    GET_SET(bool, is3D);
	//bool IsGrounded();

private:
    CPhysicController*	m_PhysicController;
    CPhysicUserData*	m_PhysicUserData;
    float				m_Gravity;
	float				m_GravityJump;
    float				m_Speed;
    float				m_JumpForce;
    float				m_CurrentJumpForce;
    bool				m_isJumping;
    bool				m_is3D;
	bool				m_isGrounded;
};


#endif