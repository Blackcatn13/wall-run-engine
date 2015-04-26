#pragma once
#ifndef CPIXELITE_H
#define CPIXELITE_H


#include "Collectible.h"
#include <string.h>

class CRenderableObject;
class CPixelite: public CCollectible {
 protected:


 public:
// CPixelite(std::string coreName, std::string name, std::string layerName, std::string userDataName, std::string LuaCode);
  CPixelite(CRenderableObject *RendObj, std::string layerName, std::string MeshLuaCode, std::string TriggerLuaCode);


  void update(float dt);

};

#endif