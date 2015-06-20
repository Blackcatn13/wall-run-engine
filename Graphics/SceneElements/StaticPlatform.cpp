#include "StaticPlatform.h"
#include "PhysicsManager.h"
#include "Core\PlayerController.h"
#include "Core\Core.h"
#include "Actor\PhysicController.h"
#include "Utils\PhysicUserData.h"
#include "XML\XMLTreeNode.h"

CStaticPlatform::CStaticPlatform(std::string platformName, std::string coreName)
  : CSceneElement(platformName, coreName) {
  InsertPhisic(Vect3f(.0f, m_PhysicsSize.y, .0f));
}

CStaticPlatform::CStaticPlatform(const CXMLTreeNode &node)
  : CSceneElement(node) {
  InsertPhisic(Vect3f(.0f, m_PhysicsSize.y, .0f));
}

CStaticPlatform::~CStaticPlatform() {
}

void CStaticPlatform::Update(float dt) {
  //Aqui meter las colisiones con el Player
  if (PLAYC->getis3D() == false) {
    std::string lala = m_Name;
    PLAYC->setisOnPlatform(1.0);
    float l_RadioPhysicsPlayer = PLAYC->getPhysicController()->GetRadius();
    Vect3f l_playerPosition = PLAYC->GetPosition();
    Vect3f dirRay = (m_Position - l_playerPosition);
    Vect3f l_dirRayFixed = Vect3f(dirRay.x, 0, dirRay.y);
    bool PhysicsApplied = false;
    l_dirRayFixed = l_dirRayFixed.Normalize();
    //solo hacemos los calculos de colisiones si la plataforma esta a una distancia minima (optimización)
    if (abs(dirRay.x) + abs(dirRay.z) < (GetPhysicsSize().x + GetPhysicsSize().z + 1.0)) {
      SCollisionInfo info = SCollisionInfo();


      l_dirRayFixed = l_dirRayFixed.Normalize() * 0.5;
      Vect3f dirRayBounding = Vect3f(l_dirRayFixed.x, -l_RadioPhysicsPlayer, l_dirRayFixed.z);
      //Vect3f dirRayBounding = dirRay * l_RadioPhysicsPlayer;
      CPhysicUserData *hit = CCORE->GetPhysicsManager()->RaycastClosestActor(Vect3f(l_playerPosition.x + dirRayBounding.x, l_playerPosition.y + dirRayBounding.y, l_playerPosition.z + dirRayBounding.z),
                             Vect3f(0, -1, 0), 0xffffffff, info);
      if (hit != NULL && hit->getName() == m_Name && info.m_fDistance <= 1.2) {
        PLAYC->getPhysicController()->Move(-l_dirRayFixed.Normalize() * PLAYC->getSpeed() * dt, dt);
        PhysicsApplied = true;
      }

      if ((PhysicsApplied == false) && (isAround(l_playerPosition, m_Position))) {

        Vect3f dirInvertida = -dirRay.Normalize();
        dirInvertida = Vect3f(dirInvertida.x, 0, dirInvertida.z);
        PLAYC->getPhysicController()->Move(dirInvertida *  PLAYC->getSpeed() * dt , dt);

      }

    }
  }
}

//bool CStaticPlatform::isInside(Vect3f vector1, Vect3f vector2) {
//  //FIX BUG DETECTA PLAYER INSIDE
//  float l_margenx = GetPhysicsSize().x + 0.0f;
//  float l_margenz = GetPhysicsSize().z + 0.0f;
//  float l_margeny = GetPhysicsSize().y + 0.0f;
//  if ((vector1.x > vector2.x - l_margenx) && (vector1.x < vector2.x + l_margenx) && (vector1.y > vector2.y - l_margeny) && (vector1.y < vector2.y + l_margeny) && (vector1.z > vector2.z - l_margenz) && (vector1.z < vector2.z + l_margenz))
//    return true;
//  else
//    return false;
//}
//
//bool CStaticPlatform::isAround(Vect3f vector1, Vect3f vector2) {
//  float l_margenx = GetPhysicsSize().x + 0.8f;
//  float l_margenz = GetPhysicsSize().z + 0.8f;
//  float l_margeny = GetPhysicsSize().y + 1.8f;
//  /*bool l_isInside = false;
//  l_isInside = isInside(vector1, vector2);*/
//  if ((vector1.x > vector2.x - l_margenx) && (vector1.x < vector2.x + l_margenx) && (vector1.y > vector2.y - l_margeny) && (vector1.y < vector2.y + l_margeny) && (vector1.z > vector2.z - l_margenz) && (vector1.z < vector2.z + l_margenz))
//    return true;
//  else
//    return false;
//}
/*
void CStaticPlatform::InsertPlatform(std::string userDataName, Vect3f size, Vect3f localPosition) {
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
} */
