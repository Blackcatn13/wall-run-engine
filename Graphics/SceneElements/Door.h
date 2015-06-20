#pragma once
#ifndef DOOR_H
#define DOOR_H

#include "SceneElement.h"
#include <string>
#include "Math\Vector3.h"
#include "Utils\Defines.h"

class CXMLTreeNode;

class CDoor : public CSceneElement {
 protected:
  bool			m_AnimationDone;
  std::string	m_LuaFunction;
  Vect3f		m_FinalPosition;
  Vect3f		m_OriginalPosition;
  bool			m_IsPlayingAnimation;

 public:
  CDoor(std::string switchName, std::string coreName, std::string lua_function, Vect3f final_Position);
  CDoor(const CXMLTreeNode &node);
  virtual ~CDoor();

  void Update(float dt);
  /* bool isAround(Vect3f vector1, Vect3f vector2);
   bool isInside(Vect3f vector1, Vect3f vector2);*/
  GET_SET(bool, AnimationDone)
  GET_SET(std::string, LuaFunction)
  GET_SET(Vect3f, FinalPosition)
  GET_SET(Vect3f, OriginalPosition)
  GET_SET(bool, IsPlayingAnimation)
};

#endif