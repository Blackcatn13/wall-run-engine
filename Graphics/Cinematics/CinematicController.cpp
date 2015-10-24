#include "CinematicController.h"
#include "Cinematic.h"
#include "Utils\Logger.h"
#include "Core_Utils/MemLeaks.h"
#include "XML\XMLTreeNode.h"
#include <string>
#include "Camera\CameraController.h"
#include "Core\Core.h"
#include "CinematicElement\CinematicElementCamera.h"
#include "CinematicElement\CinematicElementCinematic.h"
#include "CinematicElement\CinematicElement.h"


CCinematicController::CCinematicController()
  : m_FileName("")
  , m_lastElement(0)
  , m_executing(false) {
}

CCinematicController::~CCinematicController() {
  for (auto it = m_cinematics.begin(); it != m_cinematics.end(); ++it) {
    for (auto it1 = it->second.begin(); it1 != it->second.end(); ++it1) {
      CHECKED_DELETE(*it1);
    }
    it->second.clear();
  }
  m_cinematics.clear();
}

bool CCinematicController::Load(const std::string &FileName) {
  m_FileName = FileName;
  if (FileName != "No") {
    CXMLTreeNode l_XMLParser;
    if (!l_XMLParser.LoadFile(FileName.c_str())) {
      LOGGER->AddNewLog(ELL_INFORMATION, "No Cinematic File found");
    } else {
      int childs = l_XMLParser.GetNumChildren();
      for (int i = 0; i < childs; ++i) {
        CXMLTreeNode child = l_XMLParser.getNextChild();
        std::string name = child.GetName();
        if (name == "cinematic_controller") {
          std::string cinematic_name = child.GetPszISOProperty("name", "NoName");
          int cinematic_elements = child.GetNumChildren();
          std::vector<CCinematicElement *> aux;
          for (int j = 0; j < cinematic_elements; ++j) {
            CXMLTreeNode nChild = child.getNextChild();
            std::string elementType = nChild.GetName();
            if (elementType == "play_camera") {
              aux.push_back(new CCinematicElementCamera(nChild));
            } else if (elementType == "play_cinematic") {
              aux.push_back(new CCinematicElementCinematic(nChild));
            }
          }
          m_cinematics[cinematic_name] = aux;
        }
      }
    }
    //CCinematic *l_Cinematic = new CCinematic();
    //std::string name = l_Cinematic->LoadXML(FileName);
    //AddResource(name, l_Cinematic);
  } else {
    LOGGER->AddNewLog(ELL_INFORMATION, "No Cinematic File loaded");
  }
  return true;
}

bool CCinematicController::Reload() {
  return Load(m_FileName);
}

bool CCinematicController::Reload(const std::string &FileName) {
  return Load(FileName);
}

void CCinematicController::Update(float ElapsedTime) {
  if (m_executing) {
    bool aux = false;
    for (int i = 0; i < m_currentCinematic.size(); ++i) {
      aux |= m_currentCinematic[i]->Update(ElapsedTime);
    }
    m_executing = aux;
  }
}

void CCinematicController::Execute(const std::string &cinematic) {
  std::map<std::string, std::vector<CCinematicElement *>>::iterator it = m_cinematics.find(cinematic);
  if (it != m_cinematics.end()) {
    m_currentCinematic = it->second;
    m_lastElement = 0;
    m_executing = true;
    for (int i = 0; i < m_currentCinematic.size(); ++i) {
      m_currentCinematic[i]->Execute();
    }
  }
}

void CCinematicController::Restart(const std::string &cinematic) {
  std::map<std::string, std::vector<CCinematicElement *>>::iterator it = m_cinematics.find(cinematic);
  if (it != m_cinematics.end()) {
    for (int i = 0; i < it->second.size(); ++i) {
      it->second[i]->restart();
    }
  }
}

void CCinematicController::RestartAllCinematics() {
  std::map<std::string, std::vector<CCinematicElement *>>::iterator it = m_cinematics.begin();
  while (it != m_cinematics.end()) {
    for (int i = 0; i < it->second.size(); ++i) {
      it->second[i]->restart();
    }
    ++it;
  }
}