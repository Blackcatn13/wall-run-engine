#ifndef INSTANCEDBILLBOARD_H_
#define INSTANCEDBILLBOARD_H_

#include <vector>
#include "Math\Vector3.h"
#include "Math\Color.h"
#include "Utils\Defines.h"
#include "Particles\Billboard.h"
#include "RenderableVertex\VertexTypes.h"

class CGraphicsManager;
class CXMLTreeNode;
class CTexture;
class CRenderableVertexs;

class CInstancedBillboard : public CBillboard {
 public:
  CInstancedBillboard() {}
  CInstancedBillboard(CXMLTreeNode &node);
  virtual ~CInstancedBillboard();
  void Update(float ElapsedTime);
  void Render(CGraphicsManager *GM);
 private:
  int   m_numBillboards;
  TBILLBOARD_VERTEX_INSTANCE *m_vertex_list;
};

#endif // INSTANCEDBILLBOARD_H_