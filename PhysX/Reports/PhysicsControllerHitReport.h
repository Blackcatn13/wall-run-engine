#pragma once

#ifndef _PHYSICS_CONTROLLER_HIT_REPORT_H
#define _PHYSICS_CONTROLLER_HIT_REPORT_H

#include "Utils\Logger.h"
#include "Utils\BaseUtils.h"

class CPhysicsControllerHitReport : public NxUserControllerHitReport
{
	public:

	virtual NxControllerAction onShapeHit(const NxControllerShapeHit& hit)
	{
		if(hit.shape)
		{
				NxCollisionGroup group = hit.shape->getGroup();
			if ( group == ECG_DYNAMIC_OBJECTS )
			{
				NxActor& actor = hit.shape->getActor();

				if(hit.dir.y==0.0f)
				{
					NxF32 coeff = actor.getMass() * hit.length * 10.0f;
					actor.addForceAtLocalPos(hit.dir*coeff, NxVec3(0,0,0), NX_IMPULSE);
				}
			}

			if ( group == ECG_PLAYER )
			{
				#if defined _DEBUG
					LOGGER->AddNewLog( ELL_INFORMATION, "onShapeHit -> Hit ECG_PLAYER" );
				#endif	
				return NX_ACTION_NONE;
			}

			if ( group == ECG_ENEMY )
			{
				#if defined _DEBUG
					LOGGER->AddNewLog( ELL_INFORMATION, "onShapeHit -> Hit ECG_ENEMY" );
				#endif	
				return NX_ACTION_NONE;
			}
		}

		return NX_ACTION_NONE;
	}

	virtual NxControllerAction onControllerHit(const NxControllersHit& hit)
	{
		if(hit.controller)
		{
			NxCollisionGroup group = hit.controller->getActor()->getGroup();
			if ( group == ECG_DYNAMIC_OBJECTS )
			{
				#if defined _DEBUG
					LOGGER->AddNewLog( ELL_INFORMATION, "onControllerHit -> Hit ECG_DYNAMIC_OBJECTS" );
				#endif	
				return NX_ACTION_NONE;
			}

			if ( group == ECG_PLAYER )
			{
				#if defined _DEBUG
					LOGGER->AddNewLog( ELL_INFORMATION, "onControllerHit -> Hit ECG_PLAYER" );
				#endif	
				return NX_ACTION_NONE;
			}

			if ( group == ECG_ENEMY )
			{
				#if defined _DEBUG
					LOGGER->AddNewLog( ELL_INFORMATION, "onControllerHit -> Hit ECG_ENEMY" );
				#endif	
				return NX_ACTION_NONE;
			}
		}

		return NX_ACTION_NONE;
	}
};

#endif