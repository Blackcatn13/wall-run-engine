#pragma once
#ifndef MOVING_PLATFORM_H
#define MOVING_PLATFORM_H

#include <string>
#include "Math\Vector3.h"
#include "StaticPlatform.h"
#include <vector>




class CMovingPlatform : public CStaticPlatform
{
protected:
    float				m_Speed;
    Vect3f				m_Direction;
    std::vector<Vect3f> m_WayPointsVector;
    int					m_CurrentWpId;


public:
    CMovingPlatform(std::string platformName, std::string coreName, float speed);

    ~ CMovingPlatform ();

    void MoveToPoint(float dt, Vect3f point, float minDistance);
    Vect3f GetNextWP();
    std::vector<Vect3f>& GetWayPointsVector()
    {
        return m_WayPointsVector;
    }



};

#endif