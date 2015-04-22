#include "StaticPlatform.h"
#include "Actor\PhysicActor.h"
#include "Utils\PhysicUserData.h"
#include "PhysicsManager.h"
#include "Renderable\RenderableObjectsManager.h"
#include "Renderable\RenderableObject.h"
#include "Renderable\RenderableObjectsLayersManager.h"
#include "Core\Core.h"
#include "Utils\Defines.h"


CStaticPlatform::CStaticPlatform(std::string platformName, std::string coreName)
    : CMeshInstance(platformName, coreName)
{
}

CStaticPlatform::~CStaticPlatform ()
{
    PHYSXM->ReleasePhysicActor(m_PlatorformActor);
    CHECKED_DELETE(m_PlatorformActor);
    CHECKED_DELETE(m_PlatformUserData);
}


void CStaticPlatform::InsertPlatform(std::string userDataName, Vect3f size, Vect3f localPosition)
{
    // m_RenderableObject = RENDLM->GetDefaultRenderableObjectManager()->GetResource(boxName);
    m_PlatformUserData = new CPhysicUserData(userDataName);
    m_PlatformUserData->SetPaint(false);
    m_PlatorformActor = new CPhysicActor(m_PlatformUserData);
	m_PlatformPhysicsSize = Vect3f(size.x, size.y, size.z);
    // m_PlatorformActor->AddBoxSphape(size, m_RenderableObject->GetPosition(), localPosition);
    m_PlatorformActor->AddBoxSphape(size, m_Position, localPosition);
    //m_RenderableObject->SetYaw(m_fYaw);
    // m_PlatorformActor->CreateBody(0.5f);
    PHYSXM->AddPhysicActor(m_PlatorformActor);
    //m_RenderableObject->SetPosition(m_RenderableObject->GetPosition());
}