#pragma once
#ifndef CCROMO_H
#define CCROMO_H

#include "Collectible.h"
#include <string.h>

class CGraphicsManager;


class CCromo: public CCollectible {
 protected:


 public:
  CCromo(CRenderableObject *RendObj, std::string layerName, std::string MeshLuaCode, std::string TriggerLuaCode);


  void update(float dt);

};

#endif