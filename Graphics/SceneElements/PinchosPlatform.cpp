#include "PinchosPlatform.h"

#include "Core\Core.h"
#include "TriggerManager\Trigger.h"
#include "TriggerManager\TriggerManager.h"
#include "PhysicsManager.h"
#include "Renderable\RenderableObject.h"
#include "Utils\TemplatedVectorMapManager.h"
#include "Core\PlayerController.h"
#include "Utils\PhysicUserData.h"


CPinchosPlatform::CPinchosPlatform(std::string platformName, std::string coreName, std::string triggerName, float offset, bool fromX, bool fromZ)
    : CBreakablePlatform(platformName, coreName, triggerName),
      m_Offset(offset),
      m_FromZ(fromZ),
      m_FromX(fromX)

{
}



CPinchosPlatform::~CPinchosPlatform ()
{
}

void CPinchosPlatform::FallingIntoPlatform()
{
    Vect3f l_CurrentPosition = Vect3f (CCORE->GetPlayerController()->getPhysicController()->GetPosition());
    Vect3f l_Direction;
    //Calcular direccion en funcion del punto de caida para que siempre retroceda
    l_Direction = (m_Position - l_CurrentPosition);
    Vect3f l_newPosition = m_Position;
    //calcular nueva posicion
    if (m_FromX) {
        //Mirar como obtener bien la dirección
        // if (l_Direction.Normalize().x >= 0) {
        if (l_CurrentPosition.x >= m_Position.x)
            l_newPosition.x = m_Position.x + m_Offset;
        else
            l_newPosition.x = m_Position.x - m_Offset;
        /*} else {
            if (l_CurrentPosition.x >= m_Position.x)
                l_newPosition.x = m_Position.x + m_Offset;
            else
                l_newPosition.x = m_Position.x - m_Offset;
        }*/
    }
    if (m_FromZ) {
        if ( l_CurrentPosition.z >= m_Position.z)
            l_newPosition.z = m_Position.z + m_Offset;
        else
            l_newPosition.z = m_Position.z - m_Offset;
    }
    //Vect3f l_newPosition = l_CurrentPosition - l_Direction.Normalize() * m_Offset;
    // CCORE->GetPlayerController()->SetPosition(l_newPosition);
    CCORE->GetPlayerController()->getPhysicController()->SetPosition(Vect3f(l_newPosition.x, 3.0f, l_newPosition.z));
    //TODO restar corazoncito
}

/*
void CBreakablePlatform::DisablePlatform(float dt)
{
    m_PlatformUserData->SetPaint(false);
    m_PlatorformActor->Activate(false);
    m_Printable = false;
    TRIGGM->GetResource(m_TriggerName)->setUpdate(false);
    TRIGGM->GetResource(m_TriggerName)->Activate(false);
    Vect3f l_CurrentPosition = Vect3f (CCORE->GetPlayerController()->GetPosition());
    CCORE->GetPlayerController()->getPhysicController()->Move(Vect3f(.0f, -1.f, .0f), dt);
}

*/