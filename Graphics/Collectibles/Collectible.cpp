#include "Collectible.h"
#include "Core\ScriptManager.h"
#include "Core\Core.h"
#include "PhysicsManager.h"
#include <sstream>
#include "Utils\PhysicUserData.h"
#include "Actor\PhysicActor.h"
#include "Utils\Defines.h"

CCollectible::CCollectible(std::string coreName, std::string name, std::string layerName,  std::string userDataName, std::string luaCode):
  CMeshInstance(name, coreName),
  m_LayerName(layerName),
  m_LuaCode(luaCode),
  m_UserDataName(userDataName) {
}



CCollectible::~CCollectible() {
  CHECKED_DELETE(m_CollectibleActor);
  CHECKED_DELETE(m_CollectibleUserData);
}

void CCollectible::Update(float dt) {
  std::stringstream ss;

  ss << m_LuaCode << "(\"" << m_Name << "\", \"" << m_LayerName << "\",  " <<  dt << ")";
  std::string toRun = ss.str();
  SCRIPTM->RunCode(toRun.c_str());
  //SCRIPTM->RunCode(m_LuaCode);
}

void CCollectible::SetActor() {
  m_CollectibleUserData = new CPhysicUserData(m_UserDataName);
  m_CollectibleUserData->SetPaint(false);
  m_CollectibleActor = new CPhysicActor(m_CollectibleUserData);
  m_CollectibleActor->AddBoxSphape(Vect3f(0.5f, 0.5f, 0.1f), m_Position, Vect3f(0.0f, 0.0f, 0.0f));
  //m_RenderableObject->SetYaw(m_fYaw);
  // m_PlatorformActor->CreateBody(0.5f);
  PHYSXM->AddPhysicActor(m_CollectibleActor);
}