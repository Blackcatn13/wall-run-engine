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
  std::string m_ParticleEmitter, m_ParticleEmitter2, m_ParticleEmitter3, m_ParticleEmitter4, m_Billboard;
  Vect3f m_EmitterOffset, m_EmitterOffset2, m_EmitterOffset3, m_EmitterOffset4, m_BillboardOffset;
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
  GET_SET(std::string, ParticleEmitter2);
  GET_SET(std::string, ParticleEmitter3);
  GET_SET(std::string, ParticleEmitter4);
  GET_SET(std::string, Billboard);
  GET_SET(Vect3f, EmitterOffset);
  GET_SET(Vect3f, EmitterOffset2);
  GET_SET(Vect3f, EmitterOffset3);
  GET_SET(Vect3f, EmitterOffset4);
  GET_SET(Vect3f, BillboardOffset);
  GET_SET(int, Room);
};

#endif
