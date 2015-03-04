#include "PlayerController.h"
#include "Actor\PhysicController.h"
#include "Utils\PhysicUserData.h"
#include "Core\Core.h"
#include "Utils\Defines.h"
#include "PhysicsManager.h"
#include "ActionToInput.h"
#include "Camera\CameraController.h"
#include "ScriptManager.h"

CPlayerController::CPlayerController()
    : CObject3D(),
      m_Gravity(13),
      m_Speed (15),
      m_JumpForce(1.5),
      m_CurrentJumpForce(0),
      m_isJumping(false)
{
    m_PhysicUserData = new CPhysicUserData("Player");
    m_PhysicUserData->SetPaint(true);
    m_PhysicController = new CPhysicController(1, 2, 0.87, 0.1, 0.3, ECG_ESCENE, m_PhysicUserData, Vect3f(0, 2, 0), -m_Gravity);
    PHYSXM->AddPhysicController(m_PhysicController);
	SCRIPTM->RunCode("on_init_cameras_lua()");
}

CPlayerController::~CPlayerController()
{
    CHECKED_DELETE(m_PhysicUserData);
    CHECKED_DELETE(m_PhysicController);
}

void CPlayerController::Move(float dt)
{
    /*float deltaX, deltaY, deltaZ;
    if (ACT2IN->DoAction("yaw", deltaX))
        SetYaw(GetYaw() - deltaX * dt);
    if (ACT2IN->DoAction("pitch", deltaY))
        SetPitch(GetPitch() - deltaY * dt);
    float yaw = GetYaw();
    Vect3f dir = CAMCONTM->getActiveCamera()->GetDirection();
    Vect3f nor = Vect3f(mathUtils::Cos(yaw + ePI2f), 0, (mathUtils::Sin(yaw + ePI2f)));
    nor.Normalize();
    Vect3f mov = (0, 0, 0);
    if (ACT2IN->DoAction("MoveForward"))
        mov = mov + Vect3f(0, 0, 1) * m_Speed * dt;
    if (ACT2IN->DoAction("MoveBack"))
        mov = mov - Vect3f(0, 0, 1) * m_Speed * dt;
    if (ACT2IN->DoAction("MoveRigth"))
        mov = mov - Vect3f(-1, 0, 0) * m_Speed * dt;
    if (ACT2IN->DoAction("MoveLeft"))
        mov = mov + Vect3f(-1, 0, 0) * m_Speed * dt;
    if (ACT2IN->DoAction("Jump")) {
        m_isJumping = true;
        m_CurrentJumpForce = m_JumpForce;
        mov.y = m_CurrentJumpForce;
    }
    if (m_isJumping && m_CurrentJumpForce > 0) {
        m_CurrentJumpForce = m_CurrentJumpForce - (m_Gravity * dt);
        mov.y = m_CurrentJumpForce;
    }
    m_PhysicController->Move(mov, dt);
    SetPosition(m_PhysicController->GetPosition());*/
}