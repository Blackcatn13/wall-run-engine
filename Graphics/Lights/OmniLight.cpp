#include "OmniLight.h"
#include "Core_Utils/MemLeaks.h"

COmniLight::COmniLight(CXMLTreeNode &Node)
  : CLight(Node) {
  m_Type = TLightType::OMNI;
}



