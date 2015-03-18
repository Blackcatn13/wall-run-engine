
function init_level(a)
	local coreInstance = CCoreLuaWrapper().m_CoreInstance;
	local player = Player:get_instance()
	player.get_player_controller():set_position(Vect3f(0.432929, 3.000000, -0.654716));
	
	--local lua_player = Player:get_instance()
	--[[player.num_lives = player.num_lives -1
	coreInstance:trace(tostring(player.num_lives))
	local player2 = Player:get_instance()	
	coreInstance:trace(tostring(player2.num_lives))
	--]]
	init_platform("StaticPlatform001","Static001UserData", Vect3f(2,0.5,1.5), Vect3f(0,0.5,0))
	init_platform("StaticPlatform002","Static002UserData", Vect3f(2,0.5,1.5), Vect3f(0,0.5,0))	
	init_platform("StaticPlatform003","Static003UserData", Vect3f(2,0.5,1.5), Vect3f(0,0.5,0))		
	init_platform("StaticPlatform004","Static004UserData", Vect3f(1.8,0.3,1.5), Vect3f(0,0.5,0))	
	
	init_platform("BreakablePlatform001","Breakable001UserData", Vect3f(2,0.5,1.5), Vect3f(0,0.5,0))
	init_move_platform("MovingPlatform001","Moving001UserData", Vect3f(2,0.5,2), Vect3f(0,0.8,0))
	init_platform("Spikes001","Spikes001UserData", Vect3f(12,0.5,16), Vect3f(0,0.8,0))
	init_poly_platform("PolyPlatform001","Poly001UserData", Vect3f(2,2,2), Vect3f(0,0.8,0), 3.0)
	init_poly_platform("PolyPlatform002","Poly002UserData", Vect3f(2,0.5,2), Vect3f(0,0.8,0), 3.0)
	--breakable_platform1 = init_breakable_platform("Box004", "Box004UserData", Vect3f(2,0.5,2), Vect3f(0,0.5,0), "BreakablePlatform_test")	
	
	return 0
end

