function on_update_player_lua(l_ElapsedTime)
	local coreInstance = CCoreLuaWrapper().m_CoreInstance;
	local luaUtil = CCMathLuaUtils();
	local act2in = coreInstance:get_action_to_input();
	local cam_Controller = coreInstance.m_CameraController;
	
	local player = coreInstance:get_player_controller();
	--////////////////////////////////////////////////////////
	-- PARAMETERS
	--////////////////////////////////////////////////////////
	player.m_Gravity = 0.2;
	player.m_GravityJump = 0.2;
	player.m_Speed = 8;
	player.m_JumpForce = 0.2;
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
	local active_camera = cam_Controller:get_active_camera();
	local camObject = active_camera.m_pObject3D;
	local dir3D = active_camera:get_direction();
	local dirYaw = camObject:get_yaw();
	local dirNor = Vect3f(math.cos(dirYaw + (math.pi/2)), 0, (math.sin(dirYaw + (math.pi/2))));
	dir3D.y = 0;
	dir3D = luaUtil:normalize(dir3D);
	dirNor.y = 0;
	dirNor = luaUtil:normalize(dirNor);
	local mov = Vect3f(0,0,0);
	if act2in:do_action_from_lua("MoveForward") then
		if player.m_is3D == true then	
			--mov = mov + Vect3f(0,0,1) * player.m_Speed * l_ElapsedTime;
			mov = mov + dir3D * player.m_Speed * l_ElapsedTime;
		end
	end
	if act2in:do_action_from_lua("MoveBack") then
		if player.m_is3D == true then	
			--mov = mov - Vect3f(0,0,1) * player.m_Speed * l_ElapsedTime;
			mov = mov - dir3D * player.m_Speed * l_ElapsedTime;
		end
	end
	if act2in:do_action_from_lua("MoveRigth") then
		if player.m_is3D == true then	
			--mov = mov - Vect3f(-1,0,0) * player.m_Speed * l_ElapsedTime;
			mov = mov - dirNor * player.m_Speed * l_ElapsedTime;
		else
			mov = mov + Vect3f(0,0,1) * player.m_Speed * l_ElapsedTime;
		end
	end
	if act2in:do_action_from_lua("MoveLeft") then
		if player.m_is3D == true then	
			--mov = mov + Vect3f(-1,0,0) * player.m_Speed * l_ElapsedTime;
			mov = mov + dirNor * player.m_Speed * l_ElapsedTime;
		else
			mov = mov - Vect3f(0,0,1) * player.m_Speed * l_ElapsedTime;
		end
	end
	
	if player.m_isJumping == true then
		if player.m_CurrentJumpForce < 0 then
			player.m_CurrentJumpForce = player.m_CurrentJumpForce - (player.m_Gravity * l_ElapsedTime);
		else
			player.m_CurrentJumpForce = player.m_CurrentJumpForce - (player.m_GravityJump * l_ElapsedTime);
		end
		mov.y = player.m_CurrentJumpForce;
	else
		mov.y = 0.1;---player.m_Gravity/10;
	end
	if (act2in:do_action_from_lua("Jump")) and (player.m_isGrounded == true) then
		player.m_isJumping = true;
		player.m_CurrentJumpForce = player.m_JumpForce;
		mov.y = player.m_CurrentJumpForce;
	end
	player.m_PhysicController:movePlayer(mov,l_ElapsedTime,player.m_isGrounded);
	player:set_position(player.m_PhysicController:get_position());
	--move_character_controller_mesh(player, player:get_position());
	
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