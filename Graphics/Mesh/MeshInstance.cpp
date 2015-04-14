#include "MeshInstance.h"
#include "Utils\Defines.h"
#include "Core/Core.h"
#include "XML\XMLTreeNode.h"
#include "StaticMeshes\StaticMesh.h"
#include "StaticMeshes\StaticMeshManager.h"
#include "Core_Utils/MemLeaks.h"
#include "GraphicsManager.h"


CMeshInstance::CMeshInstance(const std::string &Name, const std::string &CoreName) {
  m_StaticMesh = SMESHM->GetResource(CoreName);
  setName(Name);
}

CMeshInstance::CMeshInstance(const CXMLTreeNode &Node) {
//TODO Cuando esté el código del Load que falta ponerlo aquí tb
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
    if (GRAPHM->isSphereVisible(m_Position, m_StaticMesh->GetBoundingSphere().GetRadius()))
      m_StaticMesh->Render(RM);
    else if (GRAPHM->isBoxVisible(m_StaticMesh->GetBoundingBox().GetMinPos(), m_StaticMesh->GetBoundingBox().GetMaxPos()))
      m_StaticMesh->Render(RM);
  }
}

