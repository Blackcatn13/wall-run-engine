#include "Switch.h"
#include "XML\XMLTreeNode.h"

CSwitch::CSwitch(const CXMLTreeNode &node)
  : CSceneElement(node) {
  InsertPhisic(Vect3f(.0f, m_PhysicsSize.y, .0f));
}

CSwitch::CSwitch(std::string switchName, std::string coreName)
  : CSceneElement(switchName, coreName, false) {
  InsertPhisic(Vect3f(.0f, m_PhysicsSize.y, .0f));
}

CSwitch::~CSwitch() {
}

