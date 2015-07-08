#include "Door.h"
#include "Core\ScriptManager.h"
#include "PhysicsManager.h"
#include "Core\PlayerController.h"
#include "Core\Core.h"
#include "Actor\PhysicController.h"
#include "Utils\PhysicUserData.h"
#include "XML\XMLTreeNode.h"
#include "Actor\PhysicActor.h"


CDoor::CDoor(std::string switchName, std::string coreName, std::string lua_function, Vect3f final_Position)
  : CSceneElement(switchName, coreName),
    m_AnimationDone(false),
    m_IsPlayingAnimation(false),
    m_FinalPosition (final_Position),
    m_IsOpening(false),
    m_Speed(0.0f) {
  m_HasPhisicMesh = false;
  m_Actor->Activate(true);
}

CDoor::CDoor(const CXMLTreeNode &node)
  : CSceneElement(node) ,
    m_AnimationDone(false),
    m_IsPlayingAnimation(false),
    m_FinalPosition(node.GetVect3fProperty("final_position", v3fZERO)),
    m_OriginalPosition(m_Position),
    m_IsOpening(false),
    m_Speed(node.GetFloatProperty("speed", 0.0f, false)),
    m_LuaFunction(node.GetPszISOProperty("lua_function", "", false)) {
  InsertPhisic(v3fZERO);
  m_Actor->Activate(true);

}

CDoor::~CDoor() {
}

void CDoor::Update(float dt) {
  if (m_IsOpening) {
    OpeningDoor(dt);
  }

  //Aqui meter las colisiones con el Player
  if (PLAYC->getis3D() == false) {
    PLAYC->setisOnPlatform(1.0);
    float l_RadioPhysicsPlayer = PLAYC->getPhysicController()->GetRadius();
    Vect3f l_playerPosition = PLAYC->GetPosition();
    Vect3f dirRay = (m_Position - l_playerPosition);
    Vect3f l_dirRayFixed = Vect3f(dirRay.x, 0, dirRay.y);

    l_dirRayFixed = l_dirRayFixed.Normalize();
    //solo hacemos los calculos de colisiones si la plataforma esta a una distancia minima (optimización)
    if (abs(dirRay.x) + abs(dirRay.z) < (GetPhysicsSize().x + GetPhysicsSize().z + 1.0)) {
      bool PhysicsApplied = false;
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

void CDoor::OpeningDoor(float dt) {
  Vect3f direction = (m_FinalPosition - m_Position);
  direction = direction.Normalize();
  if (m_FinalPosition.Distance(m_Position) >= 0.2f) {
    m_Position =  m_Position + /*Vect3f(1, 0, 0)*/direction.Normalize() * m_Speed * dt;
    m_Actor->SetGlobalPosition(m_Position);
  } else {
    m_IsOpening = false;
    m_IsPlayingAnimation = false;
    m_AnimationDone = true;
  }


}

//bool CDoor::isInside(Vect3f vector1, Vect3f vector2) {
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
//bool CDoor::isAround(Vect3f vector1, Vect3f vector2) {
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
