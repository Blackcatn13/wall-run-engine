#pragma once

#ifndef CINEMATIC_ELEMENT_ANIMATION_H
#define CINEMATIC_ELEMENT_ANIMATION_H

#include "CinematicElement.h"
#include <string>

class CXMLTreeNode;
class CCinematic;

class CCinematicElementAnimation : public CCinematicElement {
 public:
  CCinematicElementAnimation(const CXMLTreeNode &node);
  virtual ~CCinematicElementAnimation();
  bool Execute();
  bool Update(float dt);
 private:
  bool m_played;
  int m_room;
  std::string m_layer;
  std::string m_animatedModel;
  int m_animationId;
};


#endif