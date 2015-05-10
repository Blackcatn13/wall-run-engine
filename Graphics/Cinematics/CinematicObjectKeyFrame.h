#pragma once

#ifndef CINEMATIC_OBJECT_KEY_FRAME_H
#define CINEMATIC_OBJECT_KEY_FRAME_H

#include "Object\Object3D.h"
#include "XML\XMLTreeNode.h"

class CCinematicObjectKeyFrame : public CObject3D {
 private:
  float m_KeyFrameTime;
 public:
  CCinematicObjectKeyFrame(CXMLTreeNode &atts);
  float GetKeyFrameTime() {
    return m_KeyFrameTime;
  }
};

#endif