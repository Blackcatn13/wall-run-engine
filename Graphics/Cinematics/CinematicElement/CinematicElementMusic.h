#pragma once

#ifndef CINEMATIC_ELEMENT_MUSIC_H
#define CINEMATIC_ELEMENT_MUSIC_H

#include "CinematicElement.h"
#include <string>

class CXMLTreeNode;
class CCinematic;

class CCinematicElementMusic : public CCinematicElement {
 public:
  CCinematicElementMusic(const CXMLTreeNode &node);
  virtual ~CCinematicElementMusic();
  bool Execute();
  bool Update(float dt);
  void Stop() {}
 private:
  bool m_played;
  std::string m_Event;
  std::string m_Actor;
};


#endif