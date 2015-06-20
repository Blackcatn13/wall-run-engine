#include "SceneElement.h"
#include "Actor\PhysicActor.h"
#include "Utils\PhysicUserData.h"
#include "PhysicsManager.h"
#include "Renderable\RenderableObjectsManager.h"
#include "Renderable\RenderableObject.h"
#include "Renderable\RenderableObjectsLayersManager.h"
#include "Core\Core.h"
#include "Utils\Defines.h"
#include "XML\XMLTreeNode.h"

std::string SetUserDataName(std::string name) {
  std::stringstream ss;
  ss << name << "_UserData";
  return ss.str();
}

CSceneElement::CSceneElement(std::string switchName, std::string coreName)
  : CMeshInstance(switchName, coreName)
  , m_Actor(NULL)
  , m_UserData(NULL),
    m_Room("0") {
}

CSceneElement::CSceneElement(const CXMLTreeNode &node)
  : CMeshInstance(node),
    m_Actor(NULL),
    m_UserData(NULL),
    m_Room(node.GetPszISOProperty("room", "0")),
    m_PhysicsSize(node.GetVect3fProperty("phisic_size", v3fZERO)) {
}

CSceneElement::~CSceneElement () {
  if (m_Actor != NULL) {
    PHYSXM->ReleasePhysicActor(m_Actor);
    CHECKED_DELETE(m_Actor);
  }
  if (m_UserData != NULL)
    CHECKED_DELETE(m_UserData);
}

void CSceneElement::ActivatePhisic(bool active) {
  m_Actor->Activate(active);
}

void CSceneElement::InsertPhisic(Vect3f localPosition) {
  m_UserData = new CPhysicUserData(SetUserDataName(m_Name));
  m_UserData->SetPaint(false);
  m_Actor = new CPhysicActor(m_UserData);
  m_Actor->AddBoxSphape(m_PhysicsSize, m_Position, localPosition);
  PHYSXM->AddPhysicActor(m_Actor);
}

bool CSceneElement::isInside(Vect3f vector1, Vect3f vector2) {
  //FIX BUG DETECTA PLAYER INSIDE
  float l_margenx = GetPhysicsSize().x + 0.0f;
  float l_margenz = GetPhysicsSize().z + 0.0f;
  float l_margeny = GetPhysicsSize().y + 0.0f;
  if ((vector1.x > vector2.x - l_margenx) && (vector1.x < vector2.x + l_margenx) && (vector1.y > vector2.y - l_margeny) && (vector1.y < vector2.y + l_margeny) && (vector1.z > vector2.z - l_margenz) && (vector1.z < vector2.z + l_margenz))
    return true;
  else
    return false;
}

bool CSceneElement::isAround(Vect3f vector1, Vect3f vector2) {
  float l_margenx = GetPhysicsSize().x + 0.8f;
  float l_margenz = GetPhysicsSize().z + 0.8f;
  float l_margeny = GetPhysicsSize().y + 1.8f;
  /*bool l_isInside = false;
  l_isInside = isInside(vector1, vector2);*/
  if ((vector1.x > vector2.x - l_margenx) && (vector1.x < vector2.x + l_margenx) && (vector1.y > vector2.y - l_margeny) && (vector1.y < vector2.y + l_margeny) && (vector1.z > vector2.z - l_margenz) && (vector1.z < vector2.z + l_margenz))
    return true;
  else
    return false;
}