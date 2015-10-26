#include "CinematicElementMusic.h"
#include "XML\XMLTreeNode.h"
#include "Core\Core.h"
#include "Cinematics\Cinematic.h"
#include "WWSoundManager.h"
#include "Core_Utils/MemLeaks.h"


CCinematicElementMusic::CCinematicElementMusic(const CXMLTreeNode &node)
  : CCinematicElement(node),
    m_Event(node.GetPszISOProperty("name", "")),
    m_Actor(node.GetPszISOProperty("actor", "")) {
}

bool CCinematicElementMusic::Execute() {
  WWSNDMGR->PlayEvent(m_Event, m_Actor);
  return true;
}

bool CCinematicElementMusic::Update(float dt) {
  return false;
}

CCinematicElementMusic::~CCinematicElementMusic() {
}