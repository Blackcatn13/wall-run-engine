#include "Switch.h"
#include "Actor\PhysicActor.h"
#include "Utils\PhysicUserData.h"
#include "PhysicsManager.h"
#include "Renderable\RenderableObjectsManager.h"
#include "Renderable\RenderableObject.h"
#include "Renderable\RenderableObjectsLayersManager.h"
#include "Core\Core.h"
#include "Utils\Defines.h"


CSwitch::CSwitch(std::string switchName, std::string coreName)
    : CMeshInstance(switchName, coreName)
{
}

CSwitch::~CSwitch ()
{
    PHYSXM->ReleasePhysicActor(m_SwitchActor);
    CHECKED_DELETE(m_SwitchActor);
    CHECKED_DELETE(m_SwitchUserData);
}


void CSwitch::InsertPhisicSwitch(std::string userDataName, Vect3f size, Vect3f localPosition)
{
    // m_RenderableObject = RENDLM->GetDefaultRenderableObjectManager()->GetResource(boxName);
    m_SwitchUserData = new CPhysicUserData(userDataName);
    m_SwitchUserData->SetPaint(false);
    m_SwitchActor = new CPhysicActor(m_SwitchUserData);
	m_SwitchPhysicsSize = Vect3f(size.x, size.y, size.z);
    // m_PlatorformActor->AddBoxSphape(size, m_RenderableObject->GetPosition(), localPosition);
    m_SwitchActor->AddBoxSphape(size, m_Position, localPosition);
    //m_RenderableObject->SetYaw(m_fYaw);
    // m_PlatorformActor->CreateBody(0.5f);
    PHYSXM->AddPhysicActor(m_SwitchActor);
    //m_RenderableObject->SetPosition(m_RenderableObject->GetPosition());
}