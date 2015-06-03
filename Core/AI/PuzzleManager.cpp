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
        CXMLTreeNode nodeChild = m(i);
        std::string name = nodeChild.GetName();
        if (name == "puzzle") {
          std::string puzzleName = nodeChild.GetPszISOProperty("name", "", false);
          CPuzzle *newPuzzle = new CPuzzle();
          std::string l_LuaCode = nodeChild.GetPszISOProperty("lua_function", "", false);
          int l_NumActivatedSwitches = nodeChild.GetIntProperty("min_activated_switches", 0, false);
          newPuzzle->setLuaCode(l_LuaCode);
          newPuzzle->setMinActivatedSwitches(l_NumActivatedSwitches);
          newPuzzle->setSceneElement(nodeChild.GetPszISOProperty("scene_element", "", false));
          for (int j = 0; j < nodeChild.GetNumChildren(); ++j) {
            CTrigger *l_Trigger = TRIGGM->GetResource(nodeChild(j).GetPszISOProperty("name", "", false));
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