#include "StaticPlatform.h"
#include "Actor\PhysicActor.h"
#include "Utils\PhysicUserData.h"
#include "PhysicsManager.h"
#include "Renderable\RenderableObjectsManager.h"
#include "Renderable\RenderableObject.h"
#include "Core\Core.h"


CStaticPlatform::CStaticPlatform(std::string boxName, std::string userDataName, Vect3f size, Vect3f globalPosition, Vect3f localPosition)
{
    //m_RenderableObject = RENDM->GetResource(boxName);
    m_PlatformUserData = new CPhysicUserData(userDataName);
    m_PlatformUserData->SetPaint(true);
    m_PlatorformActor = new CPhysicActor(m_PlatformUserData);
    m_PlatorformActor->AddBoxSphape(size, globalPosition, localPosition);
    m_PlatorformActor->CreateBody(0.5f);
    PHYSXM->AddPhysicActor(m_PlatorformActor);
}

CStaticPlatform::~CStaticPlatform ()
{
    m_PlatorformActor->Destroy();
}

