//----------------------------------------------------------------------------------
// CPhysicTrigger class
// Author: Enric Vergara
//
// Description:
// Clase abstracta la cual ofrece la interfaz para poder controlar los eventos de entrar/salir
// de los triggers contenidos en la escena
//----------------------------------------------------------------------------------
#pragma once

#ifndef __PHYSIC_TRIGGER_REPORT_CLASS_H__
#define __PHYSIC_TRIGGER_REPORT_CLASS_H__

//---Engine Includes---//
#undef min
#undef max
#include "NxPhysics.h"
#include "NxShape.h"
//---------------------//

class CPhysicUserData;

class CPhysicTriggerReport : public NxUserTriggerReport 
{
public:
	//--- Init and End protocols------------------------------------------
					CPhysicTriggerReport	( void ) {}
	virtual			~CPhysicTriggerReport	( void ) {}


	//--- NxUserTriggerReport Interface	----------------------------------
	void			onTrigger	( NxShape &_TriggerShape, NxShape &_OtherShape, NxTriggerFlag _Status );
	
	//--- CPhysicTrigger Interface ---------------------------------------
	virtual void	OnEnter		( CPhysicUserData* _Entity_Trigger1, CPhysicUserData* _Other_Shape) = 0;
	virtual	void	OnLeave		( CPhysicUserData* _Entity_Trigger1, CPhysicUserData* _Other_Shape) = 0;
	virtual	void	OnStay		( CPhysicUserData* _Entity_Trigger1, CPhysicUserData* _Other_Shape) = 0;
};

#endif __PHYSIC_TRIGGER_REPORT_CLASS_H__