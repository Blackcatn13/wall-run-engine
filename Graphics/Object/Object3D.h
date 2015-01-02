//----------------------------------------------------------------------------------
// CObject3D class
// Author: Enric Vergara
//
// Description:
// Esta clase será la base de toda instancia que pueda existir en la escena
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_OBJECT_3D_H_
#define INC_OBJECT_3D_H_

#include "Math\MathTypes.h"
#include "Math\Vector3.h"
#include "Utils\Visible.h"
#include "Math\Matrix44.h"

class CObject3D : public CVisible
{
public:
    CObject3D(const Vect3f& pos, float yaw, float pitch, float roll = 0);
    CObject3D();
    virtual ~CObject3D(void)
    {
        /*Nothing*/;
    }

    //---Get Functions
    float                                               GetYaw                  () const
    {
        return m_fYaw;
    }
    float                                               GetRoll                 () const
    {
        return m_fRoll;
    }
    float                                               GetPitch                () const
    {
        return m_fPitch;
    }
    const Vect3f&               GetPosition     () const
    {
        return m_Position;
    }

    //---Set Functions
    void                SetPosition     ( const Vect3f& pos )
    {
        m_Position = pos;
    }
    void                SetYaw                  ( float yaw )
    {
        m_fYaw = yaw;
    }
    void                SetPitch                ( float pitch )
    {
        m_fPitch = pitch;
    }
    void                SetRoll                 ( float roll )
    {
        m_fRoll = roll;
    }

    void SetScale(Vect3f sc)
    {
        m_scale = sc;
    }

    Vect3f GetScale()
    {
        return m_scale;
    }

    Mat44f getTransform();

protected:
    Vect3f          m_Position;
    float               m_fYaw;
    float               m_fPitch;
    float               m_fRoll;
    Vect3f              m_scale;
    Mat44f              m_transform;
};

#endif //INC_CORE_H_