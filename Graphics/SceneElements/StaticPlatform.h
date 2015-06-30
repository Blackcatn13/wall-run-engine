#pragma once
#ifndef STATIC_PLATFORM_H
#define STATIC_PLATFORM_H

#include "Object\Object3D.h"
#include <string>
#include "Math\Vector3.h"
#include "Renderable\RenderableObject.h"
#include "Mesh\MeshInstance.h"
#include "Utils\Defines.h"
#include "SceneElement.h"

class CXMLTreeNode;

class CStaticPlatform : public CSceneElement {
 public:
  CStaticPlatform(std::string platformName, std::string coreName, bool hasRigidBody = false);
  CStaticPlatform(const CXMLTreeNode &node, bool hasRigidBody = false);
  void Update(float dt);
  virtual ~CStaticPlatform();
  /* void InsertPlatform( std::string userDataName, Vect3f size, Vect3f localPosition);
   CPhysicActor *GetPlatformActor() {
     return m_PlatorformActor;
   }
   Vect3f GetPhysicsSize() {
     return m_PlatformPhysicsSize;
   }*/
  //bool isAround(Vect3f vector1, Vect3f vector2);
  //bool isInside(Vect3f vector1, Vect3f vector2);
};

#endif