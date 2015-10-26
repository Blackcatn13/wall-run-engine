--Crear instancias de camaras
--[[variables
	m_3DCamera
	m_2DCamera
]]
--*****************************************************************************
--************ PARAMTETROS GLOBALES *******************************************
local Dist_To_Camera_Tot = 3;
local incYawTot = 0;
local lastYawBoss = -100;
local incYawBoss = 0;
local lastCamPosBoss = Vect3f(-100,-100,-100);
local incCamPosBoss = Vect3f(0,0,0);
local lastPitchBoss = -100;
local incPitchBoss = 0;
local lastZoomBoss = -100;
local incZoomBoss = 0;

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
	local near3D = 0.1;
	local far3D = 280;
	local distToCameraOffset = 3;
	local distToCameraOffset_back = -2;
	local offsetFollowY = -0.3;
	local dist_to_rotate = 3;
	local yaw_to_rotate = 0.05;
	local cam3D_speed = 11;
	local cam3D_pitchSpeed = 3;
	local cam3D_yawSpeed = 5;
	local cam3D_zoomSpeed = 3;
	local cam3D_distSpeed = 3;
	
	
	--______ CAMERA 2D _______________________
	local pitch2D = -0.1;
	local zoom2D = 18;
	local fov2D = 45.0 * 3.1415 / 180;
	local aspect2D = 16/9;
	local near2D = 0.1;
	local far2D = 90;
	local heightCameraOffset = 2.5;
	local cam2D_speed = 11;
	local cam2D_rotateSpeed = 5;
	local cam2D_yawSpeed = 5;
	local cam2D_zoomSpeed = 5;
	local cam2D_heightSpeed = 5;
	
	--______ CAMERA BOSS _______________________
	local yawBoss = 0;
	local pitchBoss = -0.10;
	local zoomBoss = 25;
	local fovBoss = 60.0 * 3.1415 / 180;
	local aspectBoss = 16/9;
	local nearBoss = 0.1;
	local farBoss = 280;
	local distToCameraOffsetBoss = 3;
	local speedYawBoss = 3;
	local speedPitchBoss = 4;
	local speedZoomBoss = 4;
	local speedCamPos = 4;
	-- Distancia jugable entre 9 y 33
	local distZoomFar = 22;
	local distZoomNear = 21;
	local bossZoomFar = 3;
	local bossZoomNear = 25;
	local camMovementRange = 36;
	local camMovementDistStart = 17;
	local distPitchNear = 17;
	local distPitchFar = 22;
	local bossPitchNear = -0.03;
	local bossPitchFar = -0.35;
	
	--______ GENERALES _______________________
	local distanciaGiro = 3;
	
	--****************************************************************************
	--****************************************************************************
	local coreInstance = CCoreLuaWrapper().m_CoreInstance;
	local act2in = coreInstance:get_action_to_input();
	local camController = coreInstance.m_CameraController;
	local cam = camController:get_active_camera();
--	coreInstance:trace("tipo camara "..tostring(cam.m_eTypeCamera));
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
	if (cam.m_eTypeCamera == 5 or cam.m_eTypeCamera == 6) and cam.m_currentWaypoint ~= nil and player.stop_follow_camera == false then
		local currentWP = cam:get_path_point(cam.m_currentWaypoint);
		local nextWP = cam:get_path_point(cam.m_nextWaypoint);
		-- POINT SPECS
		local pointSpecs = cam:get_path_point_specs(cam.m_currentWaypoint);
		local canRotate = pointSpecs.can_rotate;
		if pointSpecs.dist_to_rotate > 0 then
			dist_to_rotate = pointSpecs.dist_to_rotate;
		end
		if pointSpecs.variable_zoom == true then
			zoom3D = zoom3D + pointSpecs.variable_zoom_value;
			zoom3D_back = zoom3D_back + pointSpecs.variable_zoom_value;
		end
		if pointSpecs.variable_pitch == true then
			pitch3D = pitch3D + pointSpecs.variable_pitch_value;
			pitch3D_back = pitch3D_back + pointSpecs.variable_pitch_value;
		end
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
			if pointSpecs.yaw_offset == true then
				incYaw = pointSpecs.yaw_offset_value;
			end
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
			
			if canRotate == true or pointSpecs.follow_player_x == true then
				local distVector =  currentPlayerPos - newPosReal;
				distVector.y = 0;
				local distFromPath = distVector:length()
				if dist_to_rotate < distFromPath and canRotate == true and pointSpecs.follow_player_x == false then
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
			--coreInstance:trace(tostring(offsetPosVec.y))
			if pointSpecs.follow_player_y == true then
				if player.is_dead then
					local PlayerR = coreInstance:get_renderable_object_layer_manager():get_renderable_objects_manager_by_str_and_room(piky_layer, pCont.m_Room):get_resource(piky_mesh_name);
					offsetPosVec = Vect3f(offsetPosVec.x, pCont:get_position().y + PlayerR:getAnimationBonePosition().y, offsetPosVec.z)
				else
					offsetPosVec = Vect3f(offsetPosVec.x,pCont:get_position().y + offsetFollowY,offsetPosVec.z)
				end
				offsetPosVec = offsetPosVec + Vect3f(0, distToCamera, 0);
			end
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
				obj:set_pitch(lastPitch + incPitch * l_ElapsedTime * cam3D_pitchSpeed);
			else
				local incPitch = pitch3D - lastPitch;
				obj:set_pitch(lastPitch + incPitch * l_ElapsedTime * cam3D_pitchSpeed);
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
			cam.m_fZNear = near3D;
			cam.m_fZFar = far3D;
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
			cam.m_fZNear = near2D;
			cam.m_fZFar = far2D;
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
	elseif cam.m_eTypeCamera == 7 then -- BOSS CAM
		local obj = cam.m_pObject3D;
		local chucky = enemy_manager:get_enemy(boss_mesh_name)
		local chuckyPos = chucky:get_position();
		local playerPos = player_controller:get_position();
		local chuckyPosXZ = Vect3f(chuckyPos.x, 0, chuckyPos.z);
		local playerPosXZ = Vect3f(playerPos.x, 0, playerPos.z);
		local vectToBoss = playerPosXZ - chuckyPosXZ;
		local distToBoss = vectToBoss:length();
		
		local vectPlayerBoss = Vect3f(chuckyPos.x - playerPos.x, 0 ,chuckyPos.z - playerPos.z);
		-- YAW
		yawBoss = math.atan2(vectPlayerBoss.z, vectPlayerBoss.x);
		if lastYawBoss == -100 then
			lastYawBoss = yawBoss;
		end
		local incrAux = math.abs(lastYawBoss - yawBoss);
		while incrAux > math.pi do
			if lastYawBoss < yawBoss then
				lastYawBoss = lastYawBoss + 2 * math.pi;
			else
				yawBoss = yawBoss + 2 * math.pi;
			end
			incrAux = math.abs(lastYawBoss - yawBoss);
		end
		local incTotalYaw = yawBoss - lastYawBoss;
		incYawBoss = incTotalYaw * l_ElapsedTime * speedYawBoss;
		local yawBossTotal = lastYawBoss + incYawBoss;
		obj:set_yaw(yawBossTotal);
		lastYawBoss = yawBossTotal;
		-- PITCH
		local pitchFinal = pitchBoss;
		if lastPitchBoss == -100 then
			lastPitchBoss = pitchFinal;
		end
		if distToBoss > distPitchFar then
			pitchFinal = pitchBoss + (((distToBoss - distPitchFar) * (pitchBoss - bossPitchFar))/(distPitchFar - 33));
		end
		if distToBoss < distPitchNear then
			pitchFinal = pitchBoss + (((distToBoss - distPitchNear) * (pitchBoss - bossPitchNear)) / (distPitchNear - 9));
		end
		local incTotalPitch = pitchFinal - lastPitchBoss;
		incPitchBoss = incTotalPitch * l_ElapsedTime * speedPitchBoss;
		local pitchFinalTotal = lastPitchBoss + incPitchBoss;
		obj:set_pitch(pitchFinalTotal);
		lastPitchBoss = pitchFinalTotal;
		obj:set_roll(0);
		local CamPos = Vect3f(chuckyPos.x, chuckyPos.y, chuckyPos.z);
		if(lastCamPosBoss.x == -100 and lastCamPosBoss.y == -100 and lastCamPosBoss.z == -100) then
			lastCamPosBoss = Vect3f(CamPos.x, CamPos.y, CamPos.z);
		end
		if distToBoss > camMovementDistStart then
			vectToBoss:normalize(1);
			local percentCamMovement = (distToBoss - camMovementDistStart) / (33 - camMovementDistStart);
			CamPos = chuckyPos + (vectToBoss * percentCamMovement * camMovementRange);
		end
		local incTotalCamPos = CamPos - lastCamPosBoss;
		incCamPosBoss = incTotalCamPos * l_ElapsedTime * speedCamPos;
		local camPosFinal = lastCamPosBoss + incCamPosBoss;
		obj:set_position(camPosFinal);
		lastCamPosBoss = Vect3f(camPosFinal.x, camPosFinal.y, camPosFinal.z);
		local zoomFinal = zoomBoss;
		if lastZoomBoss == -100 then
			lastZoomBoss = zoomFinal;
		end
		if distToBoss > distZoomFar then
			zoomFinal = zoomBoss + (((distToBoss - distZoomFar) * (zoomBoss - bossZoomFar))/(distZoomFar - 33));
		end
		if distToBoss < distZoomNear then
			zoomFinal = zoomBoss + (((distToBoss - distZoomNear) * (zoomBoss - bossZoomNear)) / (distZoomNear - 9));
		end
		local incTotalZoom = zoomFinal - lastZoomBoss;
		incZoomBoss = incTotalZoom * l_ElapsedTime * speedZoomBoss;
		local zoomFinalTotal = lastZoomBoss + incZoomBoss;
		cam:set_zoom(zoomFinalTotal);
		lastZoomBoss = zoomFinalTotal;
		cam.m_fZNear = nearBoss;
		cam.m_fZFar = farBoss;
		cam.m_fFOV = fovBoss;
		cam.m_fAspectRatio = aspectBoss;
	end
end

function reset_cameras()
	local cam = coreInstance.m_CameraController:get_resource('3DCam');
	cam.m_eTypeCamera = 6;
	cam.m_currentWaypoint = 0;
	cam.m_nextWaypoint = 1;
end