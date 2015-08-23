local coreInstance = CCoreLuaWrapper().m_CoreInstance;
function on_update_scripted_controller(l_ElapsedTime)
--	local name = tostring(l_ElapsedTime);
	
	local deltaX, deltaY, deltaZ;
	local xMin = 5.0;
	local xMax = 7.0;
	local zMin = -3.4;
	local zMax = 0.7;
	local m_Speed = 10;
	deltaX = coreInstance:get_input_manager():get_mouse_delta().x;
	deltaY = coreInstance:get_input_manager():get_mouse_delta().y;
	deltaZ = coreInstance:get_input_manager():get_mouse_delta().z;

	local luaUtil = CCMathLuaUtils();
	
	local cam_Controller = coreInstance.m_CameraController;
	local cin_Controller = coreInstance:get_cinematic_controller();
	local active_camera = cam_Controller:get_active_camera();
	local type_camera = active_camera.m_eTypeCamera;
	local camObject = active_camera.m_pObject3D;
	
	local act2in = coreInstance:get_action_to_input();
	if player.can_move then
		if act2in:do_action_from_lua("FlyMode") == true then
			cam_Controller.m_FlyMode = not cam_Controller.m_FlyMode;
		end
		if type_camera == CCamera.TC_FPS then
			if act2in:do_action_from_lua("yaw", deltaX) then
				camObject:set_yaw(camObject:get_yaw() - deltaX * l_ElapsedTime);
			end
			if act2in:do_action_from_lua("pitch", deltaY) then
				camObject:set_pitch(camObject:get_pitch() - deltaY * l_ElapsedTime);
			end
			active_camera.m_fAspectRatio = 16/9;
			local yaw = camObject:get_yaw();
			local dir = active_camera:get_direction();
			local nor = Vect3f(math.cos(yaw + (math.pi/2)), 0, (math.sin(yaw + (math.pi/2))));
			nor = luaUtil:normalize(nor);
			if cam_Controller.m_FlyMode ~= true then
				dir.y = 0;
				nor.y = 0;
			end
			
			if act2in:do_action_from_lua("MoveForward") then
				camObject:set_position(camObject:get_position() + dir * cam_Controller.m_Speed *l_ElapsedTime)
			end
			if act2in:do_action_from_lua("MoveBack") then
				camObject:set_position(camObject:get_position() - dir * cam_Controller.m_Speed *l_ElapsedTime)
			end
			if act2in:do_action_from_lua("MoveRigth") then
				camObject:set_position(camObject:get_position() - nor * cam_Controller.m_Speed *l_ElapsedTime)
			end
			if act2in:do_action_from_lua("MoveLeft") then
				camObject:set_position(camObject:get_position() + nor * cam_Controller.m_Speed *l_ElapsedTime)
			end	
		end
		if type_camera == CCamera.TC_THPS and not gui_manager:get_is_displayed_console() then
			if act2in:do_action_from_lua("yaw", deltaX) then
				camObject:set_yaw(camObject:get_yaw() + deltaX * l_ElapsedTime);
			end
			if act2in:do_action_from_lua("pitch", deltaY) then
				local new_pitch = camObject:get_pitch() + deltaY * l_ElapsedTime
				if  new_pitch > -1.0 and new_pitch <0.5 then
					camObject:set_pitch(new_pitch);
				end
			end
			active_camera.m_fAspectRatio = 16/9;
			local yaw = camObject: get_yaw();

			local dir = active_camera:get_direction();
			local nor = Vect3f(math.cos(yaw + (math.pi/2)), 0, (math.sin(yaw + (math.pi/2))));
			nor = luaUtil:normalize(nor);
			if cam_Controller.m_FlyMode ~= true then
				dir.y = 0;
				nor.y = 0;
			end
			
			if act2in:do_action_from_lua("MoveForward") then
				camObject:set_position(camObject:get_position() + dir * cam_Controller.m_Speed *l_ElapsedTime *0.1)
			end
			if act2in:do_action_from_lua("MoveBack") then
				camObject:set_position(camObject:get_position() - dir * cam_Controller.m_Speed *l_ElapsedTime *0.1)
			end
			if act2in:do_action_from_lua("MoveRigth") then
				camObject:set_position(camObject:get_position() - nor * cam_Controller.m_Speed *l_ElapsedTime)
			end
			if act2in:do_action_from_lua("MoveLeft") then
				camObject:set_position(camObject:get_position() + nor * cam_Controller.m_Speed *l_ElapsedTime)
			end	
		end
		
		if type_camera == CCamera.TC_ESF then
				
			if act2in:do_action_from_lua("scrollLittle", deltaZ) then
				active_camera:add_zoom(-deltaZ * cam_Controller.m_LittleZoom * l_ElapsedTime)
			elseif act2in:do_action_from_lua("scroll", deltaZ) then
				active_camera:add_zoom(-deltaZ * cam_Controller.m_BigZoom * l_ElapsedTime)			
			end
			active_camera.m_fAspectRatio = 16/9;
			local yaw = camObject: get_yaw();
			
			local dir = active_camera:get_direction();
			local norX = Vect3f(math.cos(yaw + (math.pi/2)), 0, (math.sin(yaw + (math.pi/2))));
			norX = luaUtil:normalize(norX);
			local norY = Vect3f(math.sin(yaw + (math.pi/2)), 0, -(math.cos(yaw + (math.pi/2))));
			norY = luaUtil:normalize(norY);
			local panX, panY;
			panX = deltaX;
			panY = deltaY;
			if act2in:do_action_from_lua("PanX", panX) then
				camObject:set_position(camObject:get_position() +  norX * panX * cam_Controller.m_PanSpeed *l_ElapsedTime)
			end
			if act2in:do_action_from_lua("PanY", panY) then
				camObject:set_position(camObject:get_position() + norY * panY * cam_Controller.m_PanSpeed *l_ElapsedTime)
			end
			if act2in:do_action_from_lua("RotX", panX) then
				camObject:set_yaw(camObject:get_yaw() - panX * l_ElapsedTime);
			end
			if act2in:do_action_from_lua("RotY", panY) then
				camObject:set_pitch(camObject:get_pitch() - panY * l_ElapsedTime);
			end
		end
			
		if type_camera == CCamera.TC_CIN then
			cam_Controller:cam_updates(type_camera, l_ElapsedTime);
		end
		
		--local type_camera2 = active_camera.m_eTypeCamera;
	--[[	if camObject:get_position().x >= xMin and camObject:get_position().x <= xMax and camObject:get_position().z >= zMin and camObject:get_position().z <= zMax then
			--Aqui launcher de cinematica 2
			local name = "Tete"
			local n1 = CNamed();
			--n1:set_name(name);
			
			cam_Controller:set_active_camera("Camera002");
			cam_Controller:play(true);
			cin_Controller:get_resource("DestroyW"):play(true);
			
		end]]
	end
	
	return 0;
end
