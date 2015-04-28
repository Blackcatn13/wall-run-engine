#pragma once
#ifndef CSCENE_ELEMENT_H
#define CSCENE_ELEMENT_H

#include "Object\Object3D.h"
#include <string>
#include "Math\Vector3.h"
#include "Renderable\RenderableObject.h"
#include "Mesh\MeshInstance.h"
#include "Utils\Defines.h"

class CPhysicActor;
class CPhysicUserData;

class CSceneElement : public CMeshInstance {
 protected:
  // CRenderableObject * m_RenderableObject;
  CPhysicActor *m_Actor;
  CPhysicUserData *m_UserData;
  Vect3f m_PhysicsSize;

 public:
  CSceneElement(std::string switchName, std::string coreName);
  virtual ~CSceneElement ();
  void InsertPhisic( std::string userDataName, Vect3f size, Vect3f localPosition);
  CPhysicActor *GetActor() {
    return m_Actor;
  }
  Vect3f GetPhysicsSize() {
    return m_PhysicsSize;
  }

};

#endif