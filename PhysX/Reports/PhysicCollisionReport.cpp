#include "Reports\PhysicCollisionReport.h"

void  CPhysicCollisionReport::onContactNotify(NxContactPair& _Pair, NxU32 _Events)
{
	if ( _Pair.isDeletedActor [0] || _Pair.isDeletedActor [1] )
		return;

	NxActor* l_Actor = _Pair.actors[0];
	CPhysicUserData* l_EntityCollision1 = ( CPhysicUserData* ) l_Actor->userData;  

	l_Actor = _Pair.actors[1];
	CPhysicUserData* l_EntityCollision2	= ( CPhysicUserData* ) l_Actor->userData;

	if ( _Events & NX_NOTIFY_ON_START_TOUCH)           
	{                           
		OnStartTouch ( l_EntityCollision1, l_EntityCollision2 );
	}            
  if ( _Events & NX_NOTIFY_ON_END_TOUCH)            
	{                
		OnEndTouch ( l_EntityCollision1, l_EntityCollision2 );         
	}
  if ( _Events & NX_NOTIFY_ON_TOUCH)            
	{                
		OnTouch ( l_EntityCollision1, l_EntityCollision2 );         
	}
  if ( _Events & NX_NOTIFY_ON_START_TOUCH_FORCE_THRESHOLD)           
	{                           
		OnStartTouchForceThreshold( l_EntityCollision1, l_EntityCollision2 );
	}            
  if ( _Events & NX_NOTIFY_ON_END_TOUCH_FORCE_THRESHOLD)            
	{                
		OnEndTouchForceThreshold( l_EntityCollision1, l_EntityCollision2 );         
	}
  if ( _Events & NX_NOTIFY_ON_TOUCH_FORCE_THRESHOLD)            
	{                
		OnTouchForceThreshold( l_EntityCollision1, l_EntityCollision2 );         
	}
}