local coreInstance = CCoreLuaWrapper().m_CoreInstance;
local is_init=true;
local topPosition = -1000;
local inLoop = false;
local _land = false;
local _fallPosition = Vect3f(-10000, -10000, -10000);
function on_update_player_lua(l_ElapsedTime)
	--local coreInstance = CCoreLuaWrapper().m_CoreInstance;

	local luaUtil = CCMathLuaUtils();
	local act2in = coreInstance:get_action_to_input();
	local cam_Controller = coreInstance.m_CameraController;
	local active_camera = cam_Controller:get_active_camera();
	local camObject = active_camera.m_pObject3D;
	local player = coreInstance:get_player_controller();
	local lightM = coreInstance:get_light_manager();
	local directionalLight = lightM:get_resource("Direct002");
	local playerRenderable = coreInstance:get_renderable_object_layer_manager():get_default_renderable_object_manager():get_resource("SpongePicky");
	--////////////////////////////////////////////////////////
	-- PARAMETERS
	--////////////////////////////////////////////////////////
	player.m_Gravity = 9;						--Gravedad que afecta al personaje cuando cae.
	player.m_GravityJump = 11;					--Gravedad que afecta cuando el personaje está impulsándose hacia arriba en el salto.
	player.m_Speed = 8;							--Velocidad de movimiento.
	player.m_JumpForce = 5.5;					--Fuerza de salto, impulso.
	superjumForce = 10;						 	--SUPERSALTO CHEAT
	player.m_AttackForce = 3;					--Impulse force for the attack.
	player.m_PhysicController:set_step(0.3); 	--Altura que puede superar (escalones).
	local AirTime = 0.6;						-- Time into the air, playing air loop
	--////////////////////////////////////////////////////////
	if is_init == true then
		is_init = false
		player.m_is3D = true
	end
	if act2in:do_action_from_lua("ChangeDimension") then
		if player.m_is3D == true then 
			player.m_is3D = false;
			local cam = coreInstance.m_CameraController:get_resource("3DCam");
			cam.m_eTypeCamera = 5;
			coreInstance.m_CameraController:set_active_camera("3DCam");
		else
			player.m_is3D = true;
			local cam = coreInstance.m_CameraController:get_resource("3DCam");
			cam.m_eTypeCamera = 6;
			coreInstance.m_CameraController:set_active_camera("3DCam");
		end
	end
	-- MOVER LA LUZ DEL SHADOWMAP
	local lightDirection = directionalLight:get_direction();
	local altura = directionalLight:get_position().y - player:get_position().y;
	local offsetx = lightDirection.x*altura;
	local offsetz = lightDirection.z*altura;
	directionalLight:set_position(Vect3f(player:get_position().x-offsetx,player:get_position().y+altura,player:get_position().z-offsetz));
	--directionalLight:set_position(Vect3f(player:get_position().x,player:get_position().y+altura,player:get_position().z));
	local dir3D = active_camera:get_direction();
	local dirYaw = camObject:get_yaw();
	local dirNor = Vect3f(math.cos(dirYaw + (math.pi/2)), 0, (math.sin(dirYaw + (math.pi/2))));
	dir3D.y = 0;
	dir3D = luaUtil:normalize(dir3D);
	dirNor.y = 0;
	dirNor = luaUtil:normalize(dirNor);
	local mov = Vect3f(0,0,0);
	local PlayerYaw =  - dirYaw + 1.57;
	
	--///////////////////////////////////////////////////////////
	-- Movimiento del Player en las distintas direcciones. 
	--///////////////////////////////////////////////////////////
	if (player.m_isJumpingMoving == false) and (player.m_isAttack == false) then
		if act2in:do_action_from_lua("MoveForward") then
			if player.m_is3D == true then
				mov = mov + dir3D * player.m_Speed * l_ElapsedTime;
				player.m_Direction3D = dir3D;
				if act2in:do_action_from_lua("MoveRigth") then
					player:set_yaw(PlayerYaw + 0.7854); -- 45º
					mov = mov - dirNor * player.m_Speed * l_ElapsedTime;
					player.m_Direction3D = dir3D - dirNor;
					player.m_isTurned = false;
					player.m_JumpType = 5;
				elseif act2in:do_action_from_lua("MoveLeft") then
					player:set_yaw(PlayerYaw + 5.497); -- 315º
					mov = mov + dirNor * player.m_Speed * l_ElapsedTime;
					player.m_Direction3D = dir3D + dirNor;
					player.m_isTurned = true;
					player.m_JumpType = 8;
				else
					player:set_yaw(PlayerYaw); -- 0º
					player.m_JumpType = 1;
				end
				if player.m_isJumping == true then
					mov = mov * 0.75;
				end
			end
		elseif act2in:do_action_from_lua("MoveBack") then
			if player.m_is3D == true then
				mov = mov - dir3D * player.m_Speed * l_ElapsedTime;
				player.m_Direction3D = Vect3f(0,0,0) - dir3D;
				if act2in:do_action_from_lua("MoveRigth") then
					player:set_yaw(PlayerYaw + 2.356); -- 135º
					mov = mov - dirNor * player.m_Speed * l_ElapsedTime;
					player.m_Direction3D = Vect3f(0,0,0) - dir3D - dirNor;
					player.m_isTurned = false;
					player.m_JumpType = 6;
				elseif act2in:do_action_from_lua("MoveLeft") then
					player:set_yaw(PlayerYaw + 3.926); -- 225º
					mov = mov + dirNor * player.m_Speed * l_ElapsedTime;
					player.m_Direction3D = Vect3f(0,0,0) - dir3D + dirNor;
					player.m_isTurned = true;
					player.m_JumpType = 7;
				else
					player:set_yaw(PlayerYaw + 3.1415); -- 180º
					player.m_JumpType = 3;
				end
				if player.m_isJumping == true then
					mov = mov * 0.75;
				end
			end
		elseif act2in:do_action_from_lua("MoveRigth") then
			if player.m_is3D == true then	
				player:set_yaw(PlayerYaw + 1.57); -- 90º
				player.m_JumpType = 2;
			else
				player:set_yaw(PlayerYaw); -- 0º
			end	
			mov = mov - dirNor * player.m_Speed * l_ElapsedTime;
			player.m_Direction3D = Vect3f(0,0,0) - dirNor;
			if player.m_isJumping == true then
				mov = mov * 0.75;
			end
			player.m_isTurned = false;
		elseif act2in:do_action_from_lua("MoveLeft") then
			if player.m_is3D == true then	
				player:set_yaw(PlayerYaw + 4.712); -- 270º
				player.m_JumpType = 4;
			else
				player:set_yaw(PlayerYaw + 3.1415); -- 180º
			end	
			mov = mov + dirNor * player.m_Speed * l_ElapsedTime;
			player.m_Direction3D = dirNor;
			if player.m_isJumping == true then
				mov = mov * 0.75;
			end
			player.m_isTurned = true;
		else
			--player:set_yaw(PlayerYaw);
			player.m_JumpType = 0;
		end
	end
	
	if player.m_is3D == false then
		if player.m_isTurned == false then
			player:set_yaw(PlayerYaw + 1.57); -- 90º
		else
			player:set_yaw(PlayerYaw + 4.712); -- 270º
		end
	end

	-- --///////////////////////////////////////////////////////////
	-- Gravedad que siempre afecta al Player. 
	--///////////////////////////////////////////////////////////
	mov.y = -player.m_Gravity * l_ElapsedTime;
	
	--///////////////////////////////////////////////////////////
	-- Cuando el Player está saltando, su velocidad dependerá del tipo de salto realizado. 
	--///////////////////////////////////////////////////////////
	if _land then
		local fallPos = player:get_position();
		-- coreInstance:trace(tostring(fallPos.x))
		-- coreInstance:trace(tostring(fallPos.y))
		-- coreInstance:trace(tostring(fallPos.z))
		-- coreInstance:trace(tostring(_fallPosition.x))
		-- coreInstance:trace(tostring(_fallPosition.y))
		-- coreInstance:trace(tostring(_fallPosition.z))
		local distance = (fallPos.x - _fallPosition.x) * (fallPos.x - _fallPosition.x) + (fallPos.y - _fallPosition.y) * (fallPos.y - _fallPosition.y) + (fallPos.z - _fallPosition.z) * (fallPos.z - _fallPosition.z);
		coreInstance:trace("distance")
		coreInstance:trace(tostring(distance));
		if distance <= 2.0 then
			playerRenderable:clear_cycle(3,0.5);
			playerRenderable:execute_action(4,0,1,1,false);
			_land = false;
		end
	end
	if (player.m_isJumping == true) or (player.m_isFalling) then
		
		if player.m_CurrentJumpForce < 0 then
			player.m_CurrentJumpForce = player.m_CurrentJumpForce - (player.m_Gravity * l_ElapsedTime);
		else
			player.m_CurrentJumpForce = player.m_CurrentJumpForce - (player.m_GravityJump * l_ElapsedTime);
		end

		if player.m_isJumping then
			mov.y = playerRenderable:getBoneMovement().y;
		end
		if inLoop then
			mov.y = 0;
		end
		if mov.y == 0 and inLoop == false then
			player.m_isFalling = true;
			--coreInstance:trace(tostring(topPosition));
			player.m_isJumpingMoving = false;
		end
		if player.m_JumpingTime > AirTime then
			player.m_isJumping = false;
			--playerRenderable:clear_cycle(3,0);
			inLoop = false;
			topPosition = -1000;
			_land = true;
			local info = SCollisionInfo();
			local _dirRay = Vect3f(0,-1,0);
			coreInstance:get_phisics_manager():raycast_closest_actor(player:get_position() - 0.4, _dirRay, 1, info, 1000);
			_fallPosition = info.m_CollisionPoint;
		else
			if player.m_isFalling then
				local positionOld = playerRenderable:get_position();
				local auxPosition = playerRenderable:getAnimationBonePosition().y; 
				local newPosition = Vect3f(positionOld.x, auxPosition, positionOld.z);
				playerRenderable:set_position(newPosition);
				playerRenderable:remove_action(2);
				playerRenderable:clear_cycle(0,0);
				playerRenderable:clear_cycle(1,0);
				playerRenderable:blend_cycle(3,1,0.5);
				playerRenderable:updateSkeleton(l_ElapsedTime);
				player.m_isFalling = false;
				inLoop = true;
			end
			player.m_JumpingTime = player.m_JumpingTime + l_ElapsedTime;
			--playerRenderable:execute_action(3, 0, 0, 1, false);
		end
		if player.m_isJumpingMoving == true then
			if player.m_is3D == false then
				if player.m_isTurned == false then
					mov = mov - dirNor * player.m_JumpForce*1.5 * l_ElapsedTime;
				else
					mov = mov + dirNor * player.m_JumpForce*1.5 * l_ElapsedTime;
				end
			else
				if player.m_JumpType == 1 then --Forward
					mov = mov + dir3D * player.m_JumpForce*1.5 * l_ElapsedTime;
				elseif player.m_JumpType == 2 then --Right
					mov = mov - dirNor * player.m_JumpForce*1.5 * l_ElapsedTime;
				elseif player.m_JumpType == 3 then --Back
					mov = mov - dir3D * player.m_JumpForce*1.5 * l_ElapsedTime;
				elseif player.m_JumpType == 4 then --Left
					mov = mov + dirNor * player.m_JumpForce*1.5 * l_ElapsedTime;
				elseif player.m_JumpType == 5 then --Forward-Right
					mov = mov + dir3D * player.m_JumpForce*1.5 * l_ElapsedTime;
					mov = mov - dirNor * player.m_JumpForce*1.5 * l_ElapsedTime;
				elseif player.m_JumpType == 6 then --Back-Right
					mov = mov - dir3D * player.m_JumpForce*1.5 * l_ElapsedTime;
					mov = mov - dirNor * player.m_JumpForce*1.5 * l_ElapsedTime;
				elseif player.m_JumpType == 7 then --Back-Left
					mov = mov - dir3D * player.m_JumpForce*1.5 * l_ElapsedTime;
					mov = mov + dirNor * player.m_JumpForce*1.5 * l_ElapsedTime;
				elseif player.m_JumpType == 8 then --Forward-Left
					mov = mov + dir3D * player.m_JumpForce*1.5 * l_ElapsedTime;
					mov = mov + dirNor * player.m_JumpForce*1.5 * l_ElapsedTime;
				end
			end
		end
	end
	
	if player.m_isAttack == true then
		if player.m_CurrentAttackForce > 0.5 then	
			player.m_CurrentAttackForce = player.m_CurrentAttackForce - (player.m_Gravity*2 * l_ElapsedTime);
			mov = player.m_Direction3D * player.m_CurrentAttackForce;
			mov.y = 0.0;
		else
			local mesh = playerRenderable;
			mesh:set_position(player:get_position());
			player.m_isAttack = false;
		end
	end
	
	--///////////////////////////////////////////////////////////
	-- Acción de saltar del Player. Puede realizar 2 saltos distintos (de longitud, y salto vertical). 
	--///////////////////////////////////////////////////////////
	if (act2in:do_action_from_lua("Jump")) and (player.m_isJumping == false) then
		coreInstance:getWWSoundManager():PlayEvent("Jump", "Piky");
		player.m_JumpingTime = 0;
		playerRenderable:clear_cycle(0,0);
		playerRenderable:clear_cycle(1,0);
		playerRenderable:execute_action(2,0,0,1,true);
		player.m_isJumping = true;
		player.m_CurrentJumpForce = player.m_JumpForce;
		if player.m_is3D == false then
			if act2in:do_action_from_lua("MoveRigth") then
				player.m_isJumpingMoving = true;
			elseif act2in:do_action_from_lua("MoveLeft") then
				player.m_isJumpingMoving = true;
			end
		else
			if player.m_JumpType >= 1 then
				player.m_isJumpingMoving = true;
			end
		end
		if (act2in:do_action_from_lua("SuperJump")) then
			player.m_CurrentJumpForce = superjumForce*2;
		end
	end
	
	--///////////////////////////////////////////////////////////
	-- Acción de atacar del Player. Realiza un impulso hacia adelante. 
	--///////////////////////////////////////////////////////////
	if act2in:do_action_from_lua("Attack") then--) and (player.m_isAttack == false) then
		player.m_CurrentAttackForce = player.m_AttackForce;
		mov = player.m_Direction3D * player.m_CurrentAttackForce;
		mov.y = 0.0;
		player.m_isAttack = true;
	end
	
	if player.m_isJumping then
		player:move(mov, l_ElapsedTime);
	else
		player:is_grounded(mov,l_ElapsedTime);
	end
	player:set_position(player.m_PhysicController:get_position());
	move_character_controller_mesh(player, player:get_position(), player.m_isJumping);
	if not player.m_isJumping and not _land then
		if mov.x == 0 and mov.z == 0 then
			playerRenderable:clear_cycle(1,0);
			playerRenderable:blend_cycle(0,1,0);
		else
			playerRenderable:clear_cycle(0,0);
			playerRenderable:blend_cycle(1,1,0);
		end
	end
	return 0;
end

function move_character_controller_mesh(_player, _position, _jumping)
	local mesh = coreInstance:get_renderable_object_layer_manager():get_default_renderable_object_manager():get_resource("SpongePicky")
	mesh:set_yaw(_player:get_yaw() + math.pi)
	local pos;
	if _jumping and not inLoop then
		pos = mesh:getAnimationBonePosition().y;
		local oldPosition = mesh:get_position();
		local mesh_position = Vect3f(_position.x, oldPosition.y, _position.z)
		mesh:set_position(mesh_position)
	else
		pos = mesh:getBonePosition().y;
		--coreInstance:trace(tostring(pos));
		local mesh_position = Vect3f(_position.x, _position.y - pos - 0.2, _position.z)
		mesh:set_position(mesh_position)
	end
	
end