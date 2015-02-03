//----------------------------------------------------------------------------------
// CPhysicRevoluteJoint class
// Author: Enric Vergara
//
// Description:
// Esta clase representa joint de tipo revolute el cual puede ser modificado mediante un motor
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_PHYSIC_REVOLUTE_JOINT_H_
#define INC_PHYSIC_REVOLUTE_JOINT_H_


#include "Math\Vector3.h"

//---Forward Declarations---
class NxJoint;
class NxRevoluteJointDesc;
class CPhysicActor;
//--------------------------

class CPhysicRevoluteJoint
{
public:
							CPhysicRevoluteJoint	( void );
							~CPhysicRevoluteJoint	( void );

	void					CreateJoint				( NxJoint* joint );
	void					SetInfo					( const Vect3f& axis, const Vect3f& anchor, CPhysicActor* actorA,  CPhysicActor* actorB = 0 );
													  
	void					SetMotor				( float maxForce, float veltarget, bool limit=false, float limHigh=0.f, float limLow=0.f );
	void					ActiveMotor				( float velocity );

	//---Get PhsX Info---
	NxJoint*				GetPhXJoint				( void ) const		{ return m_pJoint; }
	NxRevoluteJointDesc*	GetPhXDescJoint			( void ) const		{ return m_pRevoluteDesc; }


private:
	NxJoint					*m_pJoint;
	NxRevoluteJointDesc		*m_pRevoluteDesc;
};

#endif //INC_PHYSIC_REVOLUTE_JOINT_H_