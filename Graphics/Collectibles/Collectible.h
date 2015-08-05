#pragma once
#ifndef COLLECTIBLE_H
#define COLLECTIBLE_H

#include <string>

#include "Utils\Defines.h"
#include "Mesh\MeshInstance.h"
#include "Utils\Named.h"
#include "Utils\Defines.h"

class CGraphicsManager;
class CRenderableObject;
class CTrigger;

class CCollectible : public CNamed {
 private:
  CRenderableObject		*m_RenderableObject;
  std::string			    m_LuaCode;
  std::string			    m_TriggerLuaCode;
  std::string			    m_LayerName;
  CTrigger				    *m_Trigger;
  std::string				m_ParamTriggerFunction;
 public:
  //CCollectible(std::string coreName, std::string name, std::string layerName, std::string userDataName, std::string luaCode);
  CCollectible(CRenderableObject *RendObj, std::string layerMame, std::string MeshLuaCode, std::string TriggerLuaCode,  Vect3f triggerSize,  std::string param2, bool visible);
  virtual  ~ CCollectible ();


  virtual void Update(float dt);

  GET_SET(std::string, LuaCode)
  GET_SET(std::string, TriggerLuaCode)
  GET_SET(CRenderableObject *, RenderableObject)
  GET_SET(std::string, LayerName)
  GET_SET(std::string, ParamTriggerFunction)


};

#endif