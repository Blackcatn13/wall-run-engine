function start_boss()
	set_player_room("0", true)
	cam_Controller:set_active_camera("ThPS")
	local position = Vect3f(30,1,2)
	local cam_position = Vect3f(position.x+2, position.y + 3, position.z )

	set_cam_position(cam_position, -math.pi) --  180ยบ en teoria 
	cam_Controller:get_active_camera().m_pObject3D:set_pitch(-0.5)	
	
	player_controller.m_PhysicController:set_position(position)
	local playerRenderableMesh = renderable_objects_layer_manager:get_renderable_objects_manager_by_str_and_room("player", 0):get_resource(piky_mesh_name);
	playerRenderableMesh:set_position(position)
	
	local chucky = enemy_manager:get_enemy("Chucky").m_RenderableObject
	chucky:set_position(Vect3f(12.30, 0.00, -2.75))
	
	set_boss_polis_visible(true)
	boss_started = true
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
