#pragma once
#ifndef POLY_PLATFORM_H
#define POLY_PLATFORM_H

#include <string>
#include "Math\Vector3.h"
#include "SceneElements\StaticPlatform.h"
#include "Utils\Defines.h"

class CPolyPlatform : public CStaticPlatform
{
protected:
    // std::string	m_RedimensionAxis;
    Vect3f		m_OriginalScale;
    float		m_RedimScale;
    bool		m_Activated;
    bool		m_Enabled;
    float		m_ActivationDistance;
    float		m_TimeOut;
    float		m_ActiveTime;
    bool		m_Collission;
    std::string		m_RedimAxis;
    float		m_Dt;

public:
    CPolyPlatform(std::string platformName, std::string coreName, std::string redimAxis, float redimScale, bool initialCollision, float activationDistance);
    virtual  ~ CPolyPlatform ();
    void ActivatePoly();
    void DeactivatePoly();
    void Update(float ElapsedTime);

    bool IsActivated()
    {
        return m_Activated;
    }
    void SetActivated(bool activated)
    {
        m_Activated = activated;
    }

    GET_SET(Vect3f, OriginalScale)
    GET_SET(float, RedimScale)
    GET_SET(bool, Enabled)
    GET_SET(float, ActivationDistance)
    GET_SET(float, TimeOut)
    GET_SET(float, ActiveTime)
    GET_SET(float, Collission)
    //  GET_SET(std::string, RedimAxis)
    std::string getRedimAxis()
    {
        return m_RedimAxis;
    }
    void setRedimAxis(std::string redimaxis)
    {
        m_RedimAxis = redimaxis;
    }
};

#endif