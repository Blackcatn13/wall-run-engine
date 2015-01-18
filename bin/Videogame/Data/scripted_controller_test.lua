function on_update_scripted_controller(l_ElapsedTime)
--	local name = tostring(l_ElapsedTime);
	local coreInstance = CCoreLuaWrapper().m_CoreInstance;
	local deltaX, deltaY, deltaZ;
	deltaX = 0.0;
	deltaY = 0.0;
	
	local m_ObjectThPS = CObject3D(Vect3f(1, 1, 1), 0, 0, 0);
	local m_ThPSCamera = CThPSCamera(0.1, 100.0, (45.0 * 3.14 / 180.0), 1.0, m_ObjectThPS, 50);
	local camObject = m_ThPSCamera.m_pObject3D;
--[[	m_ThPSCamera.m_eTypeCamera = CCamera.TC_FPS;
	local tempType = m_ThPSCamera.m_eTypeCamera;
	]]
	local luaUtil = CCMathLuaUtils();
	local type_camera = m_ThPSCamera.m_eTypeCamera;
	local cam_Controller = CCameraController();
	cam_Controller:add_new_camera("luaCam", m_ThPSCamera);
	
	cam_Controller:set_active_camera(m_ThPSCamera);
	
	--local act2in = cam_Controller:get_action_2_input();
	local act2in = coreInstance:get_action_to_input();
	local active_camera = cam_Controller:get_active_camera();
	
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
		local yaw = camObject:get_yaw();
		local dir = active_camera:get_direction();
		local nor = Vect3f(math.cos(yaw + math.pi), 0, (math.sin(yaw + math.pi)));
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
	if type_camera == CCamera.TC_THPS then
		if act2in:do_action_from_lua("yaw", deltaX) then
			camObject:set_yaw(camObject:get_yaw() - deltaX * l_ElapsedTime);
		end
		if act2in:do_action_from_lua("pitch", deltaY) then
			camObject:set_pitch(camObject:get_pitch() - deltaY * l_ElapsedTime);
		end
		local yaw = camObject: get_yaw();

		local dir = active_camera:get_direction();
		local nor = Vect3f(math.cos(yaw + math.pi), 0, (math.sin(yaw + math.pi)));
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
	
	if type_camera == CCamera.TC_ESF then
		if act2in:do_action_from_lua("scrollLittle", deltaZ) then
			m_ActiveCamera:add_zoom(-deltaZ * cam_Controller.m_LittleZoom * l_ElapsedTime)
		end
		if act2in:do_action_from_lua("scroll", deltaZ) then
			m_ActiveCamera:add_zoom(-deltaZ * cam_Controller.m_BigZoom * l_ElapsedTime)
		end
		local panX, panY;
		if act2in:do_action_from_lua("PanX", panX) then
			camObject:set_position(camObject:get_position() +  Vect3f(0, 0, panX) * cam_Controller.m_PanSpeed *l_ElapsedTime)
		end
		if act2in:do_action_from_lua("PanY", panY) then
			camObject:set_position(camObject:get_position() +  Vect3f(panY, 0, 0) * cam_Controller.m_PanSpeed *l_ElapsedTime)
		end
		if act2in:do_action_from_lua("RotX", panX) then
			camObject:set_yaw(camObject:get_yaw() - panX * l_ElapsedTime);
		end
		if act2in:do_action_from_lua("RotY", panY) then
			camObject:set_pitch(camObject:get_pitch() - panY * l_ElapsedTime);
		end
	end
		
	cam_Controller:load(".\\Data\\level6\\cameras.xml");
	cam_Controller:cam_updates(type_camera, l_ElapsedTime);
	
	local type_camera2 = m_ThPSCamera.m_eTypeCamera;
	return 0;
end

