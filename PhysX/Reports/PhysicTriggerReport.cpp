#define __DONT_INCLUDE_MEM_LEAKS__

#include "Utils\BaseUtils.h"
#include "Reports\PhysicTriggerReport.h"
#include "Utils\PhysicUserData.h"

void CPhysicTriggerReport::onTrigger ( NxShape &_TriggerShape, NxShape &_OtherShape, NxTriggerFlag _Status ) {
  // un cos a entrat en l'area del trigger per primer cop
  if ( _Status & NX_TRIGGER_ON_ENTER ) {
    // agafo el actor trigger
    NxActor &l_TriggerActor = _TriggerShape.getActor();
    CPhysicUserData *l_Entity_Trigger1 = ( CPhysicUserData *) l_TriggerActor.userData;
    // agafo l'actor que entra dins el trigger
    NxActor &l_Actor = _OtherShape.getActor();
    CPhysicUserData *l_Entity_Trigger2 = ( CPhysicUserData *) l_Actor.userData;

    // Cridem el mètode per establir l'execució del que passarà
    if ( l_Entity_Trigger1->GetMyCollisionGroup() == l_Entity_Trigger2->GetMyCollisionGroup())
      OnEnter ( l_Entity_Trigger1, l_Entity_Trigger2 );
  }
  if ( _Status & NX_TRIGGER_ON_LEAVE ) {
    // agafo el actor trigger
    NxActor &l_TriggerActor = _TriggerShape.getActor();
    CPhysicUserData *l_Entity_Trigger1 = ( CPhysicUserData *) l_TriggerActor.userData;
    // agafo l'actor que surt de dins el trigger
    NxActor &l_Actor = _OtherShape.getActor();
    CPhysicUserData *l_Entity_Trigger2 = ( CPhysicUserData *) l_Actor.userData;
    // L'actor deixa el l'area del trigger
    if ( l_Entity_Trigger1->GetMyCollisionGroup() == l_Entity_Trigger2->GetMyCollisionGroup())
      OnLeave ( l_Entity_Trigger1, l_Entity_Trigger2 );
  }
  if ( _Status & NX_TRIGGER_ON_STAY ) {
    // agafo el actor trigger
    NxActor &l_TriggerActor = _TriggerShape.getActor();
    CPhysicUserData *l_Entity_Trigger1 = ( CPhysicUserData *) l_TriggerActor.userData;
    // agafo l'actor que surt de dins el trigger
    NxActor &l_Actor = _OtherShape.getActor();
    CPhysicUserData *l_Entity_Trigger2 = ( CPhysicUserData *) l_Actor.userData;
    // L'actor es manté dins l'area
    if ( l_Entity_Trigger1->GetMyCollisionGroup() == l_Entity_Trigger2->GetMyCollisionGroup())
      OnStay ( l_Entity_Trigger1, l_Entity_Trigger2 );
  }
}

