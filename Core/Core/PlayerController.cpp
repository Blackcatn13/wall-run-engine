#include "PlayerController.h"
#include "Actor\PhysicController.h"
#include "Utils\PhysicUserData.h"
#include "Core\Core.h"
#include "Utils\Defines.h"
#include "PhysicsManager.h"
#include "ActionToInput.h"
#include "Camera\CameraController.h"
#include "ScriptManager.h"
#include "PhysicsDefs.h"
#include "PhysicsManager.h"
#include "Actor\PhysicActor.h"

CPlayerController::CPlayerController()
    : CObject3D(),
      m_Gravity(2),
	  m_GravityJump(2),
      m_Speed (15),
      m_JumpForce(0.65),
      m_CurrentJumpForce(0),
      m_isJumping(false),
      m_is3D(false)
{
    m_PhysicUserData = new CPhysicUserData("Player");
    m_PhysicUserData->SetPaint(true);
    m_PhysicController = new CPhysicController(1, 2, 0.87, 0.1, 0.3, ECG_ESCENE, m_PhysicUserData, Vect3f(0, 2, 0), -m_Gravity);
    PHYSXM->AddPhysicController(m_PhysicController);
}

CPlayerController::~CPlayerController()
{
    CHECKED_DELETE(m_PhysicUserData);
    CHECKED_DELETE(m_PhysicController);
}

void CPlayerController::Move(float dt)
{
	//En lua.
}

bool CPlayerController::IsGrounded()
{
	SCollisionInfo info = SCollisionInfo();
	CPhysicUserData* hit = CCORE->GetPhysicsManager()->RaycastClosestActor(Vect3f(m_Position.x, m_Position.y-2, m_Position.z), 
		Vect3f(0,-1,0), 0xffffffff, info);
	if (hit != NULL && info.m_fDistance <= 0.1)
		return true;
	else return false;
}