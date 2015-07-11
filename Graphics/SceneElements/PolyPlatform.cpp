#include "PolyPlatform.h"
#include "PhysicsManager.h"
#include "Actor\PhysicActor.h"
#include "Core\PlayerController.h"
#include "Core\Core.h"
#include "Utils\PhysicUserData.h"
#include "Core\ScriptManager.h"
#include "Utils\Logger.h"
#include "Lights\Light.h"
#include "Lights\LightManager.h"
#include "XML\XMLTreeNode.h"

CPolyPlatform::CPolyPlatform(std::string platformName, std::string coreName,  Vect3f finalPosition, Vect3f direction, float activationDistance, float timeOut, float speed)
  : CStaticPlatform(platformName, coreName),
    m_FinalPosition(finalPosition),
    m_Direction(direction),
    m_OriginalPosition(m_Position),
    //m_RedimAxis(redimAxis),
    m_Enabled(false),
    //  m_Collission(initialCollision),
    m_ActivationDistance(activationDistance),
    m_ActiveTime(.0f),
    m_Activated(false),
    m_Light(NULL),
    m_LightOriginalPosition(NULL),
    m_IsMoving(false),
    m_TimeOut(timeOut),
    m_Speed(speed) {
  if (m_Speed <= 20) //Intentar buscar formula matematica que de algo parecido ò_ó
    // m_Margin = 0.2;
    m_Margin = 0.5;
  else if ( m_Speed > 20 && m_Speed <= 25)
    m_Margin = 0.5;
  else
    m_Margin = 0.9;
  m_Actor->Activate(true);
}

CPolyPlatform::CPolyPlatform(const CXMLTreeNode &node)
  : CStaticPlatform(node),
    m_FinalPosition(node.GetVect3fProperty("final_position",  v3fZERO)),
    m_Direction(node.GetVect3fProperty("direction",  v3fZERO)),
    m_OriginalPosition(m_Position),
    m_Enabled(false),
    m_ActivationDistance(node.GetFloatProperty("activation_distance", .0f)),
    m_ActiveTime(.0f),
    m_Activated(false),
    m_Light(NULL),
    m_LightOriginalPosition(NULL),
    m_IsMoving(false),
    m_TimeOut(node.GetFloatProperty("time_out", .0f)),
    m_Speed(node.GetFloatProperty("speed", .0f)) {
  if (m_Speed <= 20) //Intentar buscar formula matematica que de algo parecido ò_ó
    // m_Margin = 0.2;
    m_Margin = 0.5;
  else if ( m_Speed > 20 && m_Speed <= 25)
    m_Margin = 0.5;
  else
    m_Margin = 0.9;
  m_Actor->Activate(true);
}

CPolyPlatform::~CPolyPlatform() {
}

void CPolyPlatform:: ActivatePoly() {
  if (!m_Activated) {
    // float l_NewScale = 0.0f;
    // if (m_Collission) {//bajando
    // l_NewScale = 0;
    // m_PlatorformActor->Activate(false);
    //   m_Collission = false;
    if (m_Position.Distance(m_FinalPosition) >= m_Margin/*0.9*/) {
      Vect3f l_NewPosition =  m_Position + (m_Direction * m_Speed * m_Dt);
      ApplyPhysicsToPlayer(m_Direction, m_Dt);
      m_Actor->SetGlobalPosition(l_NewPosition);
      m_Position = l_NewPosition;
      if (m_Light != NULL)
        m_Light->SetPosition(l_NewPosition);
      m_IsMoving = true;
      //Si colisiona con Piky => Desplazarle
    } else {
      m_Position = m_FinalPosition;
      ApplyPhysicsToPlayer(m_Direction, m_Dt);
      m_Actor->SetGlobalPosition(m_FinalPosition);
      m_Activated = true;
      m_IsMoving = false;
    }
    /*} else {
        m_PlatorformActor->MoveGlobalPosition(-m_Direction);
    */
  }
  /* if (m_RedimAxis == "y")
       CCORE->GetPlayerController()->getPhysicController()->Move(direction, m_Dt);*/
  // } else {//subiendo
  // l_NewScale = m_RedimScale;
  //   m_PlatorformActor->Activate(true);
  //   m_Collission = true;
  //}
  //     if (m_RedimAxis == "x") {
  //if (m_Position.x <= m_FinalPosition.x){
  //
  //}
  //         //SetScale(Vect3f(l_NewScale, GetScale().y, GetScale().z));
  //     } else if (m_RedimAxis == "y") {
  //         //SetScale(Vect3f(GetScale().x, l_NewScale , GetScale().z));
  //     } else if (m_RedimAxis == "z") {
  //         //SetScale(Vect3f(GetScale().x, GetScale().y , l_NewScale));
  //     } else {
  //         std::string msg_error = "No redimension Axis defined";
  //         LOGGER->AddNewLog(ELL_INFORMATION, msg_error.c_str());
  //     }
}

void CPolyPlatform:: DeactivatePoly() {
  if (m_Activated) {
    if (m_Position.Distance(m_OriginalPosition) >= m_Margin /*0.9*/) {
      Vect3f l_NewPosition =  m_Position + ( m_Direction * m_Speed * m_Dt * -1);
      ApplyPhysicsToPlayer(m_Direction * -1, m_Dt);
      m_Actor->SetGlobalPosition(l_NewPosition);
      m_Position = l_NewPosition;
      if (m_Light != NULL)
        m_Light->SetPosition(l_NewPosition);
      //Si colisiona con Piky => Desplazarle
    } else {
      m_Activated = false;
      m_ActiveTime = 0.0f;
      m_Position = m_OriginalPosition;
      m_Actor->SetGlobalPosition(m_OriginalPosition);
      if (m_Light != NULL && m_LightOriginalPosition != NULL)
        m_Light->SetPosition(m_LightOriginalPosition);
    }
    //m_PlatorformActor->Activate(!m_Collission);
    //if (m_Collission) {
    //    m_Collission = false;
    //    // if (m_RedimAxis == "y")
    //    CCORE->GetPlayerController()->getPhysicController()->Move(direction, m_Dt);
    //} else {
    //    m_Collission = true;
    //    //Hacer que el player suba si es necesario
    //}
    //SetScale(m_OriginalScale);
    // m_PlatorformActor->Activate(m_Collission);
  }
  /*if (m_Activated) {
      m_ActiveTime = 0.0f;
      m_PlatorformActor->Activate(false);
      SetScale(m_OriginalScale);
      if (m_Collission) {
          m_Activated = false;
          if (m_RedimAxis == "y")
              CCORE->GetPlayerController()->getPhysicController()->Move(Vect3f(.0f, -1.f, .0f), m_Dt);
      }
  } else
      m_PlatorformActor->Activate(true);*/
}
void CPolyPlatform::Update(float ElapsedTime) {
  //Esto para el lua de momento
  /*float l_Distance = CCORE->GetPlayerController()->GetPosition().Distance(m_Position);
  if (l_Distance < m_ActivationDistance)
      m_Enabled = true;
  else
      m_Enabled = false;
  */
  int room = PLAYC->getRoom() ;
  if (PLAYC->getRoom() == m_Room) {
    m_Dt = ElapsedTime;
    if (m_Activated)
      m_ActiveTime = m_ActiveTime + 1 * ElapsedTime;
    std::stringstream ss;
    ss << "update_poly_platform" << "(" << m_ActiveTime << "," << ElapsedTime << ",\"" << m_Name << "\")";
    std::string toRun = ss.str();
    SCRIPTM->RunCode(toRun.c_str());
  }
}


void CPolyPlatform::ApplyPhysicsToPlayer(Vect3f direction, float dt) {

  Vect3f l_playerPosition = PLAYC->GetPosition();
  Vect3f dirRay = (m_Position - l_playerPosition);
  Vect3f l_dirRayFixed = Vect3f(dirRay.x, 0, dirRay.y);
  l_dirRayFixed = l_dirRayFixed.Normalize();
  //solo hacemos los calculos de colisiones si la plataforma esta a una distancia minima (optimización)
  if (abs(dirRay.x) + abs(dirRay.z) < (GetPhysicsSize().x + GetPhysicsSize().z + 1.0)) {
    SCollisionInfo info = SCollisionInfo();
    float l_RadioPhysicsPlayer = 0.8;
    //float l_AlturaPlataformaDesdeOrigen = GetPhysicsSize().y + 1.5;
    //float l_MargenLateralPlataforma = 1.0;
    float l_DesplazamientoVerticalPlataforma = (GetPhysicsSize().y) * 2 + 1.5f;
    float l_PosicionMinSobrePlat = 0.0f;
    float l_PosicionMaxSobrePlat = (GetPhysicsSize().y) * 2 + 2.5f;
    //float l_DesplazamientoVerticalPlataforma = 6.5f;
    //float l_PosicionMinSobrePlat = 0.0f;
    //float l_PosicionMaxSobrePlat = 7.0f;

    //para saber si es movimiento horizontal o vertical
    //Caso horizo	ntal
    if ((abs(direction.z) + abs(direction.x)) > abs(direction.y)) {
      PLAYC->setisOnPlatform(1.0);
      bool PhysicsApplied = false;
      l_dirRayFixed = l_dirRayFixed.Normalize() * 0.5;
      Vect3f dirRayBounding = Vect3f(l_dirRayFixed.x, -l_RadioPhysicsPlayer, l_dirRayFixed.z);
      //Vect3f dirRayBounding = dirRay * l_RadioPhysicsPlayer;
      CPhysicUserData *hit = CCORE->GetPhysicsManager()->RaycastClosestActor(Vect3f(l_playerPosition.x + dirRayBounding.x, l_playerPosition.y + dirRayBounding.y, l_playerPosition.z + dirRayBounding.z),
                             Vect3f(0, -1, 0), 0xffffffff, info);

      //Intentando arreglar que al saltar debajado de la plataforma el personaje se queda incrustado
      if (hit != NULL && (hit->getName().substr(0, 4) == "Poly" || hit->getName().substr(0, 4) == "POLY")  && info.m_fDistance <= 1.2) {
        PLAYC->getPhysicController()->Move(direction.Normalize() * m_Speed * dt, dt);
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
          PLAYC->getPhysicController()->Move(Vect3f(0, -1, 0) * m_Speed * 4 * dt, dt);
          //PLAYC->setisJumping(false);
          //PLAYC->setisGrounded(true);
          //PLAYC->setCurrentJumpForce(0.0);
          //PLAYC->setisJumpingMoving(false);
        }
      }

      if (isInside(l_playerPosition, m_Position)) {
        //Hola
        float l_margenInferiorPlataforma = GetPhysicsSize().y * 2;
        PLAYC->getPhysicController()->SetPosition(l_playerPosition + Vect3f(0, -l_margenInferiorPlataforma, 0));
      }
    }
    //Caso vertical
    else {
      l_dirRayFixed = l_dirRayFixed.Normalize() * 0.5;
      Vect3f dirRayBounding = Vect3f(l_dirRayFixed.x, -l_RadioPhysicsPlayer, l_dirRayFixed.z);
      CPhysicUserData *hit = CCORE->GetPhysicsManager()->RaycastClosestActor(Vect3f(l_playerPosition.x + dirRayBounding.x, l_playerPosition.y + dirRayBounding.y, l_playerPosition.z + dirRayBounding.z),
                             Vect3f(0, -1, 0), 0xffffffff, info);

      //Intentando arreglar que al saltar debajado de la plataforma el personaje se queda incrustado
      /*if ((dirRay.y > (0.15)) && ((hit->getName().substr(0, 4) == "Poly" || hit->getName().substr(0, 4) == "POLY") && info.m_fDistance <= 0.9))
      {
      	PLAYC->getPhysicController()->Move(Vect3f(0.0, -1.0, 0.0) * m_Speed    * dt , dt);
      	PLAYC->setCurrentJumpForce(0.0);
      	PLAYC->setisJumping(false);
      	PLAYC->setisGrounded(false);
      }
      else*/
      if ((l_playerPosition.y > (m_Position.y + l_PosicionMinSobrePlat) && (l_playerPosition.y < (m_Position.y + l_PosicionMaxSobrePlat)) && ((hit->getName().substr(0, 4) == "Poly" || hit->getName().substr(0, 4) == "POLY") && info.m_fDistance <= 5.0))) {
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
        } else {
          PLAYC->setisOnPlatform(1.0);
          //PLAYC->IsGrounded(Vect3f(0.0,-1.0,0.0),dt);
        }
      } else {
        PLAYC->setisOnPlatform(1.0);

        if (isAround(l_playerPosition, m_Position)) {
          Vect3f dirInvertida = -dirRay.Normalize();
          dirInvertida = Vect3f(dirInvertida.x, 0, dirInvertida.z);
          PLAYC->getPhysicController()->Move(dirInvertida * m_Speed * 3 * dt  + direction.Normalize() * m_Speed * dt, dt);
          if (dirRay.y > 0.45) {
            PLAYC->getPhysicController()->Move(Vect3f(0, -1, 0) * m_Speed * 4 * dt, dt);
            PLAYC->setisJumping(false);
            PLAYC->setisGrounded(false);
            PLAYC->setCurrentJumpForce(0.0);
            PLAYC->setisJumpingMoving(false);
          }
          //PLAYC->setCurrentJumpForce(0.0);
          //PLAYC->setisJumping(false);
          //PLAYC->setisGrounded(false);
        }
        if (isInside(l_playerPosition, m_Position)) {
          //Hola
          float l_margenInferiorPlataforma = GetPhysicsSize().y + 0.9;
          PLAYC->getPhysicController()->SetPosition(m_Position + Vect3f(0, -l_margenInferiorPlataforma, 0));
        }
      }
    }
  }
}

//bool CPolyPlatform::isInside(Vect3f vector1, Vect3f vector2) {
//  //FIX BUG DETECTA PLAYER INSIDE
//  float l_margenx = GetPhysicsSize().x + 0.0f;
//  float l_margenz = GetPhysicsSize().z + 0.0f;
//  float l_margeny = GetPhysicsSize().y + 0.0f;
//  if ((vector1.x > vector2.x - l_margenx) && (vector1.x < vector2.x + l_margenx) && (vector1.y > vector2.y - l_margeny) && (vector1.y < vector2.y + l_margeny) && (vector1.z > vector2.z - l_margenz) && (vector1.z < vector2.z + l_margenz))
//    return true;
//  else
//    return false;
//}

bool CPolyPlatform::isAround(Vect3f vector1, Vect3f vector2) {
  float l_margenx = GetPhysicsSize().x + 0.8f;
  float l_margenz = GetPhysicsSize().z + 0.8f;
  float l_margeny = GetPhysicsSize().y + 1.8f;
  /*bool l_isInside = false;
  l_isInside = isInside(vector1, vector2);*/
  if ((vector1.x > vector2.x - l_margenx) && (vector1.x < vector2.x + l_margenx) && (vector1.y > vector2.y - l_margeny) && (vector1.y < vector2.y + l_margeny) && (vector1.z > vector2.z - l_margenz) && (vector1.z < vector2.z + l_margenz))
    return true;
  else
    return false;
}