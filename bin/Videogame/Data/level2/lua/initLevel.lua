
function init_level(a)
	init_platform("StaticPlatform001","Box003UserData", Vect3f(2,0.5,2), Vect3f(0,0.8,0))	
	init_platform("BreakablePlatform001","Breakable001UserData", Vect3f(2,0.5,2), Vect3f(0,0.8,0))
	--breakable_platform1 = init_breakable_platform("Box004", "Box004UserData", Vect3f(2,0.5,2), Vect3f(0,0.5,0), "BreakablePlatform_test")	
	
	return 0
end

function activate_trigger_update(trigger_name)
	local coreInstance = CCoreLuaWrapper().m_CoreInstance;
	local trigger_manager = coreInstance:get_trigger_manager()
	local trigger = trigger_manager:get_resource(trigger_name)
	trigger.m_Update=true

end 