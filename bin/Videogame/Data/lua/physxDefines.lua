EUserDataFlag = {
	UD_IS_HW_ACTOR 				= 0,
    UD_IS_HW_CONTROLLER			= 1,
    UD_IS_TRIGGER 				= 2,
    UD_IS_INSIDE_TRIGGER 		= 3,            
    UD_PASSES_INTERSECTION_TEST = 4,    
    UD_HIT_BY_RAYCAST 			= 5,               
    UD_NO_RENDER 				= 6,         
    UD_IS_DRAIN 				= 7,             
    UD_IS_ASLEEP 				= 8,			   
    UD_RENDER_USING_LIGHT1 		= 9	
}

ECollisionGroup = {
    ECG_ESCENE				= 0,
    ECG_PLAYER				= 1,
    ECG_ENEMY				= 2,
    ECG_TRIGGERS			= 3,
    ECG_MALGLOBAL			= 4,
    ECG_COBERTURES 			= 5,
    ECG_DYNAMIC_OBJECTS		= 6,
    ECG_STATIC_OBJECTS		= 7,
    ECG_EXPLOSIONS			= 8,
    ECG_RAY_SHOOT			= 9,
    ECG_RAY_SHOOT_PLAYER	=10,
    ECG_FORCE				=11,
    ECG_RAY_IA_GRAPH		=12,
    ECG_CAMERA				=13,
    ECG_RAGDOLL				=14,
    ECG_RAGDOLL_PLAYER		=15,
    ECG_VIGIA				=16,
    ECG_LIMITS				=17,			--Representa muros o límites del escenario por donde el player no debería poder cruzar
	ECG_LAST_GROUP			=18
}

EControleType = {
    BOX		= 0,
    CAPSULE = 1
}
