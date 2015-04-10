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
#include "Renderable\RenderableObject.h"
#include "Renderable\RenderableObjectsManager.h"
#include "Renderable\RenderableObjectsLayersManager.h"

CPlayerController::CPlayerController()
  : CObject3D(),
    m_isGrounded(false),
    m_isTurned(false),
    m_Gravity(2),
    m_GravityJump(2),
    m_Speed (15),
    m_JumpForce(0.65),
    m_CurrentJumpForce(0),
    m_isJumping(false),
    m_isJumpingMoving(false),
    m_is3D(true),
	m_isAttack(false),
	m_AttackForce(5.0),
	m_CurrentAttackForce(5.0) {
  m_PhysicUserData = new CPhysicUserData("Player");
  m_PhysicUserData->SetPaint(false);
  m_PhysicController = new CPhysicController(0.25, 0.5, 1.047, 0.1, 0.3, ECG_ESCENE, m_PhysicUserData, Vect3f(0, 5, 0), 0);
  PHYSXM->AddPhysicController(m_PhysicController);
  //CRenderableObject* malla = RENDLM->GetDefaultRenderableObjectManager()->GetResource("PIKY");
  CRenderableObject *malla = RENDLM->GetDefaultRenderableObjectManager()->GetResource("SpongePicky");
  malla->SetYaw(m_fYaw);
  malla->SetPosition(m_PhysicController->GetPosition());
}

CPlayerController::~CPlayerController() {
  CHECKED_DELETE(m_PhysicUserData);
  CHECKED_DELETE(m_PhysicController);
}

void CPlayerController::Move(float dt) {
  //En lua.
}

void CPlayerController::IsGrounded(Vect3f direction, float dt) {
  bool result = m_PhysicController->MovePlayer(direction, dt);
  if (result) {
    m_isJumping = false;
    m_isJumpingMoving = false;
    m_isGrounded = true;
  } else m_isGrounded = false;
}