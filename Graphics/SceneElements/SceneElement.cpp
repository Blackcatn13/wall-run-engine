#include "SceneElement.h"
#include "Actor\PhysicActor.h"
#include "Utils\PhysicUserData.h"
#include "PhysicsManager.h"
#include "Renderable\RenderableObjectsManager.h"
#include "Renderable\RenderableObject.h"
#include "Renderable\RenderableObjectsLayersManager.h"
#include "Core\Core.h"
#include "Utils\Defines.h"


CSceneElement::CSceneElement(std::string switchName, std::string coreName)
  : CMeshInstance(switchName, coreName)
  , m_Actor(NULL)
  , m_UserData(NULL),
    m_Room("0") {
}

CSceneElement::~CSceneElement () {
  PHYSXM->ReleasePhysicActor(m_Actor);
  CHECKED_DELETE(m_Actor);
  CHECKED_DELETE(m_UserData);
}


void CSceneElement::InsertPhisic(std::string userDataName, Vect3f size, Vect3f localPosition) {
  // m_RenderableObject = RENDLM->GetDefaultRenderableObjectManager()->GetResource(boxName);
  m_UserData = new CPhysicUserData(userDataName);
  m_UserData->SetPaint(true);
  m_Actor = new CPhysicActor(m_UserData);
  m_PhysicsSize = Vect3f(size.x, size.y, size.z);
  // m_PlatorformActor->AddBoxSphape(size, m_RenderableObject->GetPosition(), localPosition);
  m_Actor->AddBoxSphape(size, m_Position, localPosition);
  //m_RenderableObject->SetYaw(m_fYaw);
  // m_PlatorformActor->CreateBody(0.5f);
  PHYSXM->AddPhysicActor(m_Actor);
  //m_RenderableObject->SetPosition(m_RenderableObject->GetPosition());
}