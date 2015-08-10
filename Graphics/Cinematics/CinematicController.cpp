#include "CinematicController.h"
#include "Cinematic.h"
#include "Utils\Logger.h"
#include "Core_Utils/MemLeaks.h"
#include "XML\XMLTreeNode.h"
#include <string>
#include "Camera\CameraController.h"
#include "Core\Core.h"


CCinematicController::CCinematicController()
  : m_FileName("")
  , start(true) {
}

CCinematicController::~CCinematicController() {
  Destroy();
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
  Destroy();
  return Load(m_FileName);
}

bool CCinematicController::Reload(const std::string &FileName) {
  Destroy();
  return Load(FileName);
}

void CCinematicController::Update(float ElapsedTime) {
  /*TMapResource::iterator it = m_Resources.begin();
  for (it; it != m_Resources.end(); ++it) {
    (* it).second->Update(ElapsedTime);
  }*/
  if (start) {
    CAMCONTM->setActiveCamera("Camera056");
    CAMCONTM->Play(false);
    start = false;
  }
  if (!CAMCONTM->IsAnyCinematicPlaying()) {
    CAMCONTM->setActiveCamera("3DCam");
  }
}