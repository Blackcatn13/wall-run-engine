#pragma once
#ifndef CCROMO_H
#define CCROMO_H

#include "Collectible.h"
#include <string.h>

class CGraphicsManager;

class CCromo: public CCollectible {
 protected:


 public:
  CCromo(std::string coreName, std::string name, std::string layerName,  std::string userDataName,std::string luaCode);
 

  void update(float dt);	

};

#endif