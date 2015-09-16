#include "Switch.h"
#include "XML\XMLTreeNode.h"
#include "Actor\PhysicActor.h"
#include "Core\Core.h"
#include "Core\PlayerController.h"
#include "Utils\PhysicUserData.h"
#include "PhysicsManager.h"
#include "Core\ScriptManager.h"

CSwitch::CSwitch(const CXMLTreeNode &node)
  : CSceneElement(node),
    m_Switched(false) {
  InsertPhisic(Vect3f(.0f, m_PhysicsSize.y, .0f));
  m_OriginalPosition = m_Position;
}

CSwitch::CSwitch(std::string switchName, std::string coreName)
  : CSceneElement(switchName, coreName, false),
    m_Switched(false) {
  InsertPhisic(Vect3f(.0f, m_PhysicsSize.y, .0f));
  m_OriginalPosition = m_Position;
}

void CSwitch::ResetSwitch() {
  if (m_Position != m_OriginalPosition) {
    SetPosition(m_OriginalPosition);
    m_Actor->SetGlobalPosition(m_OriginalPosition);
  }
}

void CSwitch::Update(float dt) {
  if (m_Position.Distance(PLAYC->GetPosition()) < 10) {
    SCollisionInfo info = SCollisionInfo();
    Vect3f l_playerPosition = PLAYC->GetPosition();
    CPhysicUserData *hit = CCORE->GetPhysicsManager()->RaycastClosestActor(Vect3f(l_playerPosition.x , l_playerPosition.y  - 1, l_playerPosition.z + 0),
                           Vect3f(0, -1, 0), 0xffffffff, info);
    float height = PLAYC->getPhysicController()->GetHeight();
    if (hit != NULL && hit->GetMyCollisionGroup() == ECG_SCENE_ELEMENTS && info.m_fDistance <= height && !m_Switched && PLAYC->getisGrounded() && !PLAYC->getisFalling() && !PLAYC->getisJumping()) {
      m_Switched = true;
      Vect3f actor_position = m_Actor->GetPosition();
      m_Actor->SetGlobalPosition(Vect3f(actor_position.x, actor_position.y - 1, actor_position.z));
      m_Position = Vect3f(m_Position.x, m_Position.y - 1, m_Position.z);
      //Por si se queda tonto en el aire
      //if (m_Direction.y == 0) {
      float yaw = PLAYC->GetYaw();
      Vect3f vecAux = Vect3f(sin(yaw), 0, cos(yaw));
      vecAux = vecAux * dt * PLAYC->getSpeed();
      PLAYC->IsGrounded(vecAux, dt, true);
      //}
    }
  }
}

CSwitch::~CSwitch() {
}

