#include "BreakablePlatform.h"

#include "Core\Core.h"
#include "TriggerManager\Trigger.h"
#include "TriggerManager\TriggerManager.h"
#include "PhysicsManager.h"
#include "Core\PlayerController.h"
#include "Utils\PhysicUserData.h"
#include "XML\XMLTreeNode.h"

CBreakablePlatform::CBreakablePlatform(std::string platformName, std::string coreName, std::string triggerName)
  : CStaticPlatform(platformName, coreName, true),
    m_TriggerName(triggerName),
    m_Broken(false) {
  m_Actor->Activate(true);
  m_ActorOriginalPosition = m_Actor->GetPosition();
  m_ActorOriginalRotation = m_Actor->GetRotation();
  m_Actor->SetKinematic(true);
}

CBreakablePlatform::CBreakablePlatform(const CXMLTreeNode &node)
  : CStaticPlatform(node, true),
    m_Broken(false),
    m_TriggerName(node.GetPszISOProperty("trigger_name", "")) {
  m_Actor->Activate(true);
  m_ActorOriginalPosition = m_Actor->GetPosition();
  m_ActorOriginalRotation = m_Actor->GetRotation();
  m_Actor->SetKinematic(true);
}

CBreakablePlatform::~CBreakablePlatform() {
}


std::string SetUserDataName(std::string name) {
  std::stringstream ss;
  ss << name << "_UserData";
  return ss.str();
}
void CBreakablePlatform::DisablePlatform(float dt/*, Vect3f direction*/) {
  /* m_UserData->SetPaint(false);
   m_Actor->Activate(false);
   m_Printable = false;*/
  TRIGGM->GetResource(m_TriggerName)->setUpdate(false);
  TRIGGM->GetResource(m_TriggerName)->Activate(false);
  //Vect3f l_CurrentPosition = Vect3f (CCORE->GetPlayerController()->GetPosition());
// CCORE->GetPlayerController()->getPhysicController()->Move(direction, dt);
// m_ActorAux->Activate(false);
  m_Actor->SetKinematic(false);
  m_Actor->AddImpulseAtPos(Vect3f(0, -1, 0), Vect3f(0, 0, 0), 1.0);
  m_Broken = true;
}

void CBreakablePlatform::EnablePlatform() {
  if (m_Broken && m_Actor != NULL) {

    //  m_ActorAux->Activate(true);
    m_Actor->SetKinematic(true);
// m_ActorAux->SetGlobalPosition(Vect3f(m_ActorOriginalPosition.x, m_ActorOriginalPosition.y - m_PhysicsSize.y, m_ActorOriginalPosition.z));
    m_Actor->SetGlobalPosition(m_ActorOriginalPosition);
    m_Actor->SetRotation(m_ActorOriginalRotation);
    //m_Printable = true;
    //TRIGGM->GetResource(m_TriggerName)->setUpdate(false);
    TRIGGM->GetResource(m_TriggerName)->Activate(true);

    m_Broken = false;

    //Vect3f l_CurrentPosition = Vect3f (CCORE->GetPlayerController()->GetPosition());
    //CCORE->GetPlayerController()->getPhysicController()->Move(direction, dt);
  }
}


void CBreakablePlatform::Update(float ElapsedTime) {
  m_Position = m_Actor->GetPosition();
  m_fYaw = m_Actor->GetRotation().x;
  m_fPitch = m_Actor->GetRotation().y;
  m_fRoll = m_Actor->GetRotation().z;
}