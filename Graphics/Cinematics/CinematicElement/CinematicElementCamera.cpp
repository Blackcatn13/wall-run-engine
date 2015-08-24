#include "CinematicElementCamera.h"
#include "XML\XMLTreeNode.h"
#include "Core\Core.h"
#include "Camera\CameraController.h"
#include "Core_Utils/MemLeaks.h"
#include "Camera\Importer\CameraKeyController.h"

CCinematicElementCamera::CCinematicElementCamera(const CXMLTreeNode &node)
  : CCinematicElement(node),
    m_cameraName(node.GetPszISOProperty("name", "NoCamera")),
    m_returnCamera(node.GetPszISOProperty("return", "NoCamera")),
    m_played(false) {
  if (m_cameraName == "NoCamera") {
    m_ended = true;
  }
}

bool CCinematicElementCamera::Execute() {
  if (!m_ended) {
    if (m_played && !CAMCONTM->IsAnyCinematicPlaying()) {
      CAMCONTM->Stop();
      CAMCONTM->setActiveCamera(m_returnCamera);
      m_ended = true;
      m_block = false;
    } else {
      CAMCONTM->setActiveCamera(m_cameraName);
      CAMCONTM->Play(m_cameraName, true);
      m_played = true;
    }
  }
  return m_block;
}

CCinematicElementCamera::~CCinematicElementCamera() {
}

void CCinematicElementCamera::restart() {
  m_played = false;
  m_block = true;
  m_ended = false;
  ((CCameraKeyController *)CAMCONTM->GetResource(m_cameraName))->ResetTime(false);
}