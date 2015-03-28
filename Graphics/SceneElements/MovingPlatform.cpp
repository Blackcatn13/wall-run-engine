#include "MovingPlatform.h"

#include "Core\Core.h"
#include "TriggerManager\Trigger.h"
#include "TriggerManager\TriggerManager.h"
#include "PhysicsManager.h"
#include "Renderable\RenderableObject.h"
#include "Utils\TemplatedVectorMapManager.h"
#include "Core\PlayerController.h"
#include "Utils\PhysicUserData.h"
#include "Actor\PhysicController.h"
#include "Utils\LuaGlobals.h"
#include "Core\ScriptManager.h"


CMovingPlatform::CMovingPlatform(std::string platformName, std::string coreName, float speed)
  : CStaticPlatform(platformName, coreName),
    m_Speed(speed),
    m_CurrentWpId(0),
    m_WayPointsVector(NULL) {
  /*  m_PhysicUserData = new CPhysicUserData("AI");
  m_PhysicUserData->SetPaint(true);
  m_PhysicUserData->SetColor(colRED);*/
  m_Fsm = FSMMGR->GetResource("MovingPlatform");
}

void CMovingPlatform::AddBoxController(Vect3f size, float slope, float skinwidth, float offset, float gravity) {
  //m_PhysicController = new CPhysicController(radius, heigh, slope, skinwidth, offset, ECG_ESCENE, m_PlatformUserData, m_PlatorformActor->GetPosition(), .0f);
  //m_PhysicController = new CPhysicController(size, slope, skinwidth, offset, ECG_DYNAMIC_OBJECTS, m_PlatformUserData, Vect3f(GetPosition().x, GetPosition().y + 1, GetPosition().z));
  //m_PhysicController->SetType(EControleType::BOX);
  //m_PhysicController->UseGravity(false);
  //m_PhysicController->GetUserData()->GetFlags();
  //  float heigh = m_PhysicController->GetHeight();
  //PHYSXM->AddPhysicController(m_PhysicController);
  m_PlatorformActor->Activate(false);
  // m_PlatorformActor->CreateBody(0.5f);
  //m_PlatorformActor->SetKinematic(true);
  // m_PlatorformActor->SetLinearVelocity(Vect3f(1.0f, .0f, .0f) * m_Speed);
}

CMovingPlatform::~CMovingPlatform () {
  /* PHYSXM->ReleasePhysicController(m_PhysicController);
   CHECKED_DELETE(m_PhysicController);*/
}

void CMovingPlatform::MoveToPoint(float dt,  Vect3f point, float minDistance) {
  if (point.Distance(m_Position) >= minDistance) {
    Vect3f direction = (point - m_Position);
    m_Position =  m_Position + /*Vect3f(1, 0, 0)*/direction.Normalize() * m_Speed * dt;
    //m_PlatorformActor->MoveGlobalPosition(m_Position);
    //   Vect3f vel = m_PlatorformActor->GetLinearVelocity();
    m_PlatorformActor->SetGlobalPosition(m_Position);
    //EUserDataFlag hit = m_PhysicController->GetUserData()->GetFlags();
    //m_PlatorformActor->AddVelocityAtPos(direction.Normalize(), Vect3f(-1.0f, .0f, .0f), 3.0f);
    // m_PhysicController->Move(direction.Normalize() * m_Speed / 100, dt);
    //  m_Position = m_PhysicController->GetPosition();
    //Vect3f l_VelPos = m_Position - (Vect3f(GetScale().x / 2, .0f, .0f));
    //m_PlatorformActor->AddVelocityAtPos(direction.Normalize(), v3fZERO, 10);
    // m_PlatorformActor->SetLinearVelocity(direction.Normalize() * m_Speed * dt);

    SCollisionInfo info = SCollisionInfo();
    Vect3f l_playerPosition = PLAYC->GetPosition();
    Vect3f dirRay = (m_Position - l_playerPosition);
    //solo hacemos los calculos de colisiones si la plataforma esta a una distancia minima (optimización)
    if (abs(dirRay.x) + abs(dirRay.z) < 20) {
      dirRay.Normalize();
      Vect3f dirRayBounding = dirRay * 0.4;
      CPhysicUserData *hit = CCORE->GetPhysicsManager()->RaycastClosestActor(Vect3f(l_playerPosition.x + dirRayBounding.x, l_playerPosition.y + dirRayBounding.y, l_playerPosition.z + dirRayBounding.z),
                             dirRay, 0xffffffff, info);

      //Intentando arreglar que al saltar debajado de la plataforma el personaje se queda incrustado
      if ((dirRay.y > (0.15)) && (hit != NULL && hit->getName().substr(0, 6) == "Moving" && info.m_fDistance <= 0.9)) {
        PLAYC->getPhysicController()->Move(Vect3f(0.0, -1.0, 0.0) * m_Speed    * dt , dt);
        PLAYC->setCurrentJumpForce(0.0);
        PLAYC->setisJumping(false);
        PLAYC->setisGrounded(false);
      } else if (hit != NULL && hit->getName().substr(0, 6) == "Moving" && info.m_fDistance <= 0.8) {
        PLAYC->getPhysicController()->Move(direction.Normalize() * m_Speed * dt / 1.0, dt);
      }
    }
  }
}


Vect3f CMovingPlatform::GetNextWP() {
  Vect3f  l_NextWp = m_WayPointsVector[m_CurrentWpId];
  ++ m_CurrentWpId;
  if ( m_CurrentWpId == m_WayPointsVector.size())
    m_CurrentWpId = 0;
  return l_NextWp;
}

void CMovingPlatform::Update(float ElapsedTime) {
  UpdateFSM(ElapsedTime);
}

void CMovingPlatform::UpdateFSM(float elapsedTime) {
// for (TMapResource::iterator it = m_Resources.begin(); it != m_Resources.end(); ++it) {
  STATE *s = m_Fsm->m_States.GetResource(m_Fsm->m_currentState);
  char l_InitLevelText[256];
  if (s->m_onEnter == false) {
    _snprintf_s(l_InitLevelText, 256, 256, "%s(\"%s\")", s->onEnter.c_str(), getName().c_str());
    SCRIPTM->RunCode(l_InitLevelText);
    s->m_onEnter = true;
  }
  s->m_ElapsedTime += elapsedTime;
// char l_InitLevelText[256];
  int doComprobation = 0;
  if (s->m_ElapsedTime >= s->m_UpdateTime) {
    doComprobation = 1;
  }
  _snprintf_s(l_InitLevelText, 256, 256, "%s(%f,\"%s\")", s->onUpdate.c_str(), elapsedTime, getName().c_str());
  SCRIPTM->RunCode(l_InitLevelText);
  if (doComprobation == 1) {
    s->m_ElapsedTime = 0;
    doComprobation = 0;
  }
  bool change = CLuaGlobals::getInstance()->ValueChanged();
  if (change) {
    s->m_onEnter = false;
    SCRIPTM->RunCode(s->onExit.c_str());
    m_Fsm->m_previousState = m_Fsm->m_currentState;
    m_Fsm->m_currentState = CLuaGlobals::getInstance()->getString();
  }
}