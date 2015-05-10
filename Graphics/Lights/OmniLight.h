#ifndef COMNILIGHT_H
#define COMNILIGHT_H

#include "Light.h"
#include "XML\XMLTreeNode.h"

class COmniLight : public CLight {
 public:
  COmniLight() : CLight() {}
  COmniLight(CXMLTreeNode &Node);
  ////////////////////////////
  void SetShadowMap(CGraphicsManager *RM) {};
};


#endif
