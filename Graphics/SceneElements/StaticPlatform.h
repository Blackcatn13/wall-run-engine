#pragma once
#ifndef STATIC_PLATFORM_H
#define STATIC_PLATFORM_H

#include "Object\Object3D.h"
#include <string>
#include "Math\Vector3.h"
#include "Renderable\RenderableObject.h"
#include "Mesh\MeshInstance.h"
#include "Utils\Defines.h"

class CPhysicActor;
class CPhysicUserData;

class CStaticPlatform : public CMeshInstance
{
protected:
    // CRenderableObject * m_RenderableObject;
    CPhysicActor * m_PlatorformActor;
    CPhysicUserData * m_PlatformUserData;
	Vect3f m_PlatformPhysicsSize;

public:
    CStaticPlatform(std::string platformName, std::string coreName);
    virtual  ~ CStaticPlatform ();
    void InsertPlatform( std::string userDataName, Vect3f size, Vect3f localPosition);
    CPhysicActor * GetPlatformActor()
    {
        return m_PlatorformActor;
    }
	Vect3f GetPhysicsSize()
	{
		return m_PlatformPhysicsSize;
	}

};

#endif