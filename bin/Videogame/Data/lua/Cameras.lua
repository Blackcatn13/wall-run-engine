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
	fWP.y = fWP.y+5;
	local camController = coreInstance.m_CameraController;
	local active_cam = camController:get_active_camera();
	if active_cam.m_eTypeCamera ~= 2 then
		player.m_PhysicController:set_position(fWP);
		player:set_position(player.m_PhysicController:get_position());
		move_character_controller_mesh(player, player:get_position());
		cam.m_lastPlayerPos = player:get_position();
	end
end
function on_update_cameras_lua(l_ElapsedTime)
	local coreInstance = CCoreLuaWrapper().m_CoreInstance;
	local act2in = coreInstance:get_action_to_input();
	local camController = coreInstance.m_CameraController;
	local cam = camController:get_active_camera();
	if act2in:do_action_from_lua("Set3DCamera") then
		--coreInstance.m_CameraController:set_active_camera("3DCam");
		cam.m_eTypeCamera = 6;
	end
	if act2in:do_action_from_lua("Set2DCamera") then
		--coreInstance.m_CameraController:set_active_camera("2DCam");
		cam.m_eTypeCamera = 5;
	end
	--local name2=CNamed();
	--name2:set_name("UpdatePass1");
	if cam.m_eTypeCamera ~= 2 then
		local currentWP = cam:get_path_point(cam.m_currentWaypoint);
		coreInstance:trace("currentWP");
		coreInstance:trace(tostring(currentWP.x));
		coreInstance:trace(tostring(currentWP.y));
		coreInstance:trace(tostring(currentWP.z));
		local nextWP = cam:get_path_point(cam.m_nextWaypoint);
		coreInstance:trace("nextWP");
		coreInstance:trace(tostring(nextWP.x));
		coreInstance:trace(tostring(nextWP.y));
		coreInstance:trace(tostring(nextWP.z));
		--name2:set_name("UpdatePass2");
		--local lastPlayerPos = cam.m_lastPlayerPos;
		local pCont = coreInstance:get_player_controller();
		local currentPlayerPos = pCont:get_position();
		coreInstance:trace("currentPlayerPos");
		coreInstance:trace(tostring(currentPlayerPos.x));
		coreInstance:trace(tostring(currentPlayerPos.y));
		coreInstance:trace(tostring(currentPlayerPos.z));
		local playerVec = currentPlayerPos - currentWP;
		coreInstance:trace("playerVec");
		coreInstance:trace(tostring(playerVec.x));
		coreInstance:trace(tostring(playerVec.y));
		coreInstance:trace(tostring(playerVec.z));
		local playerVecZX = playerVec;
		playerVecZX.y = 0;
		local modul = playerVecZX:length();
		coreInstance:trace("modul");
		coreInstance:trace(tostring(modul));
		local playerVecZXN = playerVecZX;
		local tratar = 1;
		if(playerVecZXN.x==0)then
			if(playerVecZXN.y==0)then
				if(playerVecZXN.z==0)then
					tratar = 0;
				end
			end
		end
		coreInstance:trace("tratar normalize playerVecZXN");
		coreInstance:trace(tostring(tratar));
		if (tratar == 1) then
			playerVecZXN:normalize(1);
		end
		local cameraVec = nextWP - currentWP;
		coreInstance:trace("cameraVec");
		coreInstance:trace(tostring(cameraVec.x));
		coreInstance:trace(tostring(cameraVec.y));
		coreInstance:trace(tostring(cameraVec.z));
		local cameraVecZX = cameraVec;
		cameraVecZX.y = 0;
		local lengthVecZX = cameraVecZX:length();
		coreInstance:trace("lengthVecZX");
		coreInstance:trace(tostring(lengthVecZX));
		local cameraVecZXN = cameraVecZX;
		if(cameraVecZXN.x==0)then
			if(cameraVecZXN.y==0)then
				if(cameraVecZXN.z==0)then
					tratar = 0;
				end
			end
		end
		coreInstance:trace("tratar normalize cameraVecZXN");
		coreInstance:trace(tostring(tratar));
		if (tratar == 1) then
			cameraVecZXN:normalize(1);
		end
		local dot = 0;
		coreInstance:trace("tratar dot");
		coreInstance:trace(tostring(tratar));
		if(tratar == 1) then
			dot = playerVecZXN * cameraVecZXN;
		end
		coreInstance:trace("dot");
		coreInstance:trace(tostring(dot));
		local movimentZX = dot * modul;
		coreInstance:trace("movimentZX");
		coreInstance:trace(tostring(movimentZX));
		local percent = movimentZX / lengthVecZX;
		coreInstance:trace("percent");
		coreInstance:trace(tostring(percent));
		local newY = (currentWP.y) * (1 - percent) + (nextWP.y) * (percent);
		coreInstance:trace("newY");
		coreInstance:trace(tostring(newY));
		local newPos = currentWP + (cameraVecZXN * movimentZX);
		newPos.y = newY;
		coreInstance:trace("newPos");
		coreInstance:trace(tostring(newPos.x));
		coreInstance:trace(tostring(newPos.y));
		coreInstance:trace(tostring(newPos.z));
		local obj = cam.m_pObject3D;
		obj:set_position(Vect3f(newPos.x, newPos.y, newPos.z));
		--Update Camera 3D
		if(cam.m_eTypeCamera == 6) then
			local yaw = math.atan2(cameraVecZXN.z, cameraVecZXN.x);
			if(percent > 0.9) then
				if(cam.m_nextWaypoint < cam:get_path_size())then
					local nextVector = cam:get_path_point(cam.m_nextWaypoint + 1) - cam:get_path_point(cam.m_currentWaypoint + 1);
					nextVector:normalize(1);
					nextyaw = math.atan2(nextVector.z,nextVector.x);
					local yawpercent = (percent - 0.9) / (1 - 0.9);
					yaw = yaw * (1-yawpercent) + nextyaw * yawpercent;
				end
			end
			obj:set_yaw(yaw);
			obj:set_pitch(-0.40);
			obj:set_roll(0);
			cam:set_zoom(15);
			cam.m_fZNear = 0.1;
			cam.m_fZFar = 1000;
			--cam.m_fFOV = 45.0 * 3.1415 / 180;
			--cam.m_fAspectRatio = 1;
		end
		--Update Camera 2D
		if(cam.m_eTypeCamera == 5) then
			local yaw = math.atan2(cameraVecZXN.z, cameraVecZXN.x);
			yaw = yaw + (math.pi / 2);
			if(percent > 0.9) then
				if(cam.m_nextWaypoint < cam:get_path_size())then
					local nextVector = cam:get_path_point(cam.m_nextWaypoint + 1) - cam:get_path_point(cam.m_currentWaypoint + 1);
					nextVector:normalize(1);
					local nextyaw = math.atan2(nextVector.z,nextVector.x);
					nextyaw = nextyaw + (math.pi / 2);
					local yawpercent = (percent - 0.9) / (1 - 0.9);
					yaw = yaw * (1-yawpercent) + nextyaw * yawpercent;
				end
			end
			obj:set_yaw(yaw);
			obj:set_pitch(-0.2);
			obj:set_roll(0);
			cam:set_zoom(20);
			cam.m_fZNear = 0.1;
			cam.m_fZFar = 1000;
			--cam.m_fFOV = 45.0 * 3.1415 / 180;
			--cam.m_fAspectRatio = 1;
		end
		--cam.m_lastPlayerPos = currentPlayerPos;
		local vecPos = newPos - currentWP;
		local lengthPos = vecPos:length();
		coreInstance:trace("lengthPos");
		coreInstance:trace(tostring(lengthPos));
		local auxVec = nextWP - currentWP;
		local lengthWP = auxVec:length();
		coreInstance:trace("lengthWP");
		coreInstance:trace(tostring(lengthWP));
		if(dot > 0) then
			if(lengthWP < lengthPos) then
				if(cam.m_nextWaypoint < cam:get_path_size()) then
					coreInstance:trace("currentWaypoint before");
					coreInstance:trace(tostring(cam.m_currentWaypoint));
					cam.m_currentWaypoint = cam.m_nextWaypoint;
					coreInstance:trace("currentWaypoint after");
					coreInstance:trace(tostring(cam.m_currentWaypoint));
					coreInstance:trace("nextWaypoint before");
					coreInstance:trace(tostring(cam.m_nextWaypoint));
					cam.m_nextWaypoint = cam.m_nextWaypoint + 1;
					coreInstance:trace("nextWaypoint after");
					coreInstance:trace(tostring(cam.m_nextWaypoint));
				end
			end
		end
		if (dot<0) then
			if(cam.m_currentWaypoint > 0) then
				coreInstance:trace("nextWaypoint before");
				coreInstance:trace(tostring(cam.m_nextWaypoint));
				cam.m_nextWaypoint = cam.m_currentWaypoint;
				coreInstance:trace("nextWaypoint after");
				coreInstance:trace(tostring(cam.m_nextWaypoint));
				coreInstance:trace("currentWaypoint before");
				coreInstance:trace(tostring(cam.m_currentWaypoint));
				cam.m_currentWaypoint = cam.m_currentWaypoint - 1;
				coreInstance:trace("currentWaypoint after");
				coreInstance:trace(tostring(cam.m_currentWaypoint));
			end
		end
		local name = CNamed();
		name:set_name("LuaBreak");
		--name2:set_name("UpdatePassFinal");
	end
end