#include "BreakablePlatform.h"
#include "Actor\PhysicActor.h"
#include "Core\Core.h"
#include "TriggerManager\Trigger.h"
#include "TriggerManager\TriggerManager.h"
#include "PhysicsManager.h"
#include "Core\PlayerController.h"
#include "Utils\PhysicUserData.h"
#include "XML\XMLTreeNode.h"

CBreakablePlatform::CBreakablePlatform(std::string platformName, std::string coreName, std::string triggerName)
  : CStaticPlatform(platformName, coreName),
    m_TriggerName(triggerName) {
  m_Actor->Activate(true);
}

CBreakablePlatform::CBreakablePlatform(const CXMLTreeNode &node)
  : CStaticPlatform(node),
    m_TriggerName(node.GetPszISOProperty("trigger_name", "")) {
  m_Actor->Activate(true);
}

CBreakablePlatform::~CBreakablePlatform() {
}

void CBreakablePlatform::DisablePlatform(float dt, Vect3f direction) {
  m_UserData->SetPaint(false);
  m_Actor->Activate(false);
  m_Printable = false;
  TRIGGM->GetResource(m_TriggerName)->setUpdate(false);
  TRIGGM->GetResource(m_TriggerName)->Activate(false);
  //Vect3f l_CurrentPosition = Vect3f (CCORE->GetPlayerController()->GetPosition());
  CCORE->GetPlayerController()->getPhysicController()->Move(direction, dt);
}

