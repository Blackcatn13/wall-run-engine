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
class CXMLTreeNode;

class CSceneElement : public CMeshInstance {
 protected:
  // CRenderableObject * m_RenderableObject;
  CPhysicActor *m_Actor;
  CPhysicUserData *m_UserData;
  CPhysicActor *m_ActorAux;
  CPhysicUserData *m_UserDataAux;
  Vect3f m_PhysicsSize;
  std::string m_Room;
  bool m_HasRigidBody;
  bool m_HasPhisicMesh;

 public:
  CSceneElement(std::string switchName, std::string coreName, bool hasRigidBody = false, bool hasPhisicMesh = false);
  CSceneElement(const CXMLTreeNode &node, bool hasRigidBody = false);
  virtual ~CSceneElement ();
  void InsertPhisic(Vect3f localPosition);
  std::string SetUserDataName(std::string name);
  void ActivatePhisic(bool active);
  bool isAround(Vect3f vector1, Vect3f vector2);
  bool isInside(Vect3f vector1, Vect3f vector2);

  CPhysicActor *GetActor() {
    return m_Actor;
  }
  Vect3f GetPhysicsSize() {
    return m_PhysicsSize;
  }

  GET_SET(std::string, Room);
};

#endif