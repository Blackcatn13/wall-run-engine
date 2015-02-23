#ifndef CPLAYER_CONTROLLER_H
#define CPLAYER_CONTROLLER_H

#include "Object\Object3D.h"

class CPhysicController;
class CPhysicUserData;

class CPlayerController : public CObject3D
{
public:
    CPlayerController();
    ~CPlayerController();
	void Move(float dt);
private:
	CPhysicController*	m_PhysicController;
	CPhysicUserData*	m_PhysicUserData;
	float				m_Speed;
};


#endif