#include "MovingPlatform.h"

#include "Core\Core.h"
#include "TriggerManager\Trigger.h"
#include "TriggerManager\TriggerManager.h"
#include "PhysicsManager.h"
#include "Renderable\RenderableObject.h"
#include "Utils\TemplatedVectorMapManager.h"
#include "Core\PlayerController.h"
#include "Utils\PhysicUserData.h"


CMovingPlatform::CMovingPlatform(std::string platformName, std::string coreName, float speed)
    : CStaticPlatform(platformName, coreName),
      m_Speed(speed),
      m_CurrentWpId(0),
      m_WayPointsVector(NULL)
{
    /*  m_PhysicUserData = new CPhysicUserData("AI");
    m_PhysicUserData->SetPaint(true);
    m_PhysicUserData->SetColor(colRED);
    m_PhysicController = new CPhysicController(1, 2, 0.87, 0.1, 0.3, ECG_ESCENE, m_PhysicUserData, Vect3f(-15, 5, -15), -m_Gravity);
    PHYSXM->AddPhysicController(m_PhysicController);*/
}


CMovingPlatform::~CMovingPlatform ()
{
}

void CMovingPlatform::MoveToPoint(float dt,  Vect3f point, float minDistance)
{
    if (point.Distance(m_Position) >= minDistance) {
        Vect3f direction = (point - m_Position);
        m_Position =  m_Position + /*Vect3f(1, 0, 0)*/direction.Normalize() * m_Speed * dt;
        m_PlatorformActor->SetKinematic(true);
        m_PlatorformActor->SetGlobalPosition(m_Position);
        //Vect3f l_VelPos = m_Position - (Vect3f(GetScale().x / 2, .0f, .0f));
        //m_PlatorformActor->AddVelocityAtPos(direction.Normalize(), v3fZERO, 10);
        //m_PlatorformActor->SetLinearVelocity(direction.Normalize() * m_Speed * dt);
    }
}


Vect3f CMovingPlatform::GetNextWP()
{
    Vect3f  l_NextWp = m_WayPointsVector[m_CurrentWpId];
    ++ m_CurrentWpId;
    if ( m_CurrentWpId == m_WayPointsVector.size())
        m_CurrentWpId = 0;
    return l_NextWp;
}
