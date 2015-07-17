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
#include "Utils\LuaGlobals.h"
#include "Core\ScriptManager.h"
#include "Cooking Mesh\PhysicCookingMesh.h"

std::string CSceneElement::SetUserDataName(std::string name) {
  std::stringstream ss;
  ss << name << "_UserData";
  return ss.str();
}

CSceneElement::CSceneElement(std::string switchName, std::string coreName, bool hasRigidBody, bool hasPhisicMesh)
  : CMeshInstance(switchName, coreName)
  , m_Actor(NULL)
  , m_UserData(NULL),
    m_UserDataAux(NULL),
    m_ActorAux(NULL),
    m_Room(0),
    m_Fsm(NULL),
    m_HasRigidBody(hasRigidBody),
    m_HasPhisicMesh(hasPhisicMesh) {
}

CSceneElement::CSceneElement(const CXMLTreeNode &node, bool hasRigidBody)
  : CMeshInstance(node),
    m_Actor(NULL),
    m_ActorAux(NULL),
    m_UserData(NULL),
    m_UserDataAux(NULL),
    m_Fsm(NULL),
    m_Room(node.GetIntProperty("room", 0)),
    m_PhysicsSize(node.GetVect3fProperty("phisic_size", v3fZERO)),
    m_HasRigidBody(hasRigidBody),
    m_HasPhisicMesh( node.GetBoolProperty("use_physx_mesh", false)) {
}

CSceneElement::~CSceneElement () {
  if (m_Actor != NULL) {
    PHYSXM->ReleasePhysicActor(m_Actor);
    CHECKED_DELETE(m_Actor);
  }
  if (m_UserData != NULL)
    CHECKED_DELETE(m_UserData);
  if (m_ActorAux != NULL) {
    PHYSXM->ReleasePhysicActor(m_ActorAux);
    CHECKED_DELETE(m_ActorAux);
  }
  if (m_UserDataAux != NULL)
    CHECKED_DELETE(m_UserDataAux);
  if (m_Fsm != NULL)
    CHECKED_DELETE(m_Fsm);
}

void CSceneElement::ActivatePhisic(bool active) {
  m_Actor->Activate(active);
}

void CSceneElement::InsertPhisic(Vect3f localPosition) {
  m_UserData = new CPhysicUserData(SetUserDataName(m_Name));
  m_UserData->SetPaint(false);
  m_Actor = new CPhysicActor(m_UserData);
  if (m_HasPhisicMesh) {
    if (PHYSXM->GetSceneCookingMesh()->GetPhysicMeshMap().size() == 0)
      PHYSXM->GetSceneCookingMesh()->LoadFromXML(CCORE->getScenePhisicsFile());
    m_Actor->AddMeshFromMap( PHYSXM->GetSceneCookingMesh()->GetPhysicMeshMap(), m_Name);

  } else
    m_Actor->AddBoxSphape(m_PhysicsSize, m_Position, localPosition);
  if (m_HasRigidBody) {
    std::stringstream name;
    name << m_Name << "_Aux";
    std::string nameAux = name.str();
    m_UserDataAux = new CPhysicUserData(SetUserDataName(nameAux));
    m_UserDataAux->SetPaint(false);
    m_ActorAux = new CPhysicActor(m_UserDataAux);
    if (m_HasPhisicMesh) {
      Vect3f newSize = Vect3f(m_PhysicsSize.x - 1, m_PhysicsSize.y - 1, m_PhysicsSize.z - 1);
      m_ActorAux->AddBoxSphape(newSize, Vect3f(m_Position.x, m_Position.y - m_PhysicsSize.y, m_Position.z), localPosition);
    } else
      m_ActorAux->AddBoxSphape(m_PhysicsSize, Vect3f(m_Position.x, m_Position.y - m_PhysicsSize.y, m_Position.z), localPosition);
    PHYSXM->AddPhysicActor(m_ActorAux);
    m_Actor->CreateBody(0.5);
  }
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

void CSceneElement::UpdateFSM(float elapsedTime) {
// for (TMapResource::iterator it = m_Resources.begin(); it != m_Resources.end(); ++it) {
  STATE *s = m_Fsm->getStates().GetResource(m_Fsm->getCurrentState());
  char l_InitLevelText[256];
  if (m_Fsm->getonEnter() == false) {
    _snprintf_s(l_InitLevelText, 256, 256, "%s(%i,\"%s\")", s->onEnter.c_str(), m_Room, getName().c_str());

    SCRIPTM->RunCode(l_InitLevelText);
    m_Fsm->setonEnter(true);
  }
  s->m_ElapsedTime += elapsedTime;
// char l_InitLevelText[256];
  int doComprobation = 0;
  if (s->m_ElapsedTime >= s->m_UpdateTime) {
    doComprobation = 1;
  }
  _snprintf_s(l_InitLevelText, 256, 256, "%s(%f,%i,\"%s\")", s->onUpdate.c_str(), elapsedTime, m_Room, getName().c_str());
  SCRIPTM->RunCode(l_InitLevelText);
  if (doComprobation == 1) {
    s->m_ElapsedTime = 0;
    doComprobation = 0;
  }
  bool change =  m_Fsm->getChanged();
  if (change) {
    s->m_onEnter = false;
    SCRIPTM->RunCode(s->onExit.c_str());
    m_Fsm->setPreviousState( m_Fsm->getCurrentState());
    m_Fsm->setCurrentState( m_Fsm->getNewState());
  }
}