#pragma once

#ifndef _PHYSICS_DEFS_H
#define _PHYSICS_DEFS_H

#include <string>
#include "Math\Vector3.h"

//---- MACROS ------------
#define MAX_ARISTAS 10


//---- Declaracion de nuevos tipos------------
struct SPhysicsInitParams
{
	SPhysicsInitParams::SPhysicsInitParams( void )
		: m_fSkinWidth(0.001f)
		, m_fGravity(-9.81f)
		, m_Restitution_DefMat(0.5f)
		, m_StaticFriction_DefMat(0.5f)
		, m_DynamicFriction_DefMat(0.5f) 
	{}

	float		m_fSkinWidth;
	float		m_fGravity;
	float		m_Restitution_DefMat;				//Restitution of the default material
	float		m_StaticFriction_DefMat;		//Static Friction of the default material
	float		m_DynamicFriction_DefMat;		//Dynamic Friction of the default material
};

struct SCollisionInfo
{
	float		m_fDistance;
	Vect3f		m_Normal;
	Vect3f		m_CollisionPoint;
};

enum ECollisionGroup {
	ECG_ESCENE,
	ECG_PLAYER,
	ECG_ENEMY,
	ECG_TRIGGERS,
	ECG_MALGLOBAL,
	ECG_COBERTURES,
	ECG_DYNAMIC_OBJECTS,
	ECG_STATIC_OBJECTS,
	ECG_EXPLOSIONS,
	ECG_RAY_SHOOT,
	ECG_RAY_SHOOT_PLAYER,
	ECG_FORCE,
	ECG_RAY_IA_GRAPH,
	ECG_CAMERA,
	ECG_RAGDOLL,
	ECG_RAGDOLL_PLAYER,
	ECG_VIGIA,
	ECG_LIMITS,			// Representa muros o límites del escenario por donde el player no debería poder cruzar

	//aquest ha de ser l'últim sempre!!!!
	ECG_LAST_GROUP
};

enum EControleType
{
	BOX,
	CAPSULE
};

//enum CollisionGroup
//{
//	GROUP_NON_COLLIDABLE,
//	GROUP_COLLIDABLE_NON_PUSHABLE,
//	GROUP_COLLIDABLE_PUSHABLE,
//};

#endif