
#pragma once


#ifndef _PHYSX_OBJ_DEFS_H
#define _PHYSX_OBJ_DEFS_H

#include <string>
#include "Math\Vector3.h"
#include "Object\Object3D.h"
#include "Utils\Types.h"
#include "Utils\Named.h"

#define PHYSX_OBJ_BOX	0x0001

struct TPhysXObj : public CObject3D, public CNamed {
    int m_Type;
    uint32 m_Group;

    TPhysXObj()
        : m_Type(0)
        , m_Group(0)
    {
    }
};

struct TPhysXObjBox : public TPhysXObj {
    Vect3f m_Dimensions;

    TPhysXObjBox()
        : m_Dimensions(v3fZERO)
    {
        m_Type = PHYSX_OBJ_BOX;
    }
};

#endif