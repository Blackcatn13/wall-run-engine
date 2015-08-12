#include "CinematicElement.h"
#include "Core_Utils/MemLeaks.h"
#include "XML\XMLTreeNode.h"

CCinematicElement::CCinematicElement(const CXMLTreeNode &node)
  : m_onStart(node.GetBoolProperty("oninit")) ,
    m_block(node.GetBoolProperty("block", true)) ,
    m_ended(false) {
}

CCinematicElement::~CCinematicElement() {
}