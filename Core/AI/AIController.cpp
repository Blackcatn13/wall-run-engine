#include "AIController.h"
#include "Actor\PhysicController.h"
#include "Utils\PhysicUserData.h"
#include "Core\Core.h"
#include "PhysicsManager.h"
#include "ActionToInput.h"
#include "Camera\CameraController.h"
#include "Math\MathUtils.h"
#include "Math\Color.h"
#include "Renderable\RenderableObject.h"
#include "Renderable\RenderableObjectsManager.h"
#include "Renderable\RenderableObjectsLayersManager.h"
#include "Core\PlayerController.h"
#include <sstream>
#include "Math\Vector3.h"
#include "Core\ScriptManager.h"
#include "Actor\PhysicActor.h"

CAIController::CAIController()
  : CObject3D(),
    m_Gravity(13),
    m_Speed (0.5),
    m_TurnSpeed(2.0),
    m_JumpForce(1.5),
    m_Angle(0.0f),
    m_IsOnCooldown(false),
    m_CooldownTimer(5),
    m_ProjectileHitbox(0.5),
    m_EnemyHitboxX(1.5),//HARDCODED
    m_EnemyHitboxY(2.0),//HARDCODED
    m_CurrentCooldown(0),
    m_tiempoVidaDisparo(2.0),
    m_minAngleDisparo(0.2),
    m_BalaSpeed(5),
    m_CurrentJumpForce(0),
    m_isJumping(false),
    m_RenderableObject(NULL),
    m_SpeedModified(false) {

  m_PhysicUserData = new CPhysicUserData("AI");
  m_PhysicUserData->SetPaint(true);
  m_PhysicUserData->SetColor(colRED);
  m_PhysicController = new CPhysicController(0.5, 0.25, 45, 0.1, 0.3, ECG_ESCENE, m_PhysicUserData, Vect3f(-15, 5, -15), -m_Gravity);
  PHYSXM->AddPhysicController(m_PhysicController);
}

CAIController::CAIController(std::string mesh, std::string name, Vect3f position, float speed, float turnSpeed, float gravity):

  CObject3D (),
  m_Gravity(gravity),
  m_Speed(speed),
  m_TurnSpeed(turnSpeed),
  m_JumpForce(0),
  m_tiempoVidaDisparo(2.0),
  m_minAngleDisparo(0.2),
  m_BalaSpeed(5),
  m_CurrentJumpForce(0),
  m_IsOnCooldown(false),
  m_CooldownTimer(5),
  m_ProjectileHitbox(0.5),
  m_Angle(0.0f),
  m_EnemyHitboxX(1.5),//HARDCODED
  m_EnemyHitboxY(2.0),//HARDCODED
  m_CurrentCooldown(0),
  m_isJumping(false),
  m_Name(name),
  m_Mesh(mesh),
  m_RenderableObject(NULL),
  m_SpeedModified(false) {
  std::stringstream ss;
  ss << name << "UserData";
  std::string userDataName = ss.str();
  m_PhysicUserData = new CPhysicUserData(userDataName);
  m_PhysicUserData->SetPaint(false);
  m_PhysicUserData->SetColor(colRED);
  m_PhysicController = new CPhysicController(0.5, 0.25, 45, 0.1, 0.3, ECG_ESCENE, m_PhysicUserData, position, -gravity);
  PHYSXM->AddPhysicController(m_PhysicController);
}

CAIController::CAIController(std::string mesh, std::string name, Vect3f position,  float speed, float turnSpeed, float gravity, Vect2f controller_size):
  CObject3D (),
  m_Gravity(gravity),
  m_Speed(speed),
  m_TurnSpeed(turnSpeed),
  m_JumpForce(0),
  m_tiempoVidaDisparo(2.0),
  m_minAngleDisparo(0.2),
  m_BalaSpeed(5),
  m_CurrentJumpForce(0),
  m_IsOnCooldown(false),
  m_CooldownTimer(5),
  m_ProjectileHitbox(0.5),
  m_Angle(0.0f),
  m_EnemyHitboxX(1.5),//HARDCODED
  m_EnemyHitboxY(2.0),//HARDCODED
  m_CurrentCooldown(0),
  m_isJumping(false),
  m_SpeedModified(false) {
  for (int roomNum = 0; roomNum < RENDLM->GetSize() ; ++ roomNum) {
    CRenderableObjectsManager *l_Rom = RENDLM->GetRenderableObjectsManagerByStrAndRoom("enemies", roomNum);
    if (l_Rom->GetResource(mesh) != NULL)
      m_RenderableObject = l_Rom->GetResource(mesh);
  }
  m_Name = m_RenderableObject->getName();
  std::stringstream ss;
  ss << m_Name << "_UserData";
  std::string userDataName = ss.str();
  m_PhysicUserData = new CPhysicUserData(userDataName);
  m_PhysicUserData->SetPaint(false);
  m_PhysicUserData->SetColor(colRED);
  Vect3f newPosition = Vect3f(m_RenderableObject->GetPosition().x, m_RenderableObject->GetPosition().y + controller_size.y, m_RenderableObject->GetPosition().z);
  m_PhysicController = new CPhysicController(controller_size.x, controller_size.y, 45, 0.1, 0.3, ECG_ESCENE, m_PhysicUserData, newPosition, -gravity);//0.5,0.25

  PHYSXM->AddPhysicController(m_PhysicController);
}
CAIController::CAIController(CRenderableObject *rond, float speed, float turnSpeed, float gravity, Vect2f controller_size):
  CObject3D (),
  m_Gravity(gravity),
  m_Speed(speed),
  m_TurnSpeed(turnSpeed),
  m_JumpForce(0),
  m_tiempoVidaDisparo(2.0),
  m_minAngleDisparo(0.2),
  m_BalaSpeed(5),
  m_CurrentJumpForce(0),
  m_IsOnCooldown(false),
  m_CooldownTimer(5),
  m_ProjectileHitbox(0.5),
  m_Angle(0.0f),
  m_EnemyHitboxX(1.5),//HARDCODED
  m_EnemyHitboxY(2.0),//HARDCODED
  m_CurrentCooldown(0),
  m_isJumping(false),
  m_Name(rond->getName()),
  m_RenderableObject(rond),
  m_SpeedModified(false) {
  std::stringstream ss;
  ss << rond->getName() << "UserData";
  std::string userDataName = ss.str();
  m_PhysicUserData = new CPhysicUserData(userDataName);
  m_PhysicUserData->SetPaint(false);
  m_PhysicUserData->SetColor(colRED);
  Vect3f newPosition = Vect3f(rond->GetPosition().x, rond->GetPosition().y + controller_size.y, rond->GetPosition().z);
  m_PhysicController = new CPhysicController(controller_size.x, controller_size.y, 45, 0.1, 0.3, ECG_ESCENE, m_PhysicUserData, newPosition, -gravity);//0.5,0.25

  PHYSXM->AddPhysicController(m_PhysicController);
}
//CAIController::CAIController(std::string mesh, std::string name, Vect3f position):
//    CObject3D(),
//    m_Gravity(13),
//    m_Speed (0.5),
//    m_TurnSpeed(2.0),
//    m_JumpForce(1.5),
//    m_CurrentJumpForce(0),
//    m_isJumping(false),
//    m_Name(name),
//    m_Mesh(mesh)
//
//{
//    m_Position = position;
//    std::stringstream ss;
//    ss << name << "UserData";
//    std::string l_UserData = ss.str();
//    m_PhysicUserData = new CPhysicUserData(l_UserData);
//    m_PhysicUserData->SetPaint(true);
//    m_PhysicUserData->SetColor(colRED);
//    m_PhysicController = new CPhysicController(1, 2, 0.87, 0.1, 0.3, ECG_ENEMY, m_PhysicUserData, m_Position, -m_Gravity);
//    PHYSXM->AddPhysicController(m_PhysicController);
//}

CAIController::~CAIController() {
  PHYSXM->ReleasePhysicController(m_PhysicController);
  CHECKED_DELETE(m_PhysicUserData);
  CHECKED_DELETE(m_PhysicController);

}

void CAIController::SetShootingStats(float speedProjectile, float projectileLife, float cooldownTimer) {
  m_BalaSpeed = speedProjectile;
  m_CooldownTimer = cooldownTimer;
  m_tiempoVidaDisparo = projectileLife;
}



void CAIController::Move(float dt) {
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

void CAIController::MoveTo(float dt, Vect3f point) {
  if (point.Distance(m_Position) >= 2) {
    RotateYaw(dt, point);

    m_PhysicController->Move( Vect3f(1, 0, 0).RotateY(m_fYaw) * m_Speed, dt);
    SetPosition(m_PhysicController->GetPosition());
    Vect3f l_Position = (m_PhysicController->GetPosition());
    /*if (m_RenderableObject == NULL) { //TODO eliminar esto cuando funcione el nuevo sistema de Enemigos
         CRenderableObject *malla = RENDLM->GetRenderableObjectsManagerByStr("enemies")->GetResource("Enemy4");
         malla->SetYaw(m_fYaw);
         Vect3f position2 = malla->GetPosition();
         bool visible = malla->getVisible();
         malla->SetPosition(m_PhysicController->GetPosition());

         //FIN TODO
       } else {
      */
    if (m_RenderableObject != NULL)
      m_RenderableObject->SetPosition(m_PhysicController->GetPosition());
  }
  /* std::stringstream ss;
   ss << "Posicion de " << m_Name << ": " << m_RenderableObject->GetPosition().x << ", " << m_RenderableObject->GetPosition().y << ", " << m_RenderableObject->GetPosition().z ;
   std::string str = ss.str();
   CCORE->Trace(str);*/

}

void CAIController::MoveToPosition(Vect3f pos) {
  m_PhysicController->SetPosition(pos);
  Vect3f l_Position = m_PhysicController->GetPosition();
  SetPosition(l_Position);
  if (m_RenderableObject != NULL)
    m_RenderableObject->SetPosition(l_Position);
}


void CAIController::RotateOrMove(float dt, Vect3f point) {
  if (point.Distance(m_Position) >= 2) {
    Vect3f direction = (point - m_Position);
    Vect3f diff = Vect3f(1, 0, 0).RotateY(m_fYaw);
    float angle = getAngleDiff(direction, diff);
    if (angle > 0.5f)
      RotateYaw(dt, point);
    else {
      m_PhysicController->Move( direction.Normalize() * m_Speed, dt);
      SetPosition(m_PhysicController->GetPosition());
      Vect3f l_Position = (m_PhysicController->GetPosition());
      if (m_RenderableObject != NULL) {
        m_RenderableObject->SetPosition(m_PhysicController->GetPosition());
      }
    }
  }
}


Vect3f CAIController::UpdtateProjectilePosition(float dt) {
  return m_PosicionBala + m_DireccionBala.Normalize() * m_BalaSpeed * dt;
}

void CAIController::DestruirDisparo() {
  RENDLM->GetRenderableObjectsManagerByStr("enemies")->GetResource("disparo" + getName())->setPrintable(false);
}

bool CAIController::CheckPlayerShotCollision() {

  if ((RENDLM->GetRenderableObjectsManagerByStr("enemies")->GetResource("disparo" + getName())->getPrintable()) && (m_PosicionBala.Distance(PLAYC->GetPosition()) < m_ProjectileHitbox)) {
    return true;
  }
  return false;
}



void CAIController::RotateYaw(float dt, Vect3f point) {
  Vect3f direction = (point - m_Position);
  direction.y = 0;
  direction = direction.Normalize();
  Vect3f diff = Vect3f(1, 0, 0).RotateY(m_fYaw);
  float angle = getAngleDiff(direction, diff);
  RotateRenderable(dt, angle);
  //m_Position = m_Position + Vect3f(1,0,0).RotateZ(m_fYaw) * m_Speed * dt;
}

void CAIController::RotateRenderable(float dt, float angle) {
  m_fYaw =  m_fYaw - angle * m_TurnSpeed * dt;

  //m_PhysicController->Move( Vect3f(1, 0, 0).RotateY(m_fYaw) * m_Speed, dt);
  if (m_RenderableObject != NULL)
    m_RenderableObject->SetYaw(m_fYaw);
}

float CAIController::getAngleDiff(Vect3f A, Vect3f B) {
  float angle = m_Angle;
  float divisor = sqrt(A.x * A.x + A.y * A.y + A.z * A.z) * sqrt(B.x * B.x + B.y * B.y + B.z * B.z);

  if (divisor != 0) {
    float degangle = ((A.x * B.x + A.y * B.y + A.z * B.z) / divisor);
    //float angle = mathUtils::ACos(mathUtils::Deg2Rad(degangle));
    if (degangle > 1.0f)
      degangle = 1.0f;

    if (degangle < -1.0f)
      degangle = -1.0f;

    angle = mathUtils::ACos(degangle);
    m_Angle = angle;
  }

  //float degangle = ((A.x * B.x + A.y * B.y + A.z * B.z) / divisor);
  ////float angle = mathUtils::ACos(mathUtils::Deg2Rad(degangle));
  //float angle = mathUtils::ACos(degangle);
  /* float angle = A * B;
   angle = mathUtils::ACos(mathUtils::Deg2Rad(angle));
  */
  //std::stringstream ss;
  //ss << angle;
  //std::string str = ss.str();
  //if (str.find("IND") != std::string::npos) {
  //  printf("FUUUK");
  //}

  float dir = (A ^ B).y >= 0 ? 1 : -1;
  return (angle * dir);
}

bool CAIController::IsOnPlatform() {
  SCollisionInfo info = SCollisionInfo();
  CPhysicUserData *hit = CCORE->GetPhysicsManager()->RaycastClosestActor(Vect3f(m_PhysicController->GetPosition().x, m_PhysicController->GetPosition().y - m_PhysicController->GetHeight(), m_PhysicController->GetPosition().z),
                         Vect3f(0, -1, 0), 0xffffffff, info);

  if (hit != NULL && hit->GetMyCollisionGroup() == ECG_SCENE_ELEMENTS /*&& info.m_fDistance <= 0.4*/)
    return true;
  return false;
}