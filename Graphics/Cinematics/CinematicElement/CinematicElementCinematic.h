#pragma once

#ifndef CINEMATIC_ELEMENT_CINEMATIC_H
#define CINEMATIC_ELEMENT_CINEMATIC_H

#include "CinematicElement.h"
#include <string>

class CXMLTreeNode;

class CCinematicElementCinematic : public CCinematicElement {
 public:
  CCinematicElementCinematic(const CXMLTreeNode &node);
  virtual ~CCinematicElementCinematic();
  bool Execute();
 private:
  bool m_played;
};


#endif