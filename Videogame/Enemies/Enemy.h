#ifndef _ENEMY_H
#define _ENEMY_H

#pragma once

#include <string>
#include "Math\Vector3.h"
#include "Utils\Defines.h"

class CXMLTreeNode;

class CEnemy
{
protected:
    std::string		m_Name;
    float			m_Life;
    Vect3f			m_Position;
    float			m_Yaw;
    float			m_TimeToSpawn;
    float			m_TimeToShoot;
    float			m_ShootAccuracy;

    CEnemy();
public:
    CEnemy(CXMLTreeNode &info1);
    virtual ~CEnemy();
    virtual void Init() = 0;
    virtual void Update(float elapsedTime) = 0;
    virtual void Render() = 0;
    GET_SET(float, Life);
    GET_SET(float, Yaw);
    GET_SET_REF(std::string, Name);
    GET_SET_REF(Vect3f, Position);
    GET_SET(float, TimeToSpawn);
    GET_SET(float, TimeToShoot);
    GET_SET(float, ShootAccuracy);
    // get & set
    // set(const <type> &name)
    // const <type> & get
};

#endif