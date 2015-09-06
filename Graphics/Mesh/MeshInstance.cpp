#include "MeshInstance.h"
#include "Utils\Defines.h"
#include "Core/Core.h"
#include "XML\XMLTreeNode.h"
#include "StaticMeshes\StaticMesh.h"
#include "StaticMeshes\StaticMeshManager.h"
#include "Core_Utils/MemLeaks.h"
#include "GraphicsManager.h"


CMeshInstance::CMeshInstance(const std::string &Name, const std::string &CoreName) :
  m_CoreName(CoreName) {
  m_StaticMesh = SMESHM->GetResource(CoreName);
  setName(Name);
}

CMeshInstance::CMeshInstance(const CXMLTreeNode &Node) :
  CRenderableObject(Node),
  m_CoreName(Node.GetPszISOProperty("core", "")) {
  m_StaticMesh = SMESHM->GetResource(m_CoreName);
}

void CMeshInstance::Update(float dt) {
  if (m_visible) {
    if (GRAPHM->isSphereVisible(m_Position, m_StaticMesh->GetBoundingSphere().GetRadius()))
      m_Printable = true;
    else if (GRAPHM->isBoxVisible(m_StaticMesh->GetBoundingBox().GetMinPos(), m_StaticMesh->GetBoundingBox().GetMaxPos()))
      m_Printable = true;
    else
      m_Printable = false;
  }
// if (m_VanishActive) {
  /* if (m_Vanishing) {
     m_Modifier -= 1 * dt;
     if (m_Modifier <= 0)
       m_Vanishing = false;
   } else {
     m_Modifier += 1 * dt;
     if (m_Modifier >= 1)
       m_Vanishing = true;
   }*/
  //}
}

CMeshInstance::~CMeshInstance() {
  if (m_StaticMesh->isDestroyed())
    m_StaticMesh = 0;
}

void CMeshInstance::Render(CGraphicsManager *RM) {
  if (m_Printable) {
    Mat44f t = m44fIDENTITY;
    RM->SetTransform(t);
    RM->SetTransform(getTransform());
    m_StaticMesh->Render(RM);
  }
}

