#pragma once
#ifndef DOOR_H
#define DOOR_H

#include "SceneElement.h"
#include <string>
#include "Math\Vector3.h"
#include "Utils\Defines.h"

class CDoor : public CSceneElement {
 protected:
  bool			m_IsOpening;
  std::string	m_LuaFunction;
  Vect3f		m_FinalPosition;

 public:
  CDoor(std::string switchName, std::string coreName, std::string lua_function, Vect3f final_Position);
//  ~CSwitch ();

  void Update(float dt);
  /* bool isAround(Vect3f vector1, Vect3f vector2);
   bool isInside(Vect3f vector1, Vect3f vector2);*/
  GET_SET(bool, IsOpening)
  GET_SET(std::string, LuaFunction)
  GET_SET(Vect3f, FinalPosition)

};

#endif