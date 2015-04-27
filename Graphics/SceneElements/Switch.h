#pragma once
#ifndef SWITCH_H
#define SWITCH_H

#include "Object\Object3D.h"
#include <string>
#include "Math\Vector3.h"
#include "Renderable\RenderableObject.h"
#include "Mesh\MeshInstance.h"
#include "Utils\Defines.h"

class CPhysicActor;
class CPhysicUserData;

class CSwitch : public CMeshInstance
{
protected:
    // CRenderableObject * m_RenderableObject;
    CPhysicActor * m_SwitchActor;
    CPhysicUserData * m_SwitchUserData;
	Vect3f m_SwitchPhysicsSize;

public:
    CSwitch(std::string switchName, std::string coreName);
    ~CSwitch ();
    void InsertPhisicSwitch( std::string userDataName, Vect3f size, Vect3f localPosition);
    CPhysicActor * GetSwitchActor()
    {
        return m_SwitchActor;
    }
	Vect3f GetPhysicsSize()
	{
		return m_SwitchPhysicsSize;
	}

};

#endif