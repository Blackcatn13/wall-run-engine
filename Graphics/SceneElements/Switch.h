#pragma once
#ifndef SWITCH_H
#define SWITCH_H

#include "SceneElement.h"

class CXMLTreeNode;

class CSwitch : public CSceneElement {
 private:
  Vect3f m_OriginalPosition;
 public:
  CSwitch(std::string switchName, std::string coreName);
  CSwitch(const CXMLTreeNode &node);
  void ResetSwitch();
  virtual ~CSwitch ();
  Vect3f GetOriginalPosition() {
    return m_OriginalPosition;
  }


};

#endif