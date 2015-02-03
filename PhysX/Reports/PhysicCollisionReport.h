#pragma once

#ifndef _PHYSIC_COLLISION_REPORT_H
#define _PHYSIC_COLLISION_REPORT_H

//---Engine Includes---//
#include "NxPhysics.h"
//---------------------//

class CPhysicUserData;

class CPhysicCollisionReport : public NxUserContactReport
{
public:
	//--- Init and End protocols------------------------------------------
					CPhysicCollisionReport	( void ) {}	
	virtual			~CPhysicCollisionReport	( void ) {}

	//--- NxUserContactReport Interface	----------------------------------
	void			onContactNotify	( NxContactPair& _Pair, NxU32 _Events );
	
	//--- CPhysicContactReport Interface ---------------------------------------
	virtual void	OnStartTouch ( CPhysicUserData* _pEntity, CPhysicUserData* _pOther_shape )					= 0;
	virtual	void	OnTouch ( CPhysicUserData* _pEntity, CPhysicUserData* _pOther_shape )						= 0;
	virtual	void	OnEndTouch ( CPhysicUserData* _pEntity, CPhysicUserData* _pOther_shape )					= 0;
	virtual void	OnStartTouchForceThreshold ( CPhysicUserData* _pEntity, CPhysicUserData* _pOther_shape )	= 0;
	virtual	void	OnTouchForceThreshold ( CPhysicUserData* _pEntity, CPhysicUserData* _pOther_shape )			= 0;
	virtual	void	OnEndTouchForceThreshold ( CPhysicUserData* _pEntity, CPhysicUserData* _pOther_shape )		= 0;
};

#endif
