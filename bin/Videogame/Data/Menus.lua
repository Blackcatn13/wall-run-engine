
function set_screen_menu(model_position, room )
	-- room 9 = Game Over
	-- room 10 = Menu principal
	-- room 11 = Creditos

	player_controller.m_PhysicController:set_position(model_position)

	player.can_move = false -- IMPEDIR QUE EL PLAYER SE MUEVA
	
	if room == 9 then --GAME OVER
		cam_Controller:set_active_camera("GUICamera")
		gui_manager:set_visible_gui_element("LifeGUI", false)
		gui_manager:set_visible_gui_element("VidesGUI", false)
		local position = Vect3f(model_position.x, model_position.y + 2, model_position.z + 2)
		--player_controller:set_yaw(3.14) --180º en teoria
		set_cam_position(position, -1.57) --  -90º en teoria 
		set_player_room(9, true)
		local playerRenderableMesh = renderable_objects_layer_manager:get_renderable_objects_manager_by_str_and_room(piky_layer, room):get_resource(piky_mesh_name);
		player_controller:set_position(model_position)
		playerRenderableMesh:set_position(model_position)
		-- AMIMACION DE GAME OVER
		--playerRenderable:remove_action(10)
		playerRenderableMesh:execute_action(8,0,0.3,1,true)
	elseif room == 10 then --GUI MENU
		cam_Controller:set_active_camera("GUICamera")
		--local position = Vect3f(model_position.x - 3, model_position.y, model_position.z + 5)
		--player_controller:set_yaw(3.14) --180º en teoria
		set_cam_position(Vect3f(-925,2,3.5), -1.57) --  -90º en teoria
		set_player_room(10, true)
		local playerRenderableMesh = renderable_objects_layer_manager:get_renderable_objects_manager_by_str_and_room(piky_layer, room):get_resource(piky_mesh_name);
		playerRenderableMesh:set_position(model_position);
		playerRenderableMesh:set_yaw(3.14-0.3140)
		player_controller:set_position(model_position)
		
		-- AMIMACION DE CORRER
		--playerRenderableMesh:execute_action(1,0,0.3,1,true)
		--playerRenderableMesh:blend_cycle(1,1,0.5);
		
		-- CHUCKY
		local Chucky = enemy_manager:get_enemy("ChuckyMenu");
		--Chucky:move_to_position(Vect3f(-922.2, 6, -1.5));
		--local chucky_renderable = Chucky.m_RenderableObject
		--chucky_renderable:set_position(Vect3f(-922.2, 10, -1.5));
		Chucky.m_RenderableObject:set_yaw(3.14-0.3140);
		Chucky:set_visible(true)
		--coreInstance:trace("Sphere actor position:" .. tostring(renderable_objects_layer_manager:get_renderable_objects_manager_by_str_and_room("solid", room):get_resource("MenuSphere").m_Actor:get_position().x))
	end
	
end

function set_cam_position(cam_position, yaw)
	
	local active_camera = cam_Controller:get_active_camera()
	--[[local dir3D = active_camera:get_direction()
	local camObject = active_camera.m_pObject3D
	local dirYaw = camObject:get_yaw()
	local PlayerYaw =  - dirYaw + 1.57 --]]	
	active_camera.m_pObject3D:set_position(cam_position)
	active_camera.m_pObject3D:set_yaw(yaw)	
end


