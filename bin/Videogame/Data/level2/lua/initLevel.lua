
function init_level(a)
	--[[init_platform("StaticPlatform001","Box003UserData", Vect3f(2,0.5,2), Vect3f(0,0.8,0))	
	init_platform("BreakablePlatform001","Breakable001UserData", Vect3f(2,0.5,2), Vect3f(0,0.8,0))
	init_platform("MovingPlatform001","Moving001UserData", Vect3f(2,0.5,2), Vect3f(0,0.8,0))
	init_platform("PinchosPlatform001","Pinchos001UserData", Vect3f(2,0.5,2), Vect3f(0,0.8,0))
	init_poly_platform("PolyPlatform001","Poly001UserData", Vect3f(2,0.5,2), Vect3f(0,0.8,0), false, 3.0)
	--breakable_platform1 = init_breakable_platform("Box004", "Box004UserData", Vect3f(2,0.5,2), Vect3f(0,0.5,0), "BreakablePlatform_test")	
	--]]
	local player = Player:get_instance()
	player.get_player_controller():set_position(Vect3f(0.432929, 3.000000, -0.654716));
	local enemy = Enemy.new(Vect3f(-3.0,2.0,10.0), "MikMik")	
	set_enemy(enemy)
	return 0
end

function activate_trigger_update(trigger_name)
	local coreInstance = CCoreLuaWrapper().m_CoreInstance;
	local trigger_manager = coreInstance:get_trigger_manager()
	local trigger = trigger_manager:get_resource(trigger_name)
	trigger.m_Update=true

end 