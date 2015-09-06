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
    m_Billboard(node.GetPszISOProperty("billboard", "")),
    m_EmitterOffset(node.GetVect3fProperty("emitter_offset", v3fZERO)),
    m_EmitterOffset2(node.GetVect3fProperty("emitter2_offset", v3fZERO)),
    m_EmitterOffset3(node.GetVect3fProperty("emitter3_offset", v3fZERO)),
    m_EmitterOffset4(node.GetVect3fProperty("emitter4_offset", v3fZERO)),
    m_BillboardOffset(node.GetVect3fProperty("billboard_offset", v3fZERO)),
    m_Room(node.GetIntProperty("room", 0)),
    m_Vanishing(true),
    m_VanishActive(false),
    m_Modifier(1.0f) {
}

CRenderableObject::CRenderableObject() {
  m_Printable = true;
}


