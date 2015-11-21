#pragma once

#ifndef CINEMATIC_ELEMENT_LUA_H
#define CINEMATIC_ELEMENT_LUA_H

#include "CinematicElement.h"
#include <string>

class CXMLTreeNode;
class CCinematic;

class CCinematicElementLua : public CCinematicElement {
 public:
  CCinematicElementLua(const CXMLTreeNode &node);
  virtual ~CCinematicElementLua();
  bool Execute();
  bool Update(float dt);
  bool m_ExecuteOnUpdate;
  void Stop() {}
 private:
  bool m_played;
  std::string m_code;
  float m_time;
  float m_auxTime;
  bool m_withTime;
  bool m_onlyUpdate;
  bool m_printTime;

};


#endif