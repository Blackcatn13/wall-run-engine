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
    m_JumpForce(0.65f),
    m_CurrentJumpForce(0),
    m_isJumping(false),
    m_isJumpingMoving(false),
    m_isDoubleJumping(false),
    m_executeDoubleJump(false),
    m_isOnPlatform(1.0),
    m_is3D(true),
    m_isAttack(false),
    m_AttackForce(5.0f),
    m_CurrentAttackForce(5.0f),
    m_JumpingTime(0),
    m_Room(0),
    m_isFalling(false),
    m_mesh(NULL) {
  m_PhysicUserData = new CPhysicUserData("Player");
  m_PhysicUserData->SetPaint(false);
  m_PhysicController = new CPhysicController(0.5f, 1, 45.0f, 0.05f, 0.5f, ECG_ESCENE, m_PhysicUserData, Vect3f(55.784378f, 1.0f, 0.617737f), 0);
  m_PhysicUserData->SetMyCollisionGroup(ECG_PLAYER);
  PHYSXM->AddPhysicController(m_PhysicController);
  //CRenderableObject* malla = RENDLM->GetDefaultRenderableObjectManager()->GetResource("PIKY");
  //m_mesh = RENDLM->GetDefaultRenderableObjectManager()->GetResource("SpongePicky");
  //m_mesh->SetYaw(m_fYaw);
  //m_mesh->SetPosition(m_PhysicController->GetPosition());
}

CPlayerController::~CPlayerController() {
  CHECKED_DELETE(m_PhysicUserData);
  CHECKED_DELETE(m_PhysicController);
}

void CPlayerController::Move(Vect3f direction, float dt) {
  m_PhysicController->MovePlayer(direction, dt);

}

void CPlayerController::IsGrounded(Vect3f direction, float dt, bool cae) {
  bool result = m_PhysicController->MovePlayer(direction, dt,  cae);
  if (result) {
    m_isJumping = false;
    m_isJumpingMoving = false;
    m_isGrounded = true;
  } else {
    m_isGrounded = false;
  }
}