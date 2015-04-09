local coreInstance = CCoreLuaWrapper().m_CoreInstance;
local is_init=true;
function on_update_player_lua(l_ElapsedTime)
	--local coreInstance = CCoreLuaWrapper().m_CoreInstance;
	local luaUtil = CCMathLuaUtils();
	local act2in = coreInstance:get_action_to_input();
	local cam_Controller = coreInstance.m_CameraController;
	local active_camera = cam_Controller:get_active_camera();
	local camObject = active_camera.m_pObject3D;
	
	local player = coreInstance:get_player_controller();
	--////////////////////////////////////////////////////////
	-- PARAMETERS
	--////////////////////////////////////////////////////////
	player.m_Gravity = 11;						--Gravedad que afecta al personaje cuando cae.
	player.m_GravityJump = 11;					--Gravedad que afecta cuando el personaje está impulsándose hacia arriba en el salto.
	player.m_Speed = 8;							--Velocidad de movimiento.
	player.m_JumpForce = 5.5;					--Fuerza de salto, impulso.
	superjumForce = 10;						 	--SUPERSALTO CHEAT
	player.m_AttackForce = 3;					--Impulse force for the attack.
	player.m_PhysicController:set_step(0.3); 	--Altura que puede superar (escalones).
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
	--///////////////////////////////////////////////////////////
	-- Gravedad que siempre afecta al Player. 
	--///////////////////////////////////////////////////////////
	mov.y = -player.m_Gravity * l_ElapsedTime;
	
	--///////////////////////////////////////////////////////////
	-- Cuando el Player está saltando, su velocidad dependerá del tipo de salto realizado. 
	--///////////////////////////////////////////////////////////
	if player.m_isJumping == true then
		if player.m_CurrentJumpForce < 0 then
			player.m_CurrentJumpForce = player.m_CurrentJumpForce - (player.m_Gravity * l_ElapsedTime);
		else
			player.m_CurrentJumpForce = player.m_CurrentJumpForce - (player.m_GravityJump * l_ElapsedTime);
		end
		mov.y = player.m_CurrentJumpForce * l_ElapsedTime;
		--if player.m_isJumpingMoving == false then
			mov.y = player.m_CurrentJumpForce*2 * l_ElapsedTime;
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
			local mesh = coreInstance:get_renderable_object_layer_manager():get_default_renderable_object_manager():get_resource("SpongePicky");
			mesh:set_position(player:get_position());
			player.m_isAttack = false;
		end
	end
	
	--///////////////////////////////////////////////////////////
	-- Acción de saltar del Player. Puede realizar 2 saltos distintos (de longitud, y salto vertical). 
	--///////////////////////////////////////////////////////////
	if (act2in:do_action_from_lua("Jump")) and (player.m_isJumping == false) then
		player.m_isJumping = true;
		player.m_CurrentJumpForce = player.m_JumpForce;
		mov.y = player.m_CurrentJumpForce*1.75 * l_ElapsedTime;
		if player.m_is3D == false then
			if act2in:do_action_from_lua("MoveRigth") then
				player.m_isJumpingMoving = true;
			elseif act2in:do_action_from_lua("MoveLeft") then
				player.m_isJumpingMoving = true;
			else
				mov.y = player.m_CurrentJumpForce*2 * l_ElapsedTime;
			end
		else
			if player.m_JumpType >= 1 then
				player.m_isJumpingMoving = true;
			else
				mov.y = player.m_CurrentJumpForce*2 * l_ElapsedTime;
			end
		end
		if (act2in:do_action_from_lua("SuperJump")) then
			mov.y = superjumForce*2 * l_ElapsedTime;
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
	
	player:is_grounded(mov,l_ElapsedTime);
	player:set_position(player.m_PhysicController:get_position());
	move_character_controller_mesh(player, player:get_position());
	
	return 0;
end

function move_character_controller_mesh(_player, _position)
	local mesh = coreInstance:get_renderable_object_layer_manager():get_default_renderable_object_manager():get_resource("SpongePicky")
	--coreInstance:trace(tostring(mesh:get_position().z))
	mesh:set_yaw(_player:get_yaw())
	local mesh_position = Vect3f(_position.x, _position.y-0.4, _position.z)
	mesh:set_position(mesh_position)
--[[CRenderableObject* malla = RENDLM->GetDefaultRenderableObjectManager()->GetResource("SpongePicky");
    malla->SetYaw(m_fYaw);
    malla->SetPosition(m_PhysicController->GetPosition());--]]

end