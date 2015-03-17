--Crear instancias de camaras
--[[variables
	m_3DCamera
	m_2DCamera
]]
function on_init_cameras_lua()
	local coreInstance = CCoreLuaWrapper().m_CoreInstance;
	local player = coreInstance:get_player_controller();
	local cam = coreInstance.m_CameraController:get_resource("3DCam");
	local fWP = cam:get_path_point(0);
	player.m_PhysicController:set_position(fWP);
	player:set_position(player.m_PhysicController:get_position());
	move_character_controller_mesh(player, player:get_position());
	cam.m_lastPlayerPos = player:get_position();
end
function on_update_cameras_lua(l_ElapsedTime)
	local coreInstance = CCoreLuaWrapper().m_CoreInstance;
	local act2in = coreInstance:get_action_to_input();
	local camController = coreInstance.m_CameraController;
	if act2in:do_action_from_lua("Set3DCamera") then
		coreInstance.m_CameraController:set_active_camera("3DCam");
	end
	if act2in:do_action_from_lua("Set2DCamera") then
		coreInstance.m_CameraController:set_active_camera("2DCam");
	end
	--local name2=CNamed();
	--name2:set_name("UpdatePass1");
	local cam = camController:get_active_camera();
	local currentWP = cam:get_path_point(cam.m_currentWaypoint);
	local nextWP = cam:get_path_point(cam.m_nextWaypoint);
	--name2:set_name("UpdatePass2");
	--local lastPlayerPos = cam.m_lastPlayerPos;
	local pCont = coreInstance:get_player_controller();
	local currentPlayerPos = pCont:get_position();
	local playerVec = currentPlayerPos - currentWP;
	local playerVecN = playerVec;
	local tratar = 1;
	if(playerVecN.x==0)then
		if(playerVecN.y==0)then
			if(playerVecN.z==0)then
				tratar = 0;
			end
		end
	end
	if (tratar == 1) then
		playerVecN:normalize(1);
	end
	local cameraVec = nextWP - currentWP;
	local cameraVecN = cameraVec;
	if(cameraVecN.x==0)then
		if(cameraVecN.y==0)then
			if(cameraVecN.z==0)then
				tratar = 0;
			end
		end
	end
	if (tratar == 1) then
		cameraVecN:normalize(1);
	end
	local dot = 0;
	if(tratar == 1) then
		dot = playerVecN * cameraVecN;
	end
	local modul = playerVecN:length();
	local moviment = dot * modul;
	local newPos = currentWP + (cameraVecN * moviment);
	--Update Camera 3D
	if(cam.m_eTypeCamera == 6) then
		local obj = cam.m_pObject3D;
		local name = CNamed();
		name:set_name("LuaBreak");
		obj:set_position(Vect3f(newPos.x, newPos.y, newPos.z));
		local yaw = math.atan(cameraVecN.z, cameraVecN.x);
		obj:set_yaw(yaw);
		obj:set_pitch(-0.25);
		obj:set_roll(0);
		cam:set_zoom(25);
		cam.m_fZNear = 0.1;
		cam.m_fZFar = 1000;
		--cam.m_fFOV = 45.0 * 3.1415 / 180;
		--cam.m_fAspectRatio = 1;
	end
	--Update Camera 2D
	if(cam.m_eTypeCamera == 5) then
		local positionController = pCont:get_position();
		local obj = cam.m_pObject3D;
		obj:set_position(Vect3f(0, 1, positionController.z));
		obj:set_yaw(3.1415);
		obj:set_pitch(-0.1);
		obj:set_roll(0);
		cam:set_zoom(50);
		cam.m_fZNear = 0.1;
		cam.m_fZFar = 1000;
		--cam.m_fFOV = 45.0 * 3.1415 / 180;
		--cam.m_fAspectRatio = 1;
	end
	--cam.m_lastPlayerPos = currentPlayerPos;
	local camObj = cam.m_pObject3D;
	local objPos = camObj:get_position();
	local distanceToWP = objPos:distance(nextWP);
	if(distanceToWP < 0.2) then
		if(cam.m_nextWaypoint < cam:get_path_size()) then
			cam.m_currentWaypoint = cam.m_nextWaypoint;
			cam.m_nextWaypoint = cam.m_nextWaypoint + 1;
		end
	end
	--name2:set_name("UpdatePassFinal");
end