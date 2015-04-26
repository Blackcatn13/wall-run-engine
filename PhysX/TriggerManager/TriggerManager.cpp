#include "TriggerManager\TriggerManager.h"
#include "PhysicsManager.h"
#include "XML\XMLTreeNode.h"
#include "Utils\Defines.h"
#include "Utils\PhysicUserData.h"
#include "Core\Core.h"

CTriggerManager::CTriggerManager()
  : CPhysicTriggerReport() {
}

CTriggerManager::~CTriggerManager() {
  Clear();
}

void CTriggerManager::Clear() {
  Destroy();
//  for (size_t i = 0; i < m_UserData.size(); ++i) {
////    PHYSXM->ReleasePhysicActor(m_UserData[i]->GetActor());
//    CHECKED_DELETE(m_UserData[i]);
//  }
//  m_UserData.clear();


}

void	CTriggerManager::OnEnter		( CPhysicUserData *_Entity_Trigger1, CPhysicUserData *_Other_Shape) {
  CTrigger *trigger = GetResource(_Entity_Trigger1->getName());
  if (trigger != NULL)
    trigger->ExecuteOnEnter();
}
void	CTriggerManager::OnLeave		( CPhysicUserData *_Entity_Trigger1, CPhysicUserData *_Other_Shape) {
  CTrigger *trigger = GetResource(_Entity_Trigger1->getName());
  if (trigger != NULL)
    trigger->ExecuteOnExit();
}
void	CTriggerManager::OnStay		( CPhysicUserData *_Entity_Trigger1, CPhysicUserData *_Other_Shape) {
  CTrigger *trigger = GetResource(_Entity_Trigger1->getName());
  if (trigger != NULL)
    trigger->ExecuteOnStay();
}

bool	CTriggerManager::LoadTriggers( std::string FileName) {
  m_FileName = FileName;
  CXMLTreeNode newFile;
  if (!newFile.LoadFile(FileName.c_str())) {
    printf("ERROR loading the file.");
  } else {
    CXMLTreeNode  m = newFile["triggers"];
    if (m.Exists()) {
      int count = m.GetNumChildren();
      for (int i = 0; i < count; ++i) {
        //Creando el UserData para el trigger
        std::string name = m(i).GetPszProperty("name", "", false);
        CPhysicUserData *l_pUserData = new CPhysicUserData(name);
        l_pUserData->SetPaint(m(i).GetBoolProperty("paint", true, false));
        CColor col(m(i).GetVect3fProperty("color", v3fZERO, false));
        l_pUserData->SetColor(colRED);
        l_pUserData->SetGroup(ECG_TRIGGERS);
        //  m_UserData.push_back( l_pUserData );
        CTrigger *trigger = new CTrigger(m(i), l_pUserData);
        if (!AddResource(name, trigger)) {
          LOGGER->AddNewLog(ELL_WARNING, "El CTrigger ya existía ", name.c_str());
          CHECKED_DELETE(trigger);
        }
      }
      return true;
    }
  }
  return false;
}

bool CTriggerManager::Reload() {
  Clear();
  return LoadTriggers(m_FileName);
}

bool CTriggerManager::Reload(const std::string &FileName) {
  Clear();
  return LoadTriggers(FileName);
}

void CTriggerManager::Update(float dt) {
  for (int i = 0; i < GetResourcesVector().size(); ++i) {
    if (GetResourcesVector()[i]->getUpdate())
      GetResourcesVector()[i]->Update(dt);
  }
}