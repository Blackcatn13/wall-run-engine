#pragma once
#ifndef SWITCH_H
#define SWITCH_H

#include "SceneElement.h"
#include "Utils\Defines.h"

class CXMLTreeNode;

class CSwitch : public CSceneElement {
 private:
  Vect3f m_OriginalPosition;
  bool m_Switched;
 public:
  CSwitch(std::string switchName, std::string coreName);
  CSwitch(const CXMLTreeNode &node);
  void ResetSwitch();
  virtual ~CSwitch ();
  Vect3f GetOriginalPosition() {
    return m_OriginalPosition;
  }
  void Update(float dt);
  GET_SET(bool, Switched);
};

#endif