#include "RenderableObject.h"
#include "Core_Utils/MemLeaks.h"
#include "XML\XMLTreeNode.h"

CRenderableObject::CRenderableObject(const CXMLTreeNode &node)
  : CObject3D(node),
    CNamed(node),
    m_Printable(m_visible),
    m_ParticleEmitter(node.GetPszISOProperty("emitter", "")) {
}

CRenderableObject::CRenderableObject() {
  m_Printable = true;
}


