#include "AI\PuzzleManager.h"
#include "XML\XMLTreeNode.h"
#include "Core\Core.h"
#include "Puzzle.h"
#include "Utils\Defines.h"
#include "TriggerManager\TriggerManager.h"
#include "TriggerManager\Trigger.h"


CPuzzleManager::CPuzzleManager()
  : m_fileName ("") {
}

CPuzzleManager::~CPuzzleManager() {
  Destroy();
}


void CPuzzleManager::Reload() {
  Destroy();
  Load(m_fileName);
}

void CPuzzleManager::Load(std::string file) {
  m_fileName = file;
  CXMLTreeNode newFile;
  if (!newFile.LoadFile(m_fileName.c_str())) {
    printf("ERROR loading the file.");
  } else {
    CXMLTreeNode  m = newFile["puzzles"];
    if (m.Exists()) {
      int count = m.GetNumChildren();
      for (int i = 0; i < count; ++i) {
        CXMLTreeNode nodeChild = m.getNextChild();
        std::string name = nodeChild.GetName();
        if (name == "puzzle") {
          std::string puzzleName = nodeChild.GetPszISOProperty("name", "", false);
          CPuzzle *newPuzzle = new CPuzzle();
          std::string l_LuaCode = nodeChild.GetPszISOProperty("lua_function", "", false);
          int l_NumActivatedSwitches = nodeChild.GetIntProperty("min_activated_switches", 0, false);
          newPuzzle->setLuaCode(l_LuaCode);
          newPuzzle->setMinActivatedSwitches(l_NumActivatedSwitches);
          newPuzzle->setSceneElement(nodeChild.GetPszISOProperty("scene_element", "", false));
          newPuzzle->setSceneElement2(nodeChild.GetPszISOProperty("scene_element2", "", false));
          int count1 = nodeChild.GetNumChildren();
          for (int j = 0; j < count1; ++j) {
            CTrigger *l_Trigger = TRIGGM->GetResource(nodeChild.getNextChild().GetPszISOProperty("name", "", false));
            if (l_Trigger != NULL)
              newPuzzle->getSwitches().push_back(l_Trigger);
          }
          AddResource(puzzleName, newPuzzle);
        }
      }
    }
  }
}

void CPuzzleManager::Update(float dt) {
  for (TMapResource::iterator it = m_Resources.begin(); it != m_Resources.end(); ++it) {
    it->second->Update(dt);
  }
}

void CPuzzleManager::ResetPuzzleTriggers(std::string puzzleName) {
  TMapResource::iterator it = m_Resources.find(puzzleName);
  int l_NumSwitches = it->second->getSwitches().size();
  for (int i = 0; i < l_NumSwitches; ++i) {
    it->second->getSwitches()[i]->setIsSwitched(false);
  }
  it->second->setActivatedSwitches(0);
  it->second->setDone(false);
}
