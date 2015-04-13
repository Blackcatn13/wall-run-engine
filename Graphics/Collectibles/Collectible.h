#pragma once
#ifndef COLLECTIBLE_H
#define COLLECTIBLE_H

#include <string>

#include "Utils\Defines.h"
#include "Mesh\MeshInstance.h"

class CGraphicsManager;

class CCollectible: public CMeshInstance {
 protected:
	std::string m_LayerName;
	std::string m_LuaCode;
	std::string m_UserDataName;

 public:
  CCollectible(std::string coreName, std::string name, std::string layerName, std::string userDataName, std::string luaCode);
  virtual  ~ CCollectible ();

  GET_SET(std::string, LayerName);

  void update(float dt);	

};

#endif