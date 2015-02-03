//----------------------------------------------------------------------------------
// CPhysicFixedJoint class
// Author: Enric Vergara
//
// Description:
// Esta clase representa joint de tipo esferico
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_PHYSIC_FIXED_JOINT_H_
#define INC_PHYSIC_FIXED_JOINT_H_

//---Forward Declarations---
class NxJoint;
class NxFixedJointDesc;
class CPhysicActor;
//--------------------------

class CPhysicFixedJoint
{
public:
	CPhysicFixedJoint();
	~CPhysicFixedJoint();

	void					CreateJoint			(NxJoint* joint);
	void					SetInfo				(CPhysicActor* actorA,  CPhysicActor* actorB =0);

	//---Get PhsX Info---
	NxJoint*				GetPhXJoint			( void ) const		{ return m_pJoint; }
	NxFixedJointDesc*		GetPhXDescJoint		( void ) const		{ return m_pFixedDesc; }

private:
	NxJoint					*m_pJoint;
	NxFixedJointDesc		*m_pFixedDesc;
};

#endif //INC_PHYSIC_FIXED_JOINT_H_