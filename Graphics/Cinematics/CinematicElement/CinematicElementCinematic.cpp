#include "CinematicElementCinematic.h"
#include "XML\XMLTreeNode.h"
#include "Core\Core.h"
#include "Cinematics\Cinematic.h"
#include "Core_Utils/MemLeaks.h"


CCinematicElementCinematic::CCinematicElementCinematic(const CXMLTreeNode &node)
  : CCinematicElement(node) {
  m_cinematic = new CCinematic();
  m_cinematic->LoadXML(node.GetPszISOProperty("name", ""));
}

bool CCinematicElementCinematic::Execute() {
  m_cinematic->Play(false);
  return true;
}

bool CCinematicElementCinematic::Update(float dt) {
  bool playing = m_cinematic->IsPlaying();
  if (playing) {
    m_cinematic->Update(dt);
    playing = m_cinematic->IsPlaying();
  }
  return playing;
}

CCinematicElementCinematic::~CCinematicElementCinematic() {
  delete m_cinematic;
}

void CCinematicElementCinematic::Stop() {
  m_cinematic->ToEnd();
}