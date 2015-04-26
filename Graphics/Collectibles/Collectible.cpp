#include "Collectible.h"
#include "Core\ScriptManager.h"
#include "Core\Core.h"
#include "PhysicsManager.h"
#include <sstream>
#include "Utils\PhysicUserData.h"
#include "Actor\PhysicActor.h"
#include "Utils\Defines.h"
#include "TriggerManager\Trigger.h"
#include "TriggerManager\TriggerManager.h"




CCollectible::CCollectible(CRenderableObject *RendObj, std::string layerMame, std::string MeshLuaCode, std::string TriggerLuaCode, std::string param1, std::string param2):
  m_RenderableObject(RendObj),
  m_LuaCode(MeshLuaCode),
// m_TriggerLuaCode(TriggerLuaCode),
  m_LayerName(layerMame) {
  std::stringstream ss;
  ss << RendObj->getName() << "_UserData";
  std::string name = ss.str();
  CPhysicUserData *l_pUserData = new CPhysicUserData(name);
  l_pUserData->SetPaint(false);
  l_pUserData->SetColor(CColor(Vect3f(0.0f, 1.5f, 1.5f)));
  l_pUserData->SetGroup(ECG_TRIGGERS);
//  TRIGGM->GetUserDataVector().push_back( l_pUserData );
  m_Trigger = new CTrigger("box", m_RenderableObject->GetPosition() , Vect3f(0.5f, 0.5f, 0.1f), 0.0f, Vect3f(0.0f, 1.5f, 1.5f), "enter", TriggerLuaCode, param1, param2, l_pUserData);
  std::stringstream ss2;
  ss2 << RendObj->getName() << "_Trigger";
  std::string name2 = ss2.str();
  TRIGGM->AddResource(name2, m_Trigger);
}


CCollectible::~CCollectible() {
}

void CCollectible::Update(float dt) {
  std::stringstream ss;
// ss << m_LuaCode << "(" << m_RenderableObject << "," << dt << ")";
  ss << m_LuaCode << "(\"" << m_RenderableObject->getName() << "\", \"" << m_LayerName << "\",  " <<  dt << ")";
  std::string toRun = ss.str();
  SCRIPTM->RunCode(toRun.c_str());
  //SCRIPTM->RunCode(m_LuaCode);
}

