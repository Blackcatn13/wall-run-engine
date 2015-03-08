#include "BreakablePlatform.h"

#include "Core\Core.h"
#include "TriggerManager\Trigger.h"
#include "TriggerManager\TriggerManager.h"
#include "PhysicsManager.h"
#include "Renderable\RenderableObject.h"
#include "Utils\TemplatedVectorMapManager.h"
#include "Core\PlayerController.h"


CBreakablePlatform::CBreakablePlatform(std::string platformName, std::string coreName, std::string triggerName)
    : CStaticPlatform(platformName, coreName),
      m_TriggerName(triggerName)
{
}



CBreakablePlatform::~CBreakablePlatform ()
{
}

void CBreakablePlatform::DisablePlatform()
{
    m_PlatorformActor->Activate(false);
    m_Printable = false;
    TRIGGM->GetResource(m_TriggerName)->setUpdate(false);
}

