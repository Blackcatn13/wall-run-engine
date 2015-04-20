
function init_level(a)
	
	
	--breakable_platform1 = init_breakable_platform("Box004", "Box004UserData", Vect3f(2,0.5,2), Vect3f(0,0.5,0), "BreakablePlatform_test")	
	--]]
	--init_poly_platform("PolyPlatform001","Poly001UserData", Vect3f(2,0.5,2), Vect3f(0,0.4,0), false, 3.0)
	local coreInstance = CCoreLuaWrapper().m_CoreInstance;
	coreInstance.m_CameraController:set_active_camera("3DCam");
	init_platform("MovingPlatform001","Moving001UserData", Vect3f(2,0.5,2), Vect3f(0,0.8,0))
	init_platform("MovingPlatform002","Moving002UserData", Vect3f(2,0.5,2), Vect3f(0,0.8,0))
	init_platform("Spikes001","Spikes001UserData", Vect3f(2.5,0.5,2.2), Vect3f(0,0.8,0))
	init_poly_platform("PolyPlatform001","Poly001UserData", Vect3f(2,0.5,2), Vect3f(0,0.8,0), 3.0, 50.0)
	init_platform("BreakablePlatform001","Breakable001UserData", Vect3f(2,0.5,1.5), Vect3f(0,0.5,0))
	--local enemy = Enemy.new(Vect3f(-3.0,2.0,10.0), "MikMik")	
	--set_enemy(enemy)
	local player = Player:get_instance()
	--player.get_player_controller():set_position(Vect3f(0.432929, 3.000000, -0.654716));
	--player.m_is3D = true;
	--coreInstance:getWWSoundManager():PlayEvent("Menu", "Music");
	--coreInstance:getWWSoundManager():PlayEvent("Torches", "Torch");
	return 0
end

function activate_trigger_update(trigger_name)
	local coreInstance = CCoreLuaWrapper().m_CoreInstance;
	local trigger_manager = coreInstance:get_trigger_manager()
	local trigger = trigger_manager:get_resource(trigger_name)
	trigger.m_Update=true

end 