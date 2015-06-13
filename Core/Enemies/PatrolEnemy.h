#ifndef _PATROL_ENEMY_H
#define _PATROL_ENEMY_H

#pragma once

#include <vector>
#include "Enemy.h"
#include "Math\Vector3.h"

class CXMLTreeNode;

class CPatrolEnemy : public CEnemy
{
private:
    std::vector<Vect3f>		m_WayPoints;
public:
    CPatrolEnemy(CXMLTreeNode &info1);
    ~CPatrolEnemy();
    virtual void Init();
    virtual void Update(float elapsedTime);
    virtual void Render();
};

#endif