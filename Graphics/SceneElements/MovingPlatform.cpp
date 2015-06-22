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
#include "XML\XMLTreeNode.h"

CMovingPlatform::CMovingPlatform(std::string platformName, std::string coreName, float speed)
  : CStaticPlatform(platformName, coreName),
    m_Speed(speed),
    m_CurrentWpId(0),
    m_WayPointsVector(NULL),
    m_Activated(false) {
  /*  m_PhysicUserData = new CPhysicUserData("AI");
  m_PhysicUserData->SetPaint(true);
  m_PhysicUserData->SetColor(colRED);*/
  m_Fsm = new CFSMInstance(FSMMGR->GetResource("MovingPlatform"));
  m_Actor->Activate(true);
}

CMovingPlatform::CMovingPlatform(CXMLTreeNode &node)
  : CStaticPlatform(node),
    m_Speed(node.GetFloatProperty("speed", 0.0f)),
    m_CurrentWpId(0),
    m_Activated(false),
    m_Fsm(new CFSMInstance(FSMMGR->GetResource("MovingPlatform"))) {
  int wpCount = node.GetNumChildren();
  for (int i = 0; i < wpCount; ++i) {
    std::string name = node(i).GetName();
    if (name == "wp") {
      Vect3f l_Wp = node(i).GetVect3fProperty("pos", Vect3f(.0f, .0f, .0f), false);
      m_WayPointsVector.push_back(l_Wp);
    }
  }
  m_NextWP = m_WayPointsVector[1];
  m_Actor->Activate(true);
}

void CMovingPlatform::AddBoxController(Vect3f size, float slope, float skinwidth, float offset, float gravity) {
  //m_PhysicController = new CPhysicController(radius, heigh, slope, skinwidth, offset, ECG_ESCENE, m_PlatformUserData, m_PlatorformActor->GetPosition(), .0f);
  //m_PhysicController = new CPhysicController(size, slope, skinwidth, offset, ECG_DYNAMIC_OBJECTS, m_PlatformUserData, Vect3f(GetPosition().x, GetPosition().y + 1, GetPosition().z));
  //m_PhysicController->SetType(EControleType::BOX);
  //m_PhysicController->UseGravity(false);
  //m_PhysicController->GetUserData()->GetFlags();
  //  float heigh = m_PhysicController->GetHeight();
  //PHYSXM->AddPhysicController(m_PhysicController);
  m_Actor->Activate(false);
  // m_PlatorformActor->CreateBody(0.5f);
  //m_PlatorformActor->SetKinematic(true);
  // m_PlatorformActor->SetLinearVelocity(Vect3f(1.0f, .0f, .0f) * m_Speed);
}

CMovingPlatform::~CMovingPlatform () {
  CHECKED_DELETE(m_Fsm);
}

void CMovingPlatform::MoveToPoint(float dt,  Vect3f point, float minDistance) {
  Vect3f direction = (point - m_Position);
  direction = direction.Normalize();
  if (point.Distance(m_Position) >= minDistance) {
    m_Position =  m_Position + /*Vect3f(1, 0, 0)*/direction.Normalize() * m_Speed * dt;
    //m_PlatorformActor->MoveGlobalPosition(m_Position);
    //   Vect3f vel = m_PlatorformActor->GetLinearVelocity();
    m_Actor->SetGlobalPosition(m_Position);
    //EUserDataFlag hit = m_PhysicController->GetUserData()->GetFlags();
    //m_PlatorformActor->AddVelocityAtPos(direction.Normalize(), Vect3f(-1.0f, .0f, .0f), 3.0f);
    // m_PhysicController->Move(direction.Normalize() * m_Speed / 100, dt);
    //  m_Position = m_PhysicController->GetPosition();
    //Vect3f l_VelPos = m_Position - (Vect3f(GetScale().x / 2, .0f, .0f));
    //m_PlatorformActor->AddVelocityAtPos(direction.Normalize(), v3fZERO, 10);
    // m_PlatorformActor->SetLinearVelocity(direction.Normalize() * m_Speed * dt);
  }
  Vect3f l_playerPosition = PLAYC->GetPosition();
  Vect3f dirRay = (m_Position - l_playerPosition);
  //solo hacemos los calculos de colisiones si la plataforma esta a una distancia minima (optimización)
  if (abs(dirRay.x) + abs(dirRay.z) < (GetPhysicsSize().x + GetPhysicsSize().z + 1.0)) {
    SCollisionInfo info = SCollisionInfo();
    float l_RadioPhysicsPlayerVertical = 0.8;
    float l_RadioPhysicsPlayerHorizontal = 0.8;
    //float l_AlturaPlataformaDesdeOrigen = GetPhysicsSize().y + 1.5;
    //float l_MargenLateralPlataforma = 1.0;
    float l_DesplazamientoVerticalPlataforma = GetPhysicsSize().y + 1.5f;
    float l_PosicionMinSobrePlat = GetPhysicsSize().y + 0.2f;
    float l_PosicionMaxSobrePlat = GetPhysicsSize().y + 2.5f;

    //para saber si es movimiento horizontal o vertical
    //Caso horizontal
    if ((abs(direction.z) + abs(direction.x)) > abs(direction.y)) {
      bool PhysicsApplied = false;
      dirRay = dirRay.Normalize() * 0.4f;
      Vect3f dirRayBounding = Vect3f(dirRay.x, -l_RadioPhysicsPlayerHorizontal, dirRay.z);
      //Vect3f dirRayBounding = dirRay * l_RadioPhysicsPlayer;

      CPhysicUserData *hit = CCORE->GetPhysicsManager()->RaycastClosestActor(Vect3f(l_playerPosition.x + dirRayBounding.x, l_playerPosition.y + dirRayBounding.y, l_playerPosition.z + dirRayBounding.z),
                             Vect3f(0, -1, 0), 0xffffffff, info);

      //Intentando arreglar que al saltar debajado de la plataforma el personaje se queda incrustado
      if (hit != NULL && hit->getName().substr(0, 6) == "Moving"/*"Player"*/ && info.m_fDistance <= 0.4) { //Carlos: Cambio "Moving" por "Player" xq por alguna razón dejaba de moverse con la plataforma
        PLAYC->getPhysicController()->Move(direction.Normalize() * m_Speed * dt / 1.0, dt);
        PhysicsApplied = true;
      }

      if ((PhysicsApplied == false) && (isAround(l_playerPosition, m_Position))) {
        Vect3f dirInvertida = -dirRay.Normalize();
        dirInvertida = Vect3f(dirInvertida.x, 0, dirInvertida.z);
        PLAYC->getPhysicController()->Move(dirInvertida * m_Speed * 3 * dt  + direction.Normalize() * m_Speed * dt, dt);
        //PLAYC->setCurrentJumpForce(0.0);
        //PLAYC->setisJumping(false);
        //PLAYC->setisGrounded(false);
        if (dirRay.y > 0.45) {
          PLAYC->getPhysicController()->Move(Vect3f(0, -1, 0) * m_Speed * 4 * dt / 1.0, dt);
          PLAYC->setisJumping(false);
          PLAYC->setisGrounded(false);
          PLAYC->setCurrentJumpForce(0.0);
          PLAYC->setisJumpingMoving(false);
        }
      }

      if (isInside(l_playerPosition, m_Position)) {
        //Hola
        float l_margenInferiorPlataforma = GetPhysicsSize().y + 0.9;
        PLAYC->getPhysicController()->SetPosition(m_Position + Vect3f(0, -l_margenInferiorPlataforma, 0));
      }
    }
    //Caso vertical
    else {
      dirRay = dirRay.Normalize() * 0.5;
      Vect3f dirRayBounding = Vect3f(dirRay.x, -l_RadioPhysicsPlayerVertical, dirRay.z);
      CPhysicUserData *hit = CCORE->GetPhysicsManager()->RaycastClosestActor(Vect3f(l_playerPosition.x + dirRayBounding.x, l_playerPosition.y + dirRayBounding.y, l_playerPosition.z + dirRayBounding.z),
                             Vect3f(0, -1, 0), 0xffffffff, info);

      //Intentando arreglar que al saltar debajado de la plataforma el personaje se queda incrustado
      /*if ((dirRay.y > (0.15)) && (hit != NULL && hit->getName().substr(0,6) == "Moving" && info.m_fDistance <= 0.9))
      {
      	PLAYC->getPhysicController()->Move(Vect3f(0.0, -1.0, 0.0) * m_Speed    * dt , dt);
      	PLAYC->setCurrentJumpForce(0.0);
      	PLAYC->setisJumping(false);
      	PLAYC->setisGrounded(false);
      }
      else*/
      if ((l_playerPosition.y > (m_Position.y + l_PosicionMinSobrePlat) && (l_playerPosition.y < (m_Position.y + l_PosicionMaxSobrePlat)) && (hit != NULL && hit->getName().substr(0, 6) == "Moving" && info.m_fDistance <= 5.0))) {
        //PLAYC->getPhysicController()->MovePlayer(direction.Normalize() * m_Speed * dt + PLAYC->getGravity() * Vect3f(0,1,0) * 1.2 * dt, dt);
        //PLAYC->IsGrounded(direction.Normalize() * m_Speed * dt / 1.0, dt);
        if (!PLAYC->getisJumping() || PLAYC->getCurrentJumpForce() < 0) {
          PLAYC->setisOnPlatform(0.0);
          Vect3f l_PlayerPosition = PLAYC->getPhysicController()->GetPosition();
          l_playerPosition.y = m_Position.y + l_DesplazamientoVerticalPlataforma;
          PLAYC->getPhysicController()->SetPosition(l_playerPosition);
          PLAYC->SetPosition(l_playerPosition);
          PLAYC->setisJumping(false);
          PLAYC->setisGrounded(true);
          PLAYC->setCurrentJumpForce(0.0);
          PLAYC->setisJumpingMoving(false);
        }
      } else {
        PLAYC->setisOnPlatform(1.0);

        if (isAround(l_playerPosition, m_Position)) {
          if (dirRay.y > 0.45) {
            PLAYC->getPhysicController()->Move(Vect3f(0, -1, 0) * m_Speed * 4 * dt / 1.0, dt);
            //PLAYC->setisJumping(false);
            //PLAYC->setisGrounded(false);
            //PLAYC->setCurrentJumpForce(0.0);
            //PLAYC->setisJumpingMoving(false);
          } else {
            PLAYC->getPhysicController()->Move(-dirRay.Normalize() * m_Speed * 3 * dt / 1.0, dt);
          }
          //PLAYC->setCurrentJumpForce(0.0);
          //PLAYC->setisJumping(false);
          //PLAYC->setisGrounded(false);
        }
        if (isInside(l_playerPosition, m_Position)) {
          //Hola
          float l_margenInferiorPlataforma = GetPhysicsSize().y * 2;
          PLAYC->getPhysicController()->SetPosition(l_playerPosition + Vect3f(0, -l_margenInferiorPlataforma, 0));
        }
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

//bool CMovingPlatform::isInside(Vect3f vector1, Vect3f vector2) {
//  float l_margenx = GetPhysicsSize().x;
//  float l_margenz = GetPhysicsSize().z;
//  float l_margeny = GetPhysicsSize().y;
//  if ((vector1.x > vector2.x - l_margenx) && (vector1.x < vector2.x + l_margenx) && (vector1.y > vector2.y - l_margeny) && (vector1.y < vector2.y + l_margeny) && (vector1.z > vector2.z - l_margenz) && (vector1.z < vector2.z + l_margenz))
//    return true;
//  else
//    return false;
//}

void CMovingPlatform::UpdateFSM(float elapsedTime) {
// for (TMapResource::iterator it = m_Resources.begin(); it != m_Resources.end(); ++it) {
  STATE *s = m_Fsm->getStates().GetResource(m_Fsm->getCurrentState());
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
    m_Fsm->setPreviousState(m_Fsm->getCurrentState());
    m_Fsm->setCurrentState(CLuaGlobals::getInstance()->getString());
  }
}

bool CMovingPlatform::isAround(Vect3f vector1, Vect3f vector2) {
  float l_margenx = GetPhysicsSize().x + 0.5f;
  float l_margenz = GetPhysicsSize().z + 0.5f;
  float l_margeny = GetPhysicsSize().y + 1.5f;
  // bool l_isInside = isInside(vector1, vector2);
  if ((vector1.x > vector2.x - l_margenx) && (vector1.x < vector2.x + l_margenx) && (vector1.y > vector2.y - l_margeny) && (vector1.y < vector2.y + l_margeny) && (vector1.z > vector2.z - l_margenz) && (vector1.z < vector2.z + l_margenz))
    return true;
  else
    return false;
}