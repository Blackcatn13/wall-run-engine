#pragma once

#ifndef CINEMATIC_ELEMENT_CINEMATIC_H
#define CINEMATIC_ELEMENT_CINEMATIC_H

#include "CinematicElement.h"
#include <string>

class CXMLTreeNode;
class CCinematic;

class CCinematicElementCinematic : public CCinematicElement {
 public:
  CCinematicElementCinematic(const CXMLTreeNode &node);
  virtual ~CCinematicElementCinematic();
  bool Execute();
  bool Update(float dt);
  void Stop();
 private:
  bool m_played;
  CCinematic *m_cinematic;

};


#endif