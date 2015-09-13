#include "Switch.h"
#include "XML\XMLTreeNode.h"
#include "Actor\PhysicActor.h"

CSwitch::CSwitch(const CXMLTreeNode &node)
  : CSceneElement(node) {
  InsertPhisic(Vect3f(.0f, m_PhysicsSize.y, .0f));
  m_OriginalPosition = m_Position;
}

CSwitch::CSwitch(std::string switchName, std::string coreName)
  : CSceneElement(switchName, coreName, false) {
  InsertPhisic(Vect3f(.0f, m_PhysicsSize.y, .0f));
  m_OriginalPosition = m_Position;
}

void CSwitch::ResetSwitch() {
  if (m_Position != m_OriginalPosition) {
    SetPosition(m_OriginalPosition);
    m_Actor->SetGlobalPosition(m_OriginalPosition);
  }
}

CSwitch::~CSwitch() {
}

