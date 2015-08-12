#pragma once

#ifndef CINEMATIC_ELEMENT_CAMERA_H
#define CINEMATIC_ELEMENT_CAMERA_H

#include "CinematicElement.h"
#include <string>

class CXMLTreeNode;

class CCinematicElementCamera : public CCinematicElement {
 public:
  CCinematicElementCamera(const CXMLTreeNode &node);
  virtual ~CCinematicElementCamera();
  bool Execute();
 private:
  std::string m_cameraName;
  std::string m_returnCamera;
  bool m_played;
};


#endif