
function set_screen_menu(model_position, room )
	-- room 9 = Game Over
	-- room 10 = Menu principal
	-- room 11 = Creditos

	player_controller.m_PhysicController:set_position(model_position)
	
	cam_Controller:set_active_camera("ThPS")
	gui_manager:set_is_displayed_console(true) -- IMPEDIR QUE EL PLAYER SE MUEVA
	
	if room == 9 then --GAME OVER
		gui_manager:set_visible_gui_element("LifeGUI", false)
		gui_manager:set_visible_gui_element("VidesGUI", false)
		local position = Vect3f(model_position.x, model_position.y + 2, model_position.z + 5)
		--player_controller:set_yaw(3.14) --180º en teoria
		set_cam_position(position, -1.57) --  -90º en teoria 
		set_player_room(9, false)
		local playerRenderableMesh = renderable_objects_layer_manager:get_renderable_objects_manager_by_str_and_room("player", room):get_resource("Piky");
		playerRenderableMesh:set_position(model_position)
		-- AMIMACION DE GAME OVER
		--playerRenderable:remove_action(10)
		--playerRenderable:execute_action(9,0,0.3,1,true)
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


