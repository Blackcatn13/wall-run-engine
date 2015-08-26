#pragma once

#ifndef CINEMATIC_OBJECT_H
#define CINEMATIC_OBJECT_H

#include "CinematicPlayer.h"
#include "CinematicObjectKeyFrame.h"
#include "Renderable\RenderableObject.h"
#include "XML\XMLTreeNode.h"
#include <vector>

class CCinematicObject : public CCinematicPlayer {
 private:
  std::vector<CCinematicObjectKeyFrame *>		m_CinematicObjectKeyFrames;
  size_t										                m_CurrentKeyFrame;
  size_t                                    m_NextKeyFrame;
  CRenderableObject							            *m_RenderableObject;

 public:
  CCinematicObject(CXMLTreeNode &atts);
  virtual ~CCinematicObject();
  bool IsOk() {
    return (m_RenderableObject != NULL);
  }
  void AddCinematicObjectKeyFrame(CCinematicObjectKeyFrame *CinematicObjectKeyFrame);
  void Update(float ElapsedTime);
  void Stop();
  void OnRestartCycle();
  void CalculateFrame();
};

#endif