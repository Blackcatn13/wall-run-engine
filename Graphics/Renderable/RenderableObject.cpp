#include "RenderableObject.h"
#include "Core_Utils/MemLeaks.h"
#include "XML\XMLTreeNode.h"

CRenderableObject::CRenderableObject(const CXMLTreeNode &node)
  : CObject3D(node),
    CNamed(node),
    m_Printable(m_visible),
    m_ParticleEmitter(node.GetPszISOProperty("emitter", "")),
    m_ParticleEmitter2(node.GetPszISOProperty("emitter2", "")),
    m_ParticleEmitter3(node.GetPszISOProperty("emitter3", "")),
    m_ParticleEmitter4(node.GetPszISOProperty("emitter4", "")),
    m_Room(node.GetIntProperty("room", 0)) {
}

CRenderableObject::CRenderableObject() {
  m_Printable = true;
}


