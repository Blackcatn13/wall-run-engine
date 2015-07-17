#include "WallTrap.h"

#include "Core\Core.h"
#include "PhysicsManager.h"
#include "Renderable\RenderableObject.h"
#include "Core\PlayerController.h"
#include "Utils\PhysicUserData.h"
#include "Actor\PhysicController.h"
#include "Utils\LuaGlobals.h"
#include "Core\ScriptManager.h"
#include "XML\XMLTreeNode.h"
#include "Actor\PhysicActor.h"

CWallTrap::CWallTrap(std::string platformName, std::string coreName, float speedOpening, float speedClosing, std::string side)
  : CStaticPlatform(platformName, coreName),
    m_SpeedOpening(speedOpening),
    m_OriginalPosition(v3fZERO),
    m_FinalPosition(v3fZERO),
    m_SpeedClosing(speedClosing),
    m_Side(side),
    m_Activated(false),
    m_TimeOut(0.0f),
    m_CurrentTime(0.0f) {
  /*  m_PhysicUserData = new CPhysicUserData("AI");
  m_PhysicUserData->SetPaint(true);
  m_PhysicUserData->SetColor(colRED);*/
  m_Fsm = new CFSMInstance(FSMMGR->GetResource("WallTrap"));
  m_Actor->Activate(true);
}

CWallTrap::CWallTrap(CXMLTreeNode &node)
  : CStaticPlatform(node),
    m_SpeedOpening(node.GetFloatProperty("speed_opening", 0.0f)),
    m_SpeedClosing(node.GetFloatProperty("speed_closing", 0.0f)),
    m_Side(node.GetPszISOProperty("side", "")),
    m_TimeOut(node.GetFloatProperty("time_out", 1.0f)),
    m_CurrentTime(0.0f) ,
    m_Activated(false),
    m_OriginalPosition(m_Position),
    m_FinalPosition(node.GetVect3fProperty("final_position", v3fZERO)) {
  m_Fsm = new CFSMInstance(FSMMGR->GetResource("WallTrap"));
  m_Actor->Activate(true);
}


CWallTrap::~CWallTrap () {
//  CHECKED_DELETE(m_Fsm);
}

void CWallTrap::MoveToPoint(float dt,  Vect3f point, float minDistance, float speed) {
  Vect3f direction = (point - m_Position);
  direction = direction.Normalize();
  if (point.Distance(m_Position) >= minDistance) {
    m_Position =  m_Position + /*Vect3f(1, 0, 0)*/direction.Normalize() * speed * dt;

    m_Actor->SetGlobalPosition(m_Position);
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
      //if (hit != NULL && hit->getName().substr(0, 6) == "Moving"/*"Player"*/ && info.m_fDistance <= 0.4) {
      //  PLAYC->getPhysicController()->Move(direction.Normalize() * speed * dt / 1.0, dt);
      //  PhysicsApplied = true;
      //}


      if ((PhysicsApplied == false) && (isAround(l_playerPosition, m_Position))) {
        Vect3f dirInvertida = -dirRay.Normalize();
        dirInvertida = Vect3f(dirInvertida.x, 0, dirInvertida.z);
        PLAYC->getPhysicController()->Move(dirInvertida * speed * 3 * dt  + direction.Normalize() * speed * dt, dt);
        //PLAYC->setCurrentJumpForce(0.0);
        //PLAYC->setisJumping(false);
        //PLAYC->setisGrounded(false);
        if (dirRay.y > 0.45) {
          PLAYC->getPhysicController()->Move(Vect3f(0, -1, 0) * speed * 4 * dt / 1.0, dt);
          PLAYC->setisJumping(false);
          PLAYC->setisGrounded(false);
          PLAYC->setCurrentJumpForce(0.0);
          PLAYC->setisJumpingMoving(false);
        }
      }

      //If inside lo pasamos a lua para matar al player
      //if (isInside(l_playerPosition, m_Position)) {
      //  //Hola
      //  float l_margenInferiorPlataforma = GetPhysicsSize().y + 0.9;
      //  PLAYC->getPhysicController()->SetPosition(m_Position + Vect3f(0, -l_margenInferiorPlataforma, 0));
      //}
    }
    //Caso vertical
    //else {
    //  dirRay = dirRay.Normalize() * 0.5;
    //  Vect3f dirRayBounding = Vect3f(dirRay.x, -l_RadioPhysicsPlayerVertical, dirRay.z);
    //  CPhysicUserData *hit = CCORE->GetPhysicsManager()->RaycastClosestActor(Vect3f(l_playerPosition.x + dirRayBounding.x, l_playerPosition.y + dirRayBounding.y, l_playerPosition.z + dirRayBounding.z),
    //                         Vect3f(0, -1, 0), 0xffffffff, info);

    //  //Intentando arreglar que al saltar debajado de la plataforma el personaje se queda incrustado
    //  /*if ((dirRay.y > (0.15)) && (hit != NULL && hit->getName().substr(0,6) == "Moving" && info.m_fDistance <= 0.9))
    //  {
    //  	PLAYC->getPhysicController()->Move(Vect3f(0.0, -1.0, 0.0) * speed    * dt , dt);
    //  	PLAYC->setCurrentJumpForce(0.0);
    //  	PLAYC->setisJumping(false);
    //  	PLAYC->setisGrounded(false);
    //  }
    //  else*/
    //  if ((l_playerPosition.y > (m_Position.y + l_PosicionMinSobrePlat) && (l_playerPosition.y < (m_Position.y + l_PosicionMaxSobrePlat)) && (hit != NULL && hit->getName().substr(0, 6) == "Moving" && info.m_fDistance <= 5.0))) {
    //    //PLAYC->getPhysicController()->MovePlayer(direction.Normalize() * speed * dt + PLAYC->getGravity() * Vect3f(0,1,0) * 1.2 * dt, dt);
    //    //PLAYC->IsGrounded(direction.Normalize() * speed * dt / 1.0, dt);
    //    if (!PLAYC->getisJumping() || PLAYC->getCurrentJumpForce() < 0) {
    //      PLAYC->setisOnPlatform(0.0);
    //      Vect3f l_PlayerPosition = PLAYC->getPhysicController()->GetPosition();
    //      l_playerPosition.y = m_Position.y + l_DesplazamientoVerticalPlataforma;
    //      PLAYC->getPhysicController()->SetPosition(l_playerPosition);
    //      PLAYC->SetPosition(l_playerPosition);
    //      PLAYC->setisJumping(false);
    //      PLAYC->setisGrounded(true);
    //      PLAYC->setCurrentJumpForce(0.0);
    //      PLAYC->setisJumpingMoving(false);
    //    }
    //  } else {
    //    PLAYC->setisOnPlatform(1.0);

    //    if (isAround(l_playerPosition, m_Position)) {
    //      if (dirRay.y > 0.45) {
    //        PLAYC->getPhysicController()->Move(Vect3f(0, -1, 0) * speed * 4 * dt / 1.0, dt);
    //        //PLAYC->setisJumping(false);
    //        //PLAYC->setisGrounded(false);
    //        //PLAYC->setCurrentJumpForce(0.0);
    //        //PLAYC->setisJumpingMoving(false);
    //      } else {
    //        PLAYC->getPhysicController()->Move(-dirRay.Normalize() * speed * 3 * dt / 1.0, dt);
    //      }
    //      //PLAYC->setCurrentJumpForce(0.0);
    //      //PLAYC->setisJumping(false);
    //      //PLAYC->setisGrounded(false);
    //    }
    //    if (isInside(l_playerPosition, m_Position)) {
    //      //Hola
    //      float l_margenInferiorPlataforma = GetPhysicsSize().y * 2;
    //      PLAYC->getPhysicController()->SetPosition(l_playerPosition + Vect3f(0, -l_margenInferiorPlataforma, 0));
    //    }
    //  }
    //}
  }
}



void CWallTrap::Update(float ElapsedTime) {
  UpdateFSM(ElapsedTime);
}



//bool CWallTrap::isAround(Vect3f vector1, Vect3f vector2) {
//  float l_margenx = GetPhysicsSize().x + 0.5f;
//  float l_margenz = GetPhysicsSize().z + 0.5f;
//  float l_margeny = GetPhysicsSize().y + 1.5f;
//  // bool l_isInside = isInside(vector1, vector2);
//  if ((vector1.x > vector2.x - l_margenx) && (vector1.x < vector2.x + l_margenx) && (vector1.y > vector2.y - l_margeny) && (vector1.y < vector2.y + l_margeny) && (vector1.z > vector2.z - l_margenz) && (vector1.z < vector2.z + l_margenz))
//    return true;
//  else
//    return false;
//}