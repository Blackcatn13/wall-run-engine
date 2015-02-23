#include "PlayerController.h"
#include "Actor\PhysicController.h"
#include "Utils\PhysicUserData.h"
#include "Core\Core.h"
#include "PhysicsManager.h"
#include "ActionToInput.h"
#include "Camera\CameraController.h"

CPlayerController::CPlayerController()
	: CObject3D(),
	m_Speed (3)
{
	m_PhysicUserData = new CPhysicUserData("Player");
	m_PhysicUserData->SetPaint(true);
	m_PhysicController = new CPhysicController(1, 2, 0.87, 0.1, 0.3, ECG_ESCENE, m_PhysicUserData, Vect3f(0, 5, 0));
	PHYSXM->AddPhysicController(m_PhysicController);
}

CPlayerController::~CPlayerController()
{
}

void CPlayerController::Move(float dt)
{
	
	float deltaX, deltaY, deltaZ;
	if (ACT2IN->DoAction("yaw", deltaX))
        SetYaw(GetYaw() - deltaX * dt);
    if (ACT2IN->DoAction("pitch", deltaY))
        SetPitch(GetPitch() - deltaY * dt);
    float yaw = GetYaw();
	Vect3f dir = CAMCONTM->getActiveCamera()->GetDirection();
    Vect3f nor = Vect3f(mathUtils::Cos(yaw + ePI2f), 0, (mathUtils::Sin(yaw + ePI2f)));
    nor.Normalize();
	Vect3f mov = (0,0,0);
    if (ACT2IN->DoAction("MoveForward"))
		mov = mov + Vect3f(0,0,1) * m_Speed * dt;
    if (ACT2IN->DoAction("MoveBack"))
		mov = mov - Vect3f(0,0,1) * m_Speed * dt;
    if (ACT2IN->DoAction("MoveRigth"))
		mov = mov - Vect3f(-1,0,0) * m_Speed * dt;
    if (ACT2IN->DoAction("MoveLeft"))
		mov = mov + Vect3f(-1,0,0) * m_Speed * dt;
	m_PhysicController->Move(mov, dt);
	SetPosition(m_PhysicController->GetPosition());
}