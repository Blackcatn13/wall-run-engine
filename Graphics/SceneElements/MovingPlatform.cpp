#include "MovingPlatform.h"

#include "Core\Core.h"
#include "TriggerManager\Trigger.h"
#include "TriggerManager\TriggerManager.h"
#include "PhysicsManager.h"
#include "Renderable\RenderableObject.h"
#include "Utils\TemplatedVectorMapManager.h"
#include "Core\PlayerController.h"
#include "Utils\PhysicUserData.h"
#include "Actor\PhysicController.h"


CMovingPlatform::CMovingPlatform(std::string platformName, std::string coreName, float speed)
    : CStaticPlatform(platformName, coreName),
      m_Speed(speed),
      m_CurrentWpId(0),
      m_WayPointsVector(NULL)
{
    /*  m_PhysicUserData = new CPhysicUserData("AI");
    m_PhysicUserData->SetPaint(true);
    m_PhysicUserData->SetColor(colRED);*/
}

void CMovingPlatform::AddBoxController(Vect3f size, float slope, float skinwidth, float offset, float gravity)
{
    //m_PhysicController = new CPhysicController(radius, heigh, slope, skinwidth, offset, ECG_ESCENE, m_PlatformUserData, m_PlatorformActor->GetPosition(), .0f);
    //m_PhysicController = new CPhysicController(size, slope, skinwidth, offset, ECG_DYNAMIC_OBJECTS, m_PlatformUserData, Vect3f(GetPosition().x, GetPosition().y + 1, GetPosition().z));
    //m_PhysicController->SetType(EControleType::BOX);
    //m_PhysicController->UseGravity(false);
    //m_PhysicController->GetUserData()->GetFlags();
    //  float heigh = m_PhysicController->GetHeight();
    //PHYSXM->AddPhysicController(m_PhysicController);
    m_PlatorformActor->Activate(false);
    // m_PlatorformActor->CreateBody(0.5f);
    //m_PlatorformActor->SetKinematic(true);
    // m_PlatorformActor->SetLinearVelocity(Vect3f(1.0f, .0f, .0f) * m_Speed);
}

CMovingPlatform::~CMovingPlatform ()
{
    /* PHYSXM->ReleasePhysicController(m_PhysicController);
     CHECKED_DELETE(m_PhysicController);*/
}

void CMovingPlatform::MoveToPoint(float dt,  Vect3f point, float minDistance)
{
    if (point.Distance(m_Position) >= minDistance) {
        Vect3f direction = (point - m_Position);
        m_Position =  m_Position + /*Vect3f(1, 0, 0)*/direction.Normalize() * m_Speed * dt;
        //m_PlatorformActor->MoveGlobalPosition(m_Position);
        //   Vect3f vel = m_PlatorformActor->GetLinearVelocity();
        m_PlatorformActor->SetGlobalPosition(m_Position);
        //m_PlatorformActor->AddVelocityAtPos(direction.Normalize(), Vect3f(-1.0f, .0f, .0f), 3.0f);
        // m_PhysicController->Move(direction.Normalize() * m_Speed / 100, dt);
        //  m_Position = m_PhysicController->GetPosition();
        //Vect3f l_VelPos = m_Position - (Vect3f(GetScale().x / 2, .0f, .0f));
        //m_PlatorformActor->AddVelocityAtPos(direction.Normalize(), v3fZERO, 10);
        // m_PlatorformActor->SetLinearVelocity(direction.Normalize() * m_Speed * dt);
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
