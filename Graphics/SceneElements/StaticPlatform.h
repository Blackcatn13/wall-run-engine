#pragma once
#ifndef STATIC_PLATFORM_H
#define STATIC_PLATFORM_H

#include "Object\Object3D.h"
#include <string>
#include "Math\Vector3.h"

class CRenderableObject;
class CPhysicActor;
class CPhysicUserData;

class CStaticPlatform : public CObject3D
{
protected:
    CRenderableObject * m_RenderableObject;
    CPhysicActor * m_PlatorformActor;
    CPhysicUserData * m_PlatformUserData;

public:
    CStaticPlatform(std::string boxName, std::string userDataName, Vect3f size, Vect3f globalPosition, Vect3f localPosition);
    ~ CStaticPlatform ();

};

#endif