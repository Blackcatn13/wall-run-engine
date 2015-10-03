local current_shot_type = 0  --0: Normal, 1:powerup


function start_boss()
	set_player_room("0", true)
	cam_Controller:set_active_camera("BossCam")
	local activeCam = cam_Controller:get_active_camera()
	
	local position = Vect3f(30,1,2)
	player_controller.m_PhysicController:set_position(position)
	local playerRenderableMesh = renderable_objects_layer_manager:get_renderable_objects_manager_by_str_and_room("player", 0):get_resource(piky_mesh_name);
	playerRenderableMesh:set_position(position)
	
	local chucky = enemy_manager:get_enemy(boss_mesh_name)
	local chucky_position = Vect3f(0.0, 2.48, 0.0)
	chucky:set_position(chucky_position)
	chucky.m_RenderableObject:set_position(chucky_position)
	chucky.m_RenderableObject:set_visible(true)
	local objCam = activeCam.m_pObject3D;
	objCam:set_position(chucky_position);
	local cadira = get_renderable_object("solid",0, "CADIRA")
	cadira:set_visible(false)
	set_boss_polis_visible(true)
	chucky.m_BossRunning = true
end

function set_boss_polis_visible(visible)
	local poly1 = renderable_objects_layer_manager:get_resource_from_layers_and_room("poly", "enabled_poly", "Poly_Sala0_001", 0)
	local poly2 = renderable_objects_layer_manager:get_resource_from_layers_and_room("poly", "enabled_poly", "Poly_Sala0_002", 0)
	local poly3 = renderable_objects_layer_manager:get_resource_from_layers_and_room("poly", "enabled_poly", "Poly_Sala0_003", 0)
	local poly4 = renderable_objects_layer_manager:get_resource_from_layers_and_room("poly", "enabled_poly", "Poly_Sala0_004", 0)
	local array_polys = {poly1, poly2, poly3, poly4}
	
	
	for i = 1, table.getn(array_polys) do
		array_polys[i]:set_visible(visible)
		array_polys[i].m_Printable = visible
	end
end

---- IA----
----PARADO----
function chucky_boss_enter_stopped(name)
	local enemy = enemy_manager:get_enemy(name)
	enemy.m_RenderableObject:blend_cycle(0,1,0);
end

function chucky_boss_exit_stopped(name)
	
end

function chucky_boss_update_stopped(ElapsedTime, doComprobation, name)
	local chucky = enemy_manager:get_enemy(name)
	local player_position = player_controller:get_position()
	local player_distance = get_distance_to_player(chucky:get_position(), player_position)
	if player_distance < 1000 then
		rotate_yaw(chucky, ElapsedTime, player_position)
	end
end

----DISPARAR----
function chucky_boss_enter_shoot(name)
	
end

function chucky_boss_exit_shoot(name)
	
end

function chucky_boss_update_shoot(ElapsedTime, doComprobation, name)

end

----LANZAR----
function chucky_boss_enter_shoot(name)
	
end

function chucky_boss_exit_shoot(name)
	
end

function chucky_boss_update_shoot(ElapsedTime, doComprobation, name)

end

----DEVOLVER----
function chucky_boss_enter_return(name)
	
end

function chucky_boss_exit_return(name)
	
end

function chucky_boss_update_return(ElapsedTime, doComprobation, name)

end

----LLAMAR----
function chucky_boss_enter_call(name)
	
end

function chucky_boss_exit_call(name)
	
end

function chucky_boss_update_call(ElapsedTime, doComprobation, name)

end

----HURT----
function chucky_boss_enter_hurt(name)
	
end

function chucky_boss_exit_hurt(name)
	
end

function chucky_boss_update_hurt(ElapsedTime, doComprobation, name)

end

----DEAD----
function chucky_boss_enter_dead(name)
	
end

function chucky_boss_exit_dead(name)
	
end

function chucky_boss_update_dead(ElapsedTime, doComprobation, name)

end
