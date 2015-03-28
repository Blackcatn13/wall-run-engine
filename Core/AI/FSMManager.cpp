#include "AI\FSMManager.h"
#include "XML\XMLTreeNode.h"
#include "Core\Core.h"
#include "Core\ScriptManager.h"
#include "Utils\LuaGlobals.h"
#include "Utils\Defines.h"

CFSMManager::CFSMManager()
  : m_fileName ("") {
  m_LuaGlobals = CLuaGlobals::getInstance();
}

CFSMManager::~CFSMManager() {
  CMapManager<FSM>::TMapResource::iterator it = GetResources().begin();
  while (it != GetResources().end() ) {
    it->second->m_States.Destroy();
    ++it;
  }
  Destroy();
  CHECKED_DELETE(m_LuaGlobals);
}

void CFSMManager::Load(std::string file) {
  m_fileName = file;
  Load();
}

void CFSMManager::Reload() {
  Destroy();
  Load();
}

void CFSMManager::Load() {
  CXMLTreeNode newFile;
  if (!newFile.LoadFile(m_fileName.c_str())) {
    printf("ERROR loading the file.");
  } else {
    CXMLTreeNode  m = newFile["fsms"];
    if (m.Exists()) {
      int count = m.GetNumChildren();
      for (int i = 0; i < count; ++i) {
        std::string name = m(i).GetName();
        if (name == "fsm") {
          FSM *newFSM = new FSM();
          int childCount = m(i).GetNumChildren();
          std::string FSMName = m(i).GetPszISOProperty("name", "", false);
          for (int j = 0; j < childCount; ++j) {
            std::string l_PropName = m(i)(j).GetName();
            if (l_PropName == "initialState") {
              newFSM->m_currentState = m(i)(j).GetPszISOProperty("name", "", false);
            } else if (l_PropName == "state") {
              STATE *s = new STATE();
              int states = m(i)(j).GetNumChildren();
              std::string StateName = m(i)(j).GetPszISOProperty("name", "", false);
              for (int k = 0; k < states; ++k) {
                std::string StateType = m(i)(j)(k).GetName();
                if (StateType == "onEnter") {
                  s->onEnter = m(i)(j)(k).GetPszISOProperty("lua_funtion", "", false);
                } else if (StateType == "onExit") {
                  s->onExit = m(i)(j)(k).GetPszISOProperty("lua_funtion", "", false);
                } else if (StateType == "Update") {
                  s->onUpdate = m(i)(j)(k).GetPszISOProperty("lua_funtion", "", false);
                  s->m_UpdateTime = m(i)(j)(k).GetFloatProperty("time");
                }
                s->m_ElapsedTime = 0;
                s->m_onEnter = false;
              }
              newFSM->m_States.AddResource(StateName, s);
            }
          }
          AddResource(FSMName, newFSM);
        }
      }
    }
  }
}

void CFSMManager::Update(float dt) {
  /*for (TMapResource::iterator it = m_Resources.begin(); it != m_Resources.end(); ++it) {
    STATE *s = it->second->m_States.GetResource(it->second->m_currentState);
    if (s->m_onEnter == false) {
      SCRIPTM->RunCode(s->onEnter.c_str());
      s->m_onEnter = true;
    }
    s->m_ElapsedTime += dt;
    char l_InitLevelText[256];
    int doComprobation = 0;
    if (s->m_ElapsedTime >= s->m_UpdateTime) {
      doComprobation = 1;
    }
    _snprintf_s(l_InitLevelText, 256, 256, "%s(%f,%d)", s->onUpdate.c_str(), dt, doComprobation);
    SCRIPTM->RunCode(l_InitLevelText);
    if (doComprobation == 1) {
      s->m_ElapsedTime = 0;
      doComprobation = 0;
    }
    bool change = CLuaGlobals::getInstance()->ValueChanged();
    if (change) {
      s->m_onEnter = false;
      SCRIPTM->RunCode(s->onExit.c_str());
      it->second->m_previousState = it->second->m_currentState;
      it->second->m_currentState = CLuaGlobals::getInstance()->getString();
    }
  }*/
}