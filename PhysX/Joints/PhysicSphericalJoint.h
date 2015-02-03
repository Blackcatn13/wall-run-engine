//----------------------------------------------------------------------------------
// CPhysicSphericalJoint class
// Author: Enric Vergara
//
// Description:
// Esta clase representa joint de tipo esferico
//----------------------------------------------------------------------------------
#pragma once

#ifndef INC_PHYSIC_SPHERICAL_JOINT_H_
#define INC_PHYSIC_SPHERICAL_JOINT_H_

#include "Math\Vector3.h"
#include "Joints\JointsDefs.h"

//---Forward Declarations---
class NxJoint;
class NxSphericalJointDesc;
class CPhysicActor;
//--------------------------

class CPhysicSphericalJoint
{
public:
	CPhysicSphericalJoint();
	~CPhysicSphericalJoint();

	void					CreateJoint			(NxJoint* joint);
	void					SetInfoComplete		(const Vect3f& anchor, const Vect3f& _axis, CPhysicActor* actorA,  CPhysicActor* actorB =0);
	void					SetInfoRagdoll		(SSphericalLimitInfo _sInfo, CPhysicActor* actorA,  CPhysicActor* actorB =0);
	void					SetInfo				(const Vect3f& anchor, CPhysicActor* actorA,  CPhysicActor* actorB =0);

	//---Get PhsX Info---
	NxJoint*				GetPhXJoint			( void ) const		{	return m_pJoint; }
	NxSphericalJointDesc*	GetPhXDescJoint		( void ) const		{ return m_pSphericalDesc; }

private:
	NxJoint					*m_pJoint;
	NxSphericalJointDesc	*m_pSphericalDesc;
};

#endif //INC_PHYSIC_SPHERICAL_JOINT_H_