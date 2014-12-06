#ifndef _BOSS_ENEMY_H
#define _BOSS_ENEMY_H

#pragma once

#include "Enemy.h"

class CXMLTreeNode;


class CBossEnemy : public CEnemy
{
public:
	CBossEnemy(CXMLTreeNode &info1);
	~CBossEnemy();
	virtual void Init();
	virtual void Update(float elapsedTime);
	virtual void Render();
};

#endif
