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
 public:
  CMeshInstance(const std::string &Name, const std::string &CoreName);
  CMeshInstance(const CXMLTreeNode &Node);
  ~CMeshInstance();
  void Render(CGraphicsManager *RM);
  void Update(float dt) {};
};

#endif
