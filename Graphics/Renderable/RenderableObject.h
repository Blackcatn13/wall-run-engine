#pragma once
#ifndef CRENDERABLE_OBJECT_H
#define CRENDERABLE_OBJECT_H

#include "Utils/Defines.h"
#include "Object/Object3D.h"
#include "GraphicsManager.h"
#include "Utils/Named.h"
//#include "Mesh\MeshInstance.h"

class CRenderableObject : public CObject3D, public CNamed
{
protected:
    bool		m_Printable;
public:
    CRenderableObject();
    virtual ~CRenderableObject() {}
    virtual void Update(float ElapsedTime)
    {
        if (getVisible() == true)
            m_Printable = true;
        else
            m_Printable = false;
    }
    virtual void Render(CGraphicsManager *RM) = 0;
};

#endif
