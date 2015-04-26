#ifndef CMESHINSTANCE_H
#define CMESHINSTANCE_H

#include <string>
#include "Renderable\RenderableObject.h"

class CStaticMesh;
class CGraphicsManager;
class CXMLTreeNode;

class CMeshInstance : public CRenderableObject {
 private:
  CStaticMesh *m_StaticMesh;
  std::string m_CoreName;
 public:
  CMeshInstance(const std::string &Name, const std::string &CoreName);
  CMeshInstance(const CXMLTreeNode &Node);
  ~CMeshInstance();
  void Render(CGraphicsManager *RM);
  void Update(float dt) {};
  std::string GetCoreName() {
    return m_CoreName;
  }
};

#endif
