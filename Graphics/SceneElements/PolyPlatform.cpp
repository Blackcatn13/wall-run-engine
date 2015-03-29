#include "PolyPlatform.h"
#include "PhysicsManager.h"
#include "Actor\PhysicActor.h"
#include "Core\PlayerController.h"
#include "Core\Core.h"
#include "Utils\PhysicUserData.h"
#include "Core\ScriptManager.h"
#include "Utils\Logger.h"


CPolyPlatform::CPolyPlatform(std::string platformName, std::string coreName,  Vect3f finalPosition, Vect3f direction, float activationDistance)
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
    m_IsMoving(false) {
}

CPolyPlatform::~CPolyPlatform () {
  //PHYSXM->ReleasePhysicActor(m_PlatorformActor);
}


void CPolyPlatform:: ActivatePoly() {
  if (!m_Activated) {
    // float l_NewScale = 0.0f;
    // if (m_Collission) {//bajando
    // l_NewScale = 0;
    // m_PlatorformActor->Activate(false);
    //   m_Collission = false;
    if (m_Position.Distance(m_FinalPosition) >= 0.5) {
      Vect3f l_NewPosition =  m_Position + (m_Direction * m_Speed * m_Dt);
      m_PlatorformActor->SetGlobalPosition(l_NewPosition);
      m_Position = l_NewPosition;
      m_IsMoving = true;
      //Si colisiona con Piky => Desplazarle
    } else {
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
    if (m_Position.Distance(m_OriginalPosition) >= 0.5) {
      Vect3f l_NewPosition =  m_Position + ( m_Direction * m_Speed * m_Dt * -1);
      m_PlatorformActor->SetGlobalPosition(l_NewPosition);
      m_Position = l_NewPosition;
      //Si colisiona con Piky => Desplazarle
    } else {
      m_Activated = false;
      m_ActiveTime = 0.0f;
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
  m_Dt = ElapsedTime;
  if (m_Activated)
    m_ActiveTime = m_ActiveTime + 1 * ElapsedTime;
  std::stringstream ss;
  ss << "update_poly_platform" << "(" << m_ActiveTime << "," << ElapsedTime << ",\"" << m_Name << "\")";
  std::string toRun = ss.str();
  SCRIPTM->RunCode(toRun.c_str());
}


void CPolyPlatform::ApplyPhysicsToPlayer(Vect3f direction, float dt) 
{
	direction = direction.Normalize();
	SCollisionInfo info = SCollisionInfo();
	Vect3f l_playerPosition = PLAYC->GetPosition();
	Vect3f dirRay = (m_Position - l_playerPosition);
	//solo hacemos los calculos de colisiones si la plataforma esta a una distancia minima (optimización)
	if (abs(dirRay.x) + abs(dirRay.z) < 8)
	{
		//para saber si es movimiento horizontal o vertical
		if ((abs(direction.z) + abs(direction.x)) > abs(direction.y))
		{
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
			} 
			else if (hit != NULL && hit->getName().substr(0, 4) == "Poly" && info.m_fDistance <= 0.8) {
			PLAYC->getPhysicController()->Move(direction.Normalize() * m_Speed * dt / 1.0, dt);
			}
		 }
		else
		{
			 dirRay.Normalize();
			//Vect3f dirRayBounding = dirRay * 0.4;
			Vect3f dirRayBounding = Vect3f(0,-0.4,0);
			CPhysicUserData* hit = CCORE->GetPhysicsManager()->RaycastClosestActor(Vect3f(l_playerPosition.x + dirRayBounding.x, l_playerPosition.y + dirRayBounding.y, l_playerPosition.z + dirRayBounding.z),
								Vect3f(0,-1,0), 0xffffffff, info);

			//Intentando arreglar que al saltar debajado de la plataforma el personaje se queda incrustado
			/*if ((dirRay.y > (0.15)) && (hit != NULL && hit->getName().substr(0,6) == "Moving" && info.m_fDistance <= 0.9))
			{
				PLAYC->getPhysicController()->Move(Vect3f(0.0, -1.0, 0.0) * m_Speed    * dt , dt);
				PLAYC->setCurrentJumpForce(0.0);
				PLAYC->setisJumping(false);
				PLAYC->setisGrounded(false);
			}
			else*/
			if (hit != NULL && hit->getName().substr(0,4) == "Poly" && info.m_fDistance <= 6)
			{
				//PLAYC->getPhysicController()->MovePlayer(direction.Normalize() * m_Speed * dt + PLAYC->getGravity() * Vect3f(0,1,0) * 1.2 * dt, dt);
				//PLAYC->IsGrounded(direction.Normalize() * m_Speed * dt / 1.0, dt);
				if (!PLAYC->getisJumping() || PLAYC->getCurrentJumpForce() < 0)
				{
					PLAYC->setGravity(0.0);
					Vect3f l_PlayerPosition = PLAYC->getPhysicController()->GetPosition();
					l_playerPosition.y = m_Position.y + 3;
					PLAYC->getPhysicController()->SetPosition(l_playerPosition);
					PLAYC->SetPosition(l_playerPosition);
					PLAYC->setisJumping(false);
					PLAYC->setisGrounded(false);
					PLAYC->setCurrentJumpForce(0.0);
					PLAYC->setisJumpingMoving(false);
				}
			}
			else
			{
				PLAYC->setGravity(2.0);
			}
		}
	}
}