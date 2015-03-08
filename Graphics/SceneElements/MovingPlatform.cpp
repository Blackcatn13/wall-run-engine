#include "MovingPlatform.h"

#include "Core\Core.h"
#include "TriggerManager\Trigger.h"
#include "TriggerManager\TriggerManager.h"
#include "PhysicsManager.h"
#include "Renderable\RenderableObject.h"
#include "Utils\TemplatedVectorMapManager.h"
#include "Core\PlayerController.h"
#include "Utils\PhysicUserData.h"


CMovingPlatform::CMovingPlatform(std::string platformName, std::string coreName)
    : CStaticPlatform(platformName, coreName)
{
}


CMovingPlatform::~CMovingPlatform ()
{
}



