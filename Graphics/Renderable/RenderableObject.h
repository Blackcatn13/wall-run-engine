#pragma once
#ifndef CRENDERABLE_OBJECT_H
#define CRENDERABLE_OBJECT_H

#include "Utils/Defines.h"
#include "Object/Object3D.h"
#include "GraphicsManager.h"
#include "Utils/Named.h"

//#include "Mesh\MeshInstance.h"
class CXMLTreeNode;

class CRenderableObject : public CObject3D, public CNamed {
 protected:
  bool		m_Printable;
  std::string m_ParticleEmitter;
  int		m_Room;
 public:
  CRenderableObject();
  CRenderableObject(const CXMLTreeNode &TreeNode);
  virtual ~CRenderableObject() {}
  virtual void Update(float ElapsedTime) {
    if (getVisible() == true)
      m_Printable = true;
    else
      m_Printable = false;
  }
  GET_SET(bool, Printable);
  virtual void Render(CGraphicsManager *RM) = 0;
  GET_SET(std::string, ParticleEmitter);
  GET_SET(int, Room);
};

#endif
