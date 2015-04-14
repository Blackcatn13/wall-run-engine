#pragma once
#ifndef COLLECTIBLE_H
#define COLLECTIBLE_H

#include <string>

#include "Utils\Defines.h"
#include "Mesh\MeshInstance.h"

class CGraphicsManager;
class CPhysicActor;
class CPhysicUserData;

class CCollectible: public CMeshInstance {
 protected:
  std::string m_LayerName;
  std::string m_LuaCode;
  std::string m_UserDataName;
  CPhysicUserData *m_CollectibleUserData;
  CPhysicActor *m_CollectibleActor;

 public:
  CCollectible(std::string coreName, std::string name, std::string layerName, std::string userDataName, std::string luaCode);
  virtual  ~ CCollectible ();
  void SetActor();
  GET_SET(std::string, LayerName);

  void Update(float dt);

};

#endif