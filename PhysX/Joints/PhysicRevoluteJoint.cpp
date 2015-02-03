#define __DONT_INCLUDE_MEM_LEAKS__
#include "Joints\PhysicRevoluteJoint.h"
#include "NxRevoluteJointDesc.h"
#include "NxRevoluteJoint.h"
#include "Actor\PhysicActor.h"
#include "Utils\Logger.h"
#include "Utils\Defines.h"

#if defined(_DEBUG)
#include "Core_Utils\MemLeaks.h"
#endif

CPhysicRevoluteJoint::CPhysicRevoluteJoint()
: m_pJoint(NULL)
, m_pRevoluteDesc(NULL)
{
	m_pRevoluteDesc = new NxRevoluteJointDesc;
}

CPhysicRevoluteJoint::~CPhysicRevoluteJoint()
{
	CHECKED_DELETE(m_pRevoluteDesc);
}

void CPhysicRevoluteJoint::CreateJoint(NxJoint* joint)
{
	assert(joint);
	m_pJoint = joint;
	CHECKED_DELETE(m_pRevoluteDesc);
}

void CPhysicRevoluteJoint::SetInfo	(const Vect3f& axis, const Vect3f& anchor, CPhysicActor* actorA,  CPhysicActor* actorB)
{
	if (actorA==NULL)
	{
		LOGGER->AddNewLog(ELL_ERROR, "PhysicRevoluteJoint:: El primer actor pasado como argumento no puede ser null");
		return;
	}

	m_pRevoluteDesc->actor[0] = actorA->GetPhXActor(); 
	if (actorB!=NULL)
	{
		m_pRevoluteDesc->actor[1] = actorB->GetPhXActor();
	}
	else
	{
		m_pRevoluteDesc->actor[1] = NULL;
	}

	m_pRevoluteDesc->setGlobalAxis( NxVec3(axis.x, axis.y, axis.z) ); //The direction of the axis the bodies revolve around.
	m_pRevoluteDesc->setGlobalAnchor( NxVec3(anchor.x, anchor.y, anchor.z) ); //Reference point that the axis passes through.

}

void CPhysicRevoluteJoint::SetMotor (float maxForce, float veltarget, bool limit, float limHigh, float limLow)
{
	m_pRevoluteDesc->motor.maxForce = maxForce;
	m_pRevoluteDesc->motor.velTarget = veltarget;
	m_pRevoluteDesc->flags |= NX_RJF_MOTOR_ENABLED;

	if (limit)
	{
		NxJointLimitPairDesc limits;
		m_pRevoluteDesc->flags |= NX_RJF_LIMIT_ENABLED;
		limits.high.value = (NxReal)limHigh;
		limits.low.value = (NxReal)limLow;
		m_pRevoluteDesc->limit = limits;
	}
}

void CPhysicRevoluteJoint::ActiveMotor (float velocity)
{
	assert(m_pJoint);
	NxMotorDesc motorDesc;
	NxRevoluteJoint* revJoint=(NxRevoluteJoint *)m_pJoint;
	revJoint->getMotor(motorDesc);
	motorDesc.velTarget=velocity;
	revJoint->setMotor(motorDesc);
}