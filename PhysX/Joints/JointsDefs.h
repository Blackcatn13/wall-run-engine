#pragma once

#ifndef _JOINTS_DEFS_H
#define _JOINTS_DEFS_H

#include "Math\Vector3.h"

struct SSphericalLimitInfo
{
	Vect3f m_vAnchor, m_vAxis;
	bool  TwistLimit;
	float TwistLowValue, TwistLowRestitution, TwistHighValue, TwistHighRestitution;

	bool  SwingLimit;
	float SwingValue, SwingRestitution;

	bool  TwistSpring;
	float TwistSpringValue, TwistSpringDamper;

	bool  SwingSpring;
	float SwingSpringValue, SwingSpringDamper;

	bool  JointSpring;
	float JointSpringValue, JointSpringDamper;
};

#endif