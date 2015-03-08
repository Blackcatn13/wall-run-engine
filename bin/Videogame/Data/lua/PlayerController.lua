function on_update_player_controller(l_ElapsedTime)
	local coreInstance = CCoreLuaWrapper().m_CoreInstance;
	local luaUtil = CCMathLuaUtils();
	
	local player = coreInstance:get_player_controller();
	--local deltaX, deltaY, deltaZ;
	--deltaX = coreInstance:get_input_manager():get_mouse_delta().x;
	--deltaY = coreInstance:get_input_manager():get_mouse_delta().y;
	--deltaZ = coreInstance:get_input_manager():get_mouse_delta().z;
	
	--if act2in:do_action_from_lua("yaw", deltaX) then
	--	player:set_yaw(player:get_yaw() - deltaX * l_ElapsedTime);
	--end
	--if act2in:do_action_from_lua("pitch", deltaY) then
	--	player:set_pitch(player:get_pitch() - deltaY * l_ElapsedTime);
	--end
	--local yaw = player:get_yaw();*/
	if act2in:do_action_from_lua("ChangeDimension") then
		if player.m_is3D == true then 
			player.m_is3D = false;
		else
			player.m_is3D = true;
		end
	end
	local mov = Vect3f(0,0,0);
		if act2in:do_action_from_lua("MoveForward") then
			if player.m_is3D == true then	
				mov = mov + Vect3f(0,0,1) * player.m_Speed * l_ElapsedTime;
			end
		end
		if act2in:do_action_from_lua("MoveBack") then
			if player.m_is3D == true then	
				mov = mov - Vect3f(0,0,1) * player.m_Speed * l_ElapsedTime;
			end
		end
		if act2in:do_action_from_lua("MoveRigth") then
			if player.m_is3D == true then	
				mov = mov - Vect3f(-1,0,0) * player.m_Speed * l_ElapsedTime;
			else
				mov = mov + Vect3f(0,0,1) * player.m_Speed * l_ElapsedTime;
			end
		end
		if act2in:do_action_from_lua("MoveLeft") then
			if player.m_is3D == true then	
				mov = mov + Vect3f(-1,0,0) * player.m_Speed * l_ElapsedTime;
			else
				mov = mov - Vect3f(0,0,1) * player.m_Speed * l_ElapsedTime;
			end
		end
	if act2in:do_action_from_lua("Jump") then
		player.m_isJumping = true;
		player.m_CurrentJumpForce = player.m_JumpForce;
		mov.y = player.m_CurrentJumpForce;
	end
	if (player.m_isJumping == true) and (player.m_CurrentJumpForce > 0) then
		player.m_CurrentJumpForce = player.m_CurrentJumpForce - (player.m_Gravity * l_ElapsedTime);
		mov.y = player.m_CurrentJumpForce;
	end
	player.m_PhysicController:move(mov,l_ElapsedTime);
	player:set_position(m_PhysicController:get_position());
	
	return 0;
end