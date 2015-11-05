#pragma once

#ifndef CINEMATIC_H
#define CINEMATIC_H

#include "CinematicPlayer.h"
#include "CinematicObject.h"
#include "CinematicObjectKeyFrame.h"
#include "Renderable\RenderableObject.h"
#include "XML\XMLTreeNode.h"
#include "GraphicsManager.h"
#include <vector>

class CCinematic : public CRenderableObject, public CCinematicPlayer {
 protected:
  std::vector<CCinematicObject *>		m_CinematicObjects;
 public:
//	CCinematic(MKeyValue &atts);
  CCinematic();
  virtual ~CCinematic();
  virtual void Stop();
  virtual void Play(bool Cycle);
  virtual void Pause();
  void ToEnd();
  std::string LoadXML(const std::string &Filename);
  void AddCinematicObject(CCinematicObject *CinematicObject);
  void Update(float ElapsedTime);
  virtual void Render(CGraphicsManager *RM) {}
};

#endif