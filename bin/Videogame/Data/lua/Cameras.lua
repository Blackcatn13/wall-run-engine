--Crear instancias de camaras
--[[variables
	m_3DCamera
	m_2DCamera
]]
--*****************************************************************************
--************ PARAMTETROS GLOBALES *******************************************
local Dist_To_Camera_Tot = 3;
local incYawTot = 0;
--*****************************************************************************
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
	--****************************************************************************
	--*********** PARAMETROS VARIABLES *******************************************
	--______ CAMERA 3D _______________________
	local pitch3D = -0.20;
	local pitch3D_back = -0.60;
	local zoom3D = 6;
	local zoom3D_back = 12;
	local fov3D = 60.0 * 3.1415 / 180;
	local aspect3D = 16/9;
	local distToCameraOffset = 3;
	local distToCameraOffset_back = -2;
	local dist_to_rotate = 3;
	local yaw_to_rotate = 0.05;
	local cam3D_speed = 11;
	local cam3D_rotateSpeed = 5;
	local cam3D_yawSpeed = 5;
	local cam3D_zoomSpeed = 5;
	local cam3D_distSpeed = 5;
	
	
	--______ CAMERA 2D _______________________
	local pitch2D = -0.1;
	local zoom2D = 18;
	local fov2D = 45.0 * 3.1415 / 180;
	local aspect2D = 16/9;
	local heightCameraOffset = 2.5;
	local cam2D_speed = 11;
	local cam2D_rotateSpeed = 5;
	local cam2D_yawSpeed = 5;
	local cam2D_zoomSpeed = 5;
	local cam2D_heightSpeed = 5;
	
	--______ GENERALES _______________________
	local distanciaGiro = 3;
	
	--****************************************************************************
	--****************************************************************************
	local coreInstance = CCoreLuaWrapper().m_CoreInstance;
	local act2in = coreInstance:get_action_to_input();
	local camController = coreInstance.m_CameraController;
	local cam = camController:get_active_camera();
	if gui_manager:get_is_displayed_console() == false then
		if act2in:do_action_from_lua("Set3DCamera") then
			--coreInstance.m_CameraController:set_active_camera("3DCam");
			cam.m_eTypeCamera = 6;
		end
		if act2in:do_action_from_lua("Set2DCamera") then
			--coreInstance.m_CameraController:set_active_camera("2DCam");
			cam.m_eTypeCamera = 5;
		end
	end
	--local name2=CNamed();
	--name2:set_name("UpdatePass1");
	if cam.m_eTypeCamera ~= 2 and cam.m_eTypeCamera ~= 1 and cam.m_currentWaypoint ~= nil then
		local currentWP = cam:get_path_point(cam.m_currentWaypoint);
		local nextWP = cam:get_path_point(cam.m_nextWaypoint);
		local pointSpecs = cam:get_path_point_specs(cam.m_currentWaypoint);
		local canRotate = pointSpecs.can_rotate;
		--name2:set_name("UpdatePass2");
		--local lastPlayerPos = cam.m_lastPlayerPos;
		local pCont = coreInstance:get_player_controller();
		local currentPlayerPos = pCont:get_position();
		local playerVec = currentPlayerPos - currentWP;
		local playerVecZX = playerVec;
		playerVecZX.y = 0;
		local modul = playerVecZX:length();
		local playerVecZXN = playerVecZX;
		local tratar = 1;
		if(playerVecZXN.x==0)then
			if(playerVecZXN.y==0)then
				if(playerVecZXN.z==0)then
					tratar = 0;
				end
			end
		end
		if (tratar == 1) then
			playerVecZXN:normalize(1);
		end
		local cameraVec = nextWP - currentWP;
		local cameraVecZX = cameraVec;
		cameraVecZX.y = 0;
		local lengthVecZX = cameraVecZX:length();
		local cameraVecZXN = cameraVecZX;
		if(cameraVecZXN.x==0)then
			if(cameraVecZXN.y==0)then
				if(cameraVecZXN.z==0)then
					tratar = 0;
				end
			end
		end
		if (tratar == 1) then
			cameraVecZXN:normalize(1);
		end
		local dot = 0;
		if(tratar == 1) then
			dot = playerVecZXN * cameraVecZXN;
		end
		local movimentZX = dot * modul;
		local percent = movimentZX / lengthVecZX;
		local percentToRotation = 0.5;
		if(lengthVecZX > 2 * distanciaGiro) then
			percentToRotation = distanciaGiro / lengthVecZX;
		end
		local newY = (currentWP.y) * (1 - percent) + (nextWP.y) * (percent);
		local newPos = currentWP + (cameraVecZXN * movimentZX);
		newPos.y = newY;
		local newPosReal = Vect3f(0,0,0);
		if(percent > 1) then
			newPosReal = nextWP;
		else
			if (percent < 0) then
				newPosReal = currentWP;
			else
				newPosReal = newPos;
			end
		end
		local obj = cam.m_pObject3D;
		local lastPos = obj:get_position();
		--obj:set_position(Vect3f(newPosReal.x, newPosReal.y, newPosReal.z));
		--Update Camera 3D
		if(cam.m_eTypeCamera == 6) then
			local incYaw = 0;
			local distToCamera = 0;
			if player.going_back == true and pointSpecs.can_go_back == true then
				local incDist = distToCameraOffset_back - Dist_To_Camera_Tot;
				distToCamera = Dist_To_Camera_Tot + incDist * l_ElapsedTime * cam3D_distSpeed;
				Dist_To_Camera_Tot = distToCamera;
			else
				local incDist = distToCameraOffset - Dist_To_Camera_Tot;
				distToCamera = Dist_To_Camera_Tot + incDist * l_ElapsedTime * cam3D_distSpeed;
				Dist_To_Camera_Tot = distToCamera;
			end
			local offsetPosVec = newPosReal + Vect3f(0, distToCamera, 0);
			
			if canRotate == true then
				local distVector =  currentPlayerPos - newPosReal;
				distVector.y = 0;
				local distFromPath = distVector:length()
				if dist_to_rotate < distFromPath then
					incYaw = (distFromPath - dist_to_rotate) * yaw_to_rotate;
				end
				local vector_aux = Vect3f(cameraVecZXN.x, 0, cameraVecZXN.z);
				vector_aux:rotate_y(math.rad(90));
				local dotAux = vector_aux * distVector;
				if dotAux > 0 then
					incYaw = -incYaw;
				end
				local playerPos = Vect3f(pCont:get_position().x, newPosReal.y, pCont:get_position().z);
				offsetPosVec = playerPos + Vect3f(0, distToCamera, 0);
				
			end
			coreInstance:trace(tostring(offsetPosVec.y))
			local movVec = offsetPosVec - lastPos;
			local finalPos = lastPos + (movVec * (l_ElapsedTime * cam3D_speed));
			obj:set_position(Vect3f(finalPos.x, finalPos.y, finalPos.z));
			--obj:set_position(Vect3f(newPosReal.x, newPosReal.y, newPosReal.z));
			local yaw = math.atan2(cameraVecZXN.z, cameraVecZXN.x);
			if(percent >= 1 - percentToRotation) then
				if(cam.m_nextWaypoint < cam:get_path_size())then
					local nextVector = cam:get_path_point(cam.m_nextWaypoint + 1) - cam:get_path_point(cam.m_currentWaypoint + 1);
					nextVector:normalize(1);
					nextyaw = math.atan2(nextVector.z,nextVector.x);
					local diferenciaYaw = math.abs(yaw-nextyaw);
					if(diferenciaYaw > math.pi) then
						if(nextyaw < yaw) then
							nextyaw = nextyaw + 2*math.pi;
						else
							yaw = yaw + 2*math.pi;
						end
					end
					local yawpercent = (percent - (1 - percentToRotation)) / (1 - (1 - percentToRotation));
					yawpercent = yawpercent / 2;
					if (percent>1) then
						yawpercent = 0.5;
					end
					yaw = yaw * (1-yawpercent) + nextyaw * yawpercent;
					if(yaw > math.pi) then
						yaw = yaw - 2 * math.pi;
					end
				end
			end
			if(percent < percentToRotation) then
				if(cam.m_currentWaypoint > 0) then
					local previousVector = cam:get_path_point(cam.m_nextWaypoint - 1) - cam:get_path_point(cam.m_currentWaypoint - 1);
					previousVector:normalize(1);
					previousyaw = math.atan2(previousVector.z,previousVector.x);
					local diferenciaYaw = math.abs(yaw-previousyaw);
					if(diferenciaYaw > math.pi) then
						if(previousyaw < yaw) then
							previousyaw = previousyaw + 2*math.pi;
						else
							yaw = yaw + 2 * math.pi;
						end
					end
					local yawpercent = percent / percentToRotation;
					yawpercent = 0.5 + (yawpercent / 2);
					if (percent < 0) then
						yawpercent = 0.5;
					end
					yaw = yaw * yawpercent + previousyaw * (1-yawpercent);
					if(yaw > math.pi) then
						yaw = yaw - 2 * math.pi;
					end
				end
			end
			incrementYaw = incYaw - incYawTot;
			yaw = yaw + incYawTot + incrementYaw * l_ElapsedTime * cam3D_yawSpeed;
			incYawTot = incYaw;
			obj:set_yaw(yaw);
			local lastPitch = obj:get_pitch();
			if player.going_back == true and pointSpecs.can_go_back == true then
				local incPitch = pitch3D_back - lastPitch;
				obj:set_pitch(lastPitch + incPitch * l_ElapsedTime * cam3D_rotateSpeed);
			else
				local incPitch = pitch3D - lastPitch;
				obj:set_pitch(lastPitch + incPitch * l_ElapsedTime * cam3D_rotateSpeed);
			end
			obj:set_roll(0);
			local lastZoom = cam:get_zoom();
			if player.going_back == true and pointSpecs.can_go_back == true then
				local incZoom = zoom3D_back - lastZoom;
				cam:set_zoom(lastZoom + incZoom * l_ElapsedTime * cam3D_zoomSpeed);
			else
				local incZoom = zoom3D - lastZoom;
				cam:set_zoom(lastZoom + incZoom * l_ElapsedTime * cam3D_zoomSpeed);
			end
			cam.m_fZNear = 0.1;
			cam.m_fZFar = 280;
			cam.m_fFOV = fov3D;
			cam.m_fAspectRatio = aspect3D;
		end
		--Update Camera 2D
		if(cam.m_eTypeCamera == 5) then
			local offsetPosVec = newPosReal + Vect3f(0, heightCameraOffset, 0);
			local movVec = offsetPosVec - lastPos;
			local finalPos = lastPos + (movVec * (l_ElapsedTime * cam2D_speed));
			obj:set_position(Vect3f(finalPos.x, finalPos.y, finalPos.z));
			local yaw = math.atan2(cameraVecZXN.z, cameraVecZXN.x);
			yaw = yaw + (math.pi / 2);
			if(percent >= 1 - percentToRotation) then
				if(cam.m_nextWaypoint < cam:get_path_size())then
					local nextVector = cam:get_path_point(cam.m_nextWaypoint + 1) - cam:get_path_point(cam.m_currentWaypoint + 1);
					nextVector:normalize(1);
					local nextyaw = math.atan2(nextVector.z,nextVector.x);
					nextyaw = nextyaw + (math.pi / 2);
					local diferenciaYaw = math.abs(yaw-nextyaw);
					if(diferenciaYaw > math.pi) then
						if(nextyaw < yaw) then
							nextyaw = nextyaw + 2*math.pi;
						else
							yaw = yaw + 2*math.pi;
						end
					end
					local yawpercent = (percent - (1 - percentToRotation)) / (1 - (1 - percentToRotation));
					yawpercent = yawpercent / 2;
					if (percent>1) then
						yawpercent = 0.5;
					end
					yaw = yaw * (1-yawpercent) + nextyaw * yawpercent;
					if(yaw > math.pi) then
						yaw = yaw - 2 * math.pi;
					end
				end
			end
			if(percent < percentToRotation) then
				if(cam.m_currentWaypoint > 0) then
					local previousVector = cam:get_path_point(cam.m_nextWaypoint - 1) - cam:get_path_point(cam.m_currentWaypoint - 1);
					previousVector:normalize(1);
					previousyaw = math.atan2(previousVector.z,previousVector.x);
					previousyaw = previousyaw + (math.pi / 2);
					local diferenciaYaw = math.abs(yaw-previousyaw);
					if(diferenciaYaw > math.pi) then
						if(previousyaw < yaw) then
							previousyaw = previousyaw + 2*math.pi;
						else
							yaw = yaw + 2*math.pi;
						end
					end
					local yawpercent = percent / percentToRotation;
					yawpercent = 0.5 + (yawpercent / 2);
					if (percent < 0) then
						yawpercent = 0.5;
					end
					yaw = yaw * yawpercent + previousyaw * (1-yawpercent);
					if(yaw > math.pi) then
						yaw = yaw - 2 * math.pi;
					end
				end
			end
			obj:set_yaw(yaw);
			obj:set_pitch(pitch2D);
			obj:set_roll(0);
			cam:set_zoom(zoom2D);
			cam.m_fZNear = 0.1;
			cam.m_fZFar = 30;
			cam.m_fFOV = fov2D;
			cam.m_fAspectRatio = aspect2D;
		end
		--cam.m_lastPlayerPos = currentPlayerPos;
		local vecPos = newPos - currentWP;
		local lengthPos = vecPos:length();
		local auxVec = nextWP - currentWP;
		local lengthWP = auxVec:length();
		if(dot > 0) then
			if(lengthWP < lengthPos) then
				if(cam.m_nextWaypoint < cam:get_path_size()) then
					cam.m_currentWaypoint = cam.m_nextWaypoint;
					cam.m_nextWaypoint = cam.m_nextWaypoint + 1;
				end
			end
		end
		if (dot<0) then
			if(cam.m_currentWaypoint > 0) then
				cam.m_nextWaypoint = cam.m_currentWaypoint;
				cam.m_currentWaypoint = cam.m_currentWaypoint - 1;
			end
		end
		--name2:set_name("UpdatePassFinal");
	end
end