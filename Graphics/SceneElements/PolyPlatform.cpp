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
    m_Enabled(false),
    m_ActivationDistance(activationDistance),
    m_ActiveTime(.0f),
    m_Activated(false),
    m_Light(NULL),
    m_LightOriginalPosition(NULL),
    m_IsMoving(false),
    m_TimeOut(timeOut),
    m_Speed(speed) {
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
  m_Actor->Activate(true);
}

CPolyPlatform::~CPolyPlatform() {
}

bool CPolyPlatform::GetDifferencePositions(Vect3f direction, Vect3f position, Vect3f finalPosition) {
  bool l_Result = false;
  int Axis = GetAxis(direction);
  //Poly horizontales seran instantaneas, las verticales seran graduales
  if (Axis != 0) {
    if (Axis != 2) l_Result = true;
    else if (position.Distance(finalPosition) <= 0.3 || position[Axis - 1] >= finalPosition[Axis - 1]) l_Result = true;
  }
  return l_Result;
}

void CPolyPlatform:: ActivatePoly() {
  //Cuando se presiona M, la poly se activa
  if (!m_Activated) {
    if (!GetDifferencePositions(m_Direction, m_Position, m_FinalPosition)) {
      Vect3f l_NewPosition =  m_Position + (m_Direction * m_Speed * m_Dt);
      m_Actor->SetGlobalPosition(l_NewPosition);
      m_Position = l_NewPosition;

      m_IsMoving = true;
      //Si colisiona con Piky => Desplazarle
      ApplyPhysicsToPlayer(m_Direction, m_Dt);
    } else {
      m_Position = m_FinalPosition;
      m_Actor->SetGlobalPosition(m_FinalPosition);
      m_Activated = true;

      m_IsMoving = false;
      ApplyPhysicsToPlayer(m_Direction, m_Dt);
    }
  }
}

void CPolyPlatform:: DeactivatePoly() {
  //Desactivación después del movimiento
  if (m_Activated) {
    if (!GetDifferencePositions(-m_Direction, m_OriginalPosition, m_Position)) {
      Vect3f l_NewPosition =  m_Position + ( -m_Direction * m_Speed * m_Dt);
      m_Actor->SetGlobalPosition(l_NewPosition);
      m_Position = l_NewPosition;
      if (m_Direction.y != 0)
        PLAYC->getPhysicController()->ReportSceneChanged();
      //Si colisiona con Piky => Desplazarle
      ApplyPhysicsToPlayer(-m_Direction, m_Dt);
    } else {
      m_Position = m_OriginalPosition;
      m_Actor->SetGlobalPosition(m_OriginalPosition);
      m_Activated = false;
      m_IsMoving = false;
      m_ActiveTime = 0.0f;

      ApplyPhysicsToPlayer(-m_Direction, m_Dt);
      if (m_Direction.y == 0) {

        PLAYC->getPhysicController()->ReportSceneChanged();
      }
      //  if (PLAYC->getisGrounded() && !PLAYC->getisJumping() && !PLAYC->getisFalling())
      //   PLAYC->getPhysicController()->Move(Vect3f(0, -1, 0) , m_Dt);
    }
  }
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
      m_ActiveTime = m_ActiveTime + ElapsedTime;
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
    float l_DesplazamientoVerticalPlataforma = (GetPhysicsSize().y) * 2 + 1.5f;
    float l_PosicionMinSobrePlat = 0.0f;
    float l_PosicionMaxSobrePlat = (GetPhysicsSize().y) * 2 + 2.5f;
    //para saber si es movimiento horizontal o vertical
    //Caso horizontal
    if ((abs(direction.z) + abs(direction.x)) > abs(direction.y)) {
      PLAYC->setisOnPlatform(1.0);
      bool PhysicsApplied = false;
      l_dirRayFixed = l_dirRayFixed.Normalize() * 0.5;
      Vect3f dirRayBounding = Vect3f(l_dirRayFixed.x, -l_RadioPhysicsPlayer, l_dirRayFixed.z);
      CPhysicUserData *hit = CCORE->GetPhysicsManager()->RaycastClosestActor(Vect3f(l_playerPosition.x + dirRayBounding.x,
                             l_playerPosition.y + dirRayBounding.y, l_playerPosition.z + dirRayBounding.z), Vect3f(0, -1, 0), 0xffffffff, info);
      //Intentando arreglar que al saltar debajado de la plataforma el personaje se queda incrustado
      if (hit != NULL && (hit->GetMyCollisionGroup() == ECG_SCENE_ELEMENTS)  && info.m_fDistance <= 1.2) {
        PLAYC->getPhysicController()->Move(direction.Normalize() * m_Speed * dt, dt);
        PhysicsApplied = true;
      }
      if ((PhysicsApplied == false) && (isAround(l_playerPosition, m_Position))) {
        Vect3f dirInvertida = -dirRay.Normalize();
        dirInvertida = Vect3f(dirInvertida.x, 0, dirInvertida.z);
        PLAYC->getPhysicController()->Move(dirInvertida * m_Speed * 3 * dt  + direction.Normalize() * m_Speed * dt, dt);
        if (dirRay.y > 0.45) {
          PLAYC->getPhysicController()->Move(Vect3f(0, -1, 0) * m_Speed * 4 * dt, dt);
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
      CPhysicUserData *hit = CCORE->GetPhysicsManager()->RaycastClosestActor(Vect3f(l_playerPosition.x + dirRayBounding.x,
                             l_playerPosition.y + dirRayBounding.y, l_playerPosition.z + dirRayBounding.z), Vect3f(0, -1, 0), 0xffffffff, info);
      if (hit != NULL && (l_playerPosition.y > (m_Position.y + l_PosicionMinSobrePlat) && (l_playerPosition.y < (m_Position.y + l_PosicionMaxSobrePlat)) && (hit->GetMyCollisionGroup() == ECG_SCENE_ELEMENTS && info.m_fDistance <= 5.0))) {
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
        }
        if (isInside(l_playerPosition, m_Position)) {
         // float l_margenInferiorPlataforma = GetPhysicsSize().y + 0.9;
			Vect3f l_NearestDirection = GetNearestExitDirection();
			PLAYC->getPhysicController()->SetPosition(PLAYC->getPhysicController()->GetPosition() + (l_NearestDirection*m_Speed*dt));
        }
      }
    }//Final caso vertical/horizontal
  }
}

Vect3f CPolyPlatform::GetNearestExitDirection(){
	Vect3f l_TempDirection =  PLAYC->GetPosition() - m_Position;
	Vect3f l_DirectionXZ = Vect3f(l_TempDirection.x, 0.0f, l_TempDirection.z).Normalize();
	 return Vect3f(l_DirectionXZ.x, 1, l_DirectionXZ.z);
}

bool CPolyPlatform::isAround(Vect3f vector1, Vect3f vector2) {
  float l_margenx = GetPhysicsSize().x + 0.8f;
  float l_margenz = GetPhysicsSize().z + 0.8f;
  float l_margeny = GetPhysicsSize().y + 1.8f;
  if ((vector1.x > vector2.x - l_margenx) && (vector1.x < vector2.x + l_margenx) && (vector1.y > vector2.y - l_margeny)
      && (vector1.y < vector2.y + l_margeny) && (vector1.z > vector2.z - l_margenz) && (vector1.z < vector2.z + l_margenz))
    return true;
  else
    return false;
}

int CPolyPlatform::GetAxis(Vect3f direction) {
  int l_Result = 0;
  if (direction.x != 0) l_Result = 1;
  else if (direction.y != 0) l_Result = 2;
  else if (direction.z != 0) l_Result = 3;
  /*else if (direction.x < 0) l_Result = -1;
  else if (direction.y < 0) l_Result = -2;
  else if (direction.z < 0) l_Result = -3;*/
  return l_Result;
}