#include "CinematicController.h"
#include "Cinematic.h"
#include "Utils\Logger.h"
#include "Core_Utils/MemLeaks.h"


CCinematicController::CCinematicController()
  : m_FileName("") {
}

CCinematicController::~CCinematicController() {
  Destroy();
}

bool CCinematicController::Load(const std::string &FileName) {
  m_FileName = FileName;
  if (FileName != "No") {
    CCinematic *l_Cinematic = new CCinematic();
    std::string name = l_Cinematic->LoadXML(FileName);
    AddResource(name, l_Cinematic);
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
  TMapResource::iterator it = m_Resources.begin();
  for (it; it != m_Resources.end(); ++it) {
    (* it).second->Update(ElapsedTime);
  }
}