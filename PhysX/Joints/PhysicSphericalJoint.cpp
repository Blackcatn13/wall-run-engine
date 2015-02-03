#define __DONT_INCLUDE_MEM_LEAKS__
#include "Joints\PhysicSphericalJoint.h"
#include "NxSphericalJointDesc.h"
#include "Actor\PhysicActor.h"
#include "Utils\Logger.h"
#include "Utils\BaseUtils.h"
#include "Utils\Defines.h"

#if defined(_DEBUG)
#include "Core_Utils\MemLeaks.h"
#endif

CPhysicSphericalJoint::CPhysicSphericalJoint()
: m_pJoint(NULL)
, m_pSphericalDesc(NULL)
{
	m_pSphericalDesc = new NxSphericalJointDesc;
}

CPhysicSphericalJoint::~CPhysicSphericalJoint()
{
	CHECKED_DELETE(m_pSphericalDesc);
}

void CPhysicSphericalJoint::CreateJoint(NxJoint* joint)
{
	assert(joint);
	m_pJoint = joint;
	CHECKED_DELETE(m_pSphericalDesc);
}

void CPhysicSphericalJoint::SetInfo	(const Vect3f& anchor, CPhysicActor* actorA,  CPhysicActor* actorB)
{
	if (actorA==NULL)
	{
		LOGGER->AddNewLog(ELL_ERROR, "PhysicSphericalJoint:: El primer actor pasado como argumento no puede ser null");
		return;
	}

	NxVec3 pos(anchor.x, anchor.y, anchor.z );

	m_pSphericalDesc->setToDefault();
	m_pSphericalDesc->actor[0] = actorA->GetPhXActor();
	if (actorB!=NULL)
	{
		m_pSphericalDesc->actor[1] = actorB->GetPhXActor();	
	}
	else
	{
		m_pSphericalDesc->actor[1] = NULL;	
	}


	m_pSphericalDesc->setGlobalAnchor(pos);

   
 
}

void CPhysicSphericalJoint::SetInfoComplete	(const Vect3f& anchor, const Vect3f& _axis, CPhysicActor* actorA,  CPhysicActor* actorB)
{
	if (actorA==NULL)
	{
		LOGGER->AddNewLog(ELL_ERROR, "PhysicSphericalJoint:: El primer actor pasado como argumento no puede ser null");
		return;
	}

	NxVec3 pos(anchor.x, anchor.y, anchor.z );
	NxVec3 axis(_axis.x, _axis.y, _axis.z );

	m_pSphericalDesc->setToDefault();
	m_pSphericalDesc->actor[0] = actorA->GetPhXActor();
	if (actorB!=NULL)
	{
		m_pSphericalDesc->actor[1] = actorB->GetPhXActor();	
	}
	else
	{
		m_pSphericalDesc->actor[1] = NULL;	
	}


 
  //LIMITS PELS TWIST!!!!!!! (gir de munyeca)
  m_pSphericalDesc->flags |= NX_SJF_TWIST_LIMIT_ENABLED;
  m_pSphericalDesc->twistLimit.low.value = 0.025f*NxPi;
  m_pSphericalDesc->twistLimit.low.restitution = 0.0f;
  m_pSphericalDesc->twistLimit.high.value = 0.025f*NxPi;
  m_pSphericalDesc->twistLimit.high.restitution = 0.0f;

  //Es pot push pero al retornar, com mes petit es el valor, menys espai recorre.
  m_pSphericalDesc->flags |= NX_SJF_SWING_LIMIT_ENABLED;
  m_pSphericalDesc->swingLimit.value = 0.1f*NxPi;
  m_pSphericalDesc->swingLimit.restitution = 0.0f;

  //Twist Spring Enabled
  m_pSphericalDesc->flags |= NX_SJF_TWIST_SPRING_ENABLED;
  m_pSphericalDesc->twistSpring.damper = 1.0f;
  m_pSphericalDesc->twistSpring.spring = 0.5f;

  //Swing Spring Enabled
  m_pSphericalDesc->flags |= NX_SJF_SWING_SPRING_ENABLED;
  m_pSphericalDesc->swingSpring.damper = 1.0f;
  m_pSphericalDesc->swingSpring.spring = 0.5f;
  
  //Joint Springs
  /*m_pSphericalDesc->flags |= NX_SJF_JOINT_SPRING_ENABLED;
  m_pSphericalDesc->jointSpring.damper = 1.0f;
  m_pSphericalDesc->jointSpring.spring = 0.5f;*/

  //Projection per errors
  /*m_pSphericalDesc->projectionMode = NX_JPM_POINT_MINDIST;
  m_pSphericalDesc->projectionDistance = 0.15f;*/

  m_pSphericalDesc->setGlobalAnchor(pos);
  m_pSphericalDesc->setGlobalAxis(axis);


}


void CPhysicSphericalJoint::SetInfoRagdoll	(SSphericalLimitInfo _sInfo, CPhysicActor* actorA,  CPhysicActor* actorB)
{
  if (actorA==NULL)
	{
		LOGGER->AddNewLog(ELL_ERROR, "PhysicSphericalJoint:: El primer actor pasado como argumento no puede ser null");
		return;
	}

  NxVec3 pos(_sInfo.m_vAnchor.x, _sInfo.m_vAnchor.y, _sInfo.m_vAnchor.z );
	NxVec3 axis(_sInfo.m_vAxis.x, _sInfo.m_vAxis.y, _sInfo.m_vAxis.z );

	m_pSphericalDesc->setToDefault();

	m_pSphericalDesc->actor[0] = actorA->GetPhXActor();
	if (actorB!=NULL)
	{
		m_pSphericalDesc->actor[1] = actorB->GetPhXActor();	
	}
	else
	{
		m_pSphericalDesc->actor[1] = NULL;	
	}


 
  //LIMITS PELS TWIST!!!!!!! (gir de munyeca)
  if (_sInfo.TwistLimit)
  {
    m_pSphericalDesc->flags |= NX_SJF_TWIST_LIMIT_ENABLED;
    m_pSphericalDesc->twistLimit.low.value = _sInfo.TwistLowValue*NxPi;
    m_pSphericalDesc->twistLimit.low.restitution = _sInfo.TwistLowRestitution;
    m_pSphericalDesc->twistLimit.high.value = _sInfo.TwistHighValue*NxPi;
    m_pSphericalDesc->twistLimit.high.restitution = _sInfo.TwistHighRestitution;

    m_pSphericalDesc->twistLimit.low.hardness = 0.5f;
    m_pSphericalDesc->twistLimit.high.hardness = 0.5f;

  }

  //Es pot push pero al retornar, com mes petit es el valor, menys espai recorre.
  if (_sInfo.SwingLimit)
  {
    m_pSphericalDesc->flags |= NX_SJF_SWING_LIMIT_ENABLED;
    m_pSphericalDesc->swingLimit.value = _sInfo.SwingValue*NxPi;
    m_pSphericalDesc->swingLimit.restitution = _sInfo.SwingRestitution;
    m_pSphericalDesc->swingLimit.hardness = 0.5f;
  }

  //Twist Spring Enabled
  if (_sInfo.TwistSpring)
  {
    m_pSphericalDesc->flags |= NX_SJF_TWIST_SPRING_ENABLED;
    m_pSphericalDesc->twistSpring.damper = _sInfo.TwistSpringDamper;
    m_pSphericalDesc->twistSpring.spring = _sInfo.TwistSpringValue;
  }

  //Swing Spring Enabled
  if (_sInfo.SwingSpring)
  {
    m_pSphericalDesc->flags |= NX_SJF_SWING_SPRING_ENABLED;
    m_pSphericalDesc->swingSpring.damper = _sInfo.SwingSpringDamper;
    m_pSphericalDesc->swingSpring.spring = _sInfo.SwingSpringValue;
  }
  
  //Joint Springs
  if (_sInfo.JointSpring)
  {
    m_pSphericalDesc->flags |= NX_SJF_JOINT_SPRING_ENABLED;
    m_pSphericalDesc->jointSpring.damper = _sInfo.JointSpringDamper;
    m_pSphericalDesc->jointSpring.spring = _sInfo.JointSpringValue;
  }

  //TODO: Passar per parametre
  m_pSphericalDesc->projectionMode = NX_JPM_POINT_MINDIST;
  m_pSphericalDesc->projectionDistance = 0.22f;
  m_pSphericalDesc->solverExtrapolationFactor = 1.1f;
  
  //Projection per errors
  /*m_pSphericalDesc->projectionMode = NX_JPM_POINT_MINDIST;
  m_pSphericalDesc->projectionDistance = 0.15f;*/

  m_pSphericalDesc->setGlobalAnchor(pos);
  m_pSphericalDesc->setGlobalAxis(axis);

}