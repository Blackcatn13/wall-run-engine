#include "Switch.h"
#include "XML\XMLTreeNode.h"

CSwitch::CSwitch(const CXMLTreeNode &node)
  : CSceneElement(node) {
}

CSwitch::CSwitch(std::string switchName, std::string coreName)
  : CSceneElement(switchName, coreName) {
  InsertPhisic(Vect3f(.0f, m_PhysicsSize.y, .0f));
}

CSwitch::~CSwitch() {
}

