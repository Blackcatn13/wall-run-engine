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
  m_cinematic->Update(dt);
  return m_cinematic->IsPlaying();
}

CCinematicElementCinematic::~CCinematicElementCinematic() {
  delete m_cinematic;
}