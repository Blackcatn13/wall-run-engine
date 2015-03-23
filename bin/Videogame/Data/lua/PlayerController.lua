local coreInstance = CCoreLuaWrapper().m_CoreInstance;
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
	player.m_Gravity = 1;						--Gravedad que afecta al personaje cuando cae.
	player.m_GravityJump = 1;					--Gravedad que afecta cuando el personaje está impulsándose hacia arriba en el salto.
	player.m_Speed = 8;							--Velocidad de movimiento.
	player.m_JumpForce = 0.35;					--Fuerza de salto, impulso.
	player.m_PhysicController:set_step(0.3); 	--Altura que puede superar (escalones).
	--////////////////////////////////////////////////////////
	
	if act2in:do_action_from_lua("ChangeDimension") then
		if player.m_is3D == true then 
			player.m_is3D = false;
			coreInstance.m_CameraController:set_active_camera("2DCam");
		else
			player.m_is3D = true;
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
	
	--///////////////////////////////////////////////////////////
	-- Movimiento del Player en las distintas direcciones. 
	--///////////////////////////////////////////////////////////
	--if player.m_isJumping == false then
		if act2in:do_action_from_lua("MoveForward") then
			if player.m_is3D == true then
				mov = mov + dir3D * player.m_Speed * l_ElapsedTime;
				player:set_yaw(0); -- 0º
				if act2in:do_action_from_lua("MoveRigth") then
					player:set_yaw(0.7854); -- 45º
					mov = mov - dirNor * player.m_Speed * l_ElapsedTime;
					player.m_isTurned = false;
				elseif act2in:do_action_from_lua("MoveLeft") then
					player:set_yaw(5.497); -- 315º
					mov = mov + dirNor * player.m_Speed * l_ElapsedTime;
					player.m_isTurned = true;
				end
			end
		elseif act2in:do_action_from_lua("MoveBack") then
			if player.m_is3D == true then
				mov = mov - dir3D * player.m_Speed * l_ElapsedTime;
				player:set_yaw(3.1415); -- 180º
				if act2in:do_action_from_lua("MoveRigth") then
					player:set_yaw(2.356); -- 135º
					mov = mov - dirNor * player.m_Speed * l_ElapsedTime;
					player.m_isTurned = false;
				elseif act2in:do_action_from_lua("MoveLeft") then
					player:set_yaw(3.926); -- 225º
					mov = mov + dirNor * player.m_Speed * l_ElapsedTime;
					player.m_isTurned = true;
				end
			end
		elseif act2in:do_action_from_lua("MoveRigth") then
			if player.m_is3D == true then	
				player:set_yaw(1.57); -- 90º
			end	
			mov = mov - dirNor * player.m_Speed * l_ElapsedTime;
			player.m_isTurned = false;
		elseif act2in:do_action_from_lua("MoveLeft") then
			if player.m_is3D == true then	
				player:set_yaw(4.712); -- 270º
			end	
			mov = mov + dirNor * player.m_Speed * l_ElapsedTime;
			player.m_isTurned = true;
		end
	--end
	--///////////////////////////////////////////////////////////
	-- Gravedad que siempre afecta al Player. 
	--///////////////////////////////////////////////////////////
	mov.y = -player.m_Gravity;
	
	--///////////////////////////////////////////////////////////
	-- Cuando el Player está saltando, su velocidad dependerá del tipo de salto realizado. 
	--///////////////////////////////////////////////////////////
	if player.m_isJumping == true then
		if player.m_CurrentJumpForce < 0 then
			player.m_CurrentJumpForce = player.m_CurrentJumpForce - (player.m_Gravity * l_ElapsedTime);
		else
			player.m_CurrentJumpForce = player.m_CurrentJumpForce - (player.m_GravityJump * l_ElapsedTime);
		end
		mov.y = player.m_CurrentJumpForce;
		if player.m_isJumpingMoving == false then
			mov.y = player.m_CurrentJumpForce*1.75;
		end
		if player.m_is3D == false then
			if player.m_isJumpingMoving == true then
				if player.m_isTurned == false then
					mov.z = player.m_JumpForce/1.5;
				else
					mov.z = -player.m_JumpForce/1.5;
				end
			end
		end
	end	
	
	--///////////////////////////////////////////////////////////
	-- Acción de saltar del Player. Puede realizar 2 saltos distintos (de longitud, y salto vertical). 
	--///////////////////////////////////////////////////////////
	if (act2in:do_action_from_lua("Jump")) and (player.m_isJumping == false) then
		player.m_isJumping = true;
		player.m_CurrentJumpForce = player.m_JumpForce;
		mov.y = player.m_CurrentJumpForce;
		if player.m_is3D == false then
			if act2in:do_action_from_lua("MoveRigth") then
				mov.z = player.m_JumpForce/1.5;
				player.m_isJumpingMoving = true;
			elseif act2in:do_action_from_lua("MoveLeft") then
				mov.z = -player.m_JumpForce/1.5;
				player.m_isJumpingMoving = true;
			else
				mov.y = player.m_CurrentJumpForce*2;
			end
		end
	end
	
	player:is_grounded(mov,l_ElapsedTime);
	player:set_position(player.m_PhysicController:get_position());
	move_character_controller_mesh(player, player:get_position());
	
	return 0;
end

function move_character_controller_mesh(_player, _position)
	local mesh = coreInstance:get_renderable_object_layer_manager():get_default_renderable_object_manager():get_resource("SpongePicky")
	mesh:set_yaw(_player:get_yaw())
	mesh:set_position(_position)
--[[CRenderableObject* malla = RENDLM->GetDefaultRenderableObjectManager()->GetResource("SpongePicky");
    malla->SetYaw(m_fYaw);
    malla->SetPosition(m_PhysicController->GetPosition());--]]

end