#pragma once
#ifndef SWITCH_H
#define SWITCH_H

#include "SceneElement.h"

class CXMLTreeNode;

class CSwitch : public CSceneElement {
 public:
  CSwitch(std::string switchName, std::string coreName);
  CSwitch(const CXMLTreeNode &node);
  virtual ~CSwitch ();
};

#endif