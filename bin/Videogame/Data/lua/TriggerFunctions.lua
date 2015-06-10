function activate_teleport(player_position)
	--local player = coreInstance:get_player_controller()
	coreInstance:trace(player_position)
	local vec_array = split_str(player_position, "%s")
	local vec3f_array = Vect3f(tonumber(vec_array[1]), tonumber(vec_array[2]), tonumber(vec_array[3]))
	player_controller.m_PhysicController:set_position(vec3f_array)
	local camera = coreInstance.m_CameraController:get_active_camera()
	--	coreInstance:trace("laaaaaa")
	camera.m_pObject3D:set_position(Vect3f(camera.m_pObject3D:get_position().x, camera.m_pObject3D:get_position().y, vec3f_array.z - 10))

	--camera.m_pObject3D:set_position(Vect3f(3.118870, 20.0, 271.008423))
end
function set_is_3D()
	--local player = coreInstance:get_player_controller();
	player_controller.m_is3D = true;
	coreInstance.m_CameraController:set_active_camera("3DCam");

end

function set_player_zone(zone_number)
	player.zone = zone_number
	coreInstance:trace("New Zone " .. tostring(player.zone) )
end

function unlock_image(image_name)
	coreInstance:trace("Imagen a obtener: "  ..image_name)
	for i = 1, table.maxn(Cards_Vector) do
	coreInstance:trace("Imagen segun el vector: " ..Cards_Vector[i]:get_name())
		if Cards_Vector[i]:get_name() == image_name then
			Cards_Vector[i]:set_unlocked(true)
			coreInstance:trace("Image ".. image_name .."Unclocked!!")
			return 0
		end
	end
	coreInstance:trace("Image Not found")
end

function toogle_switch(trigger_name, puzzle_name)
	coreInstance:trace("Enter switch")
	--local player = Player:get_instance()
	coreInstance:trace(tostring(player.get_controller().m_isJumping))
	local puzzle = coreInstance:get_puzzle_manager():get_resource(puzzle_name)
	--local total_switches = puzzle.m_MinActivatedSwitches
	
	if player ~= nil then
		--local player_controller = player.get_controller()
		--if player_controller.m_isAttack == true or player_controller.m_isJumping == true then
			--local trigger_manager = coreInstance:get_trigger_manager()
			local trigger = trigger_manager:get_resource(trigger_name)
			--coreInstance:trace("Toogle switch")
			if trigger ~= nil then		
				local switch_mesh = get_renderable_object("puzzle", trigger_name)
				if trigger.m_IsSwitched ~= true then
					trigger.m_IsSwitched = true
				--	coreInstance:trace("Trigger activado?" ..tostring(trigger.m_IsSwitched))
					puzzle.m_ActivatedSwitches = puzzle.m_ActivatedSwitches + 1	
					local new_position = Vect3f(switch_mesh:get_position().x,switch_mesh:get_position().y - 0.3,switch_mesh:get_position().z )
					change_object_position(switch_mesh, new_position)
				--[[else 
					trigger.m_IsSwitched = false
					coreInstance:trace(tostring(trigger.m_IsSwitched))
					puzzle.m_ActivatedSwitches = puzzle.m_ActivatedSwitches - 1
					local new_position = Vect3f(switch_mesh:get_position().x,switch_mesh:get_position().y + 0.3,switch_mesh:get_position().z )
					change_object_position(switch_mesh, new_position)
					--]]
				end
			end
		--end
	end
	--coreInstance:trace(tostring(puzzle.m_ActivatedSwitches))
	--if activated_switches == total_switches then --TODO obtener datos del puzzle en si
		--coreInstance:trace("all switches activated")
		--open_door()
	--end
end

function activate_trigger_update(trigger_name)
	local coreInstance = CCoreLuaWrapper().m_CoreInstance;
	--local trigger_manager = coreInstance:get_trigger_manager()
	local trigger = trigger_manager:get_resource(trigger_name)
	trigger.m_Update=true

end 

function trigger_set_2D(offset_axis)
	local player_controller = coreInstance:get_player_controller();
	player.going_back = false;
	player_controller.m_is3D = false;
	local cam = coreInstance.m_CameraController:get_resource("3DCam");
	cam.m_eTypeCamera = 5;
	coreInstance.m_CameraController:set_active_camera("3DCam");
	if player_controller.m_isTurned == true then
		player_controller.m_isTurned = false
	end
	
	--player:set_yaw(coreInstance.m_CameraController:get_active_camera().m_pObject3D:get_yaw())
	--coreInstance:trace(tostring(player:get_yaw()))
	local camera_object = coreInstance.m_CameraController:get_active_camera().m_pObject3D
	local active_camera = cam_Controller:get_active_camera();
	local currentWP = nil
	local nextWP = nil
	if active_camera.m_currentWaypoint ~= nil then
		currentWP = active_camera:get_path_point(active_camera.m_currentWaypoint)
		nextWP = active_camera:get_path_point(active_camera.m_nextWaypoint)
	end
	local pos_x =  player_controller:get_position().x
	local pos_z =  player_controller:get_position().z
	coreInstance:trace(offset_axis)
	if offset_axis == "x" then
		pos_x = camera_object:get_position().x
	elseif offset_axis == "z" then
		pos_z = camera_object:get_position().z
	else
		pos_x = camera_object:get_position().x
		pos_z = camera_object:get_position().z
	end
	
	local position_2d = Vect3f(pos_x, player_controller:get_position().y ,pos_z)
	
	if currentWP ~= nil and nextWP ~= nil then
		local camPathVector = nextWP - currentWP
		camPathVector.y = 0
		local playerVec = (player_controller.m_PhysicController:get_position()) - currentWP
		playerVec.y = 0
		if playerVec.x ~= 0 or playerVec.y ~= 0 or playerVec.z ~= 0 then
			local modul = playerVec:length()
			playerVec:normalize(1)
			camPathVector:normalize(1)
			local dot = camPathVector * playerVec
			local moviment = dot * modul
			local playerPos = currentWP + (camPathVector * moviment)
			position_2d.x = playerPos.x
			position_2d.z = playerPos.z
		end
	end
	
	player_controller.m_PhysicController:set_position(position_2d)
	move_character_controller_mesh(player_controller, position_2d);
end

function trigger_set_3D()
	local player_controller = coreInstance:get_player_controller();
	player_controller.m_is3D = true;
	local cam = coreInstance.m_CameraController:get_resource("3DCam");
	cam.m_eTypeCamera = 6;
	coreInstance.m_CameraController:set_active_camera("3DCam");

end

function player_die()
	--local coreInstance = CCoreLuaWrapper().m_CoreInstance;
	coreInstance:trace("dieeeeee")
	--local player = Player:get_instance()
	player.player_die()
end

function set_player_respawn_spikes_point()
	--local player = Player:get_instance()
	player.last_spikes_position = Vect3f(player.get_player_controller():get_position())
end

function set_checkpoint(trigger_name)
	--local coreInstance = CCoreLuaWrapper().m_CoreInstance;
		coreInstance:trace("enter checkpoint")
	--local player = Player:get_instance()
	local current_position = Vect3f(player.get_player_controller():get_position())
	coreInstance:trace("Player Position set")
	local checkpoint = CheckPoint.new()
	coreInstance:trace("Checkpoint created")
	--coreInstance:trace(tostring(checkpoint:is_activated()))
	--if checkpoint:is_activated() == false then
	if player.check_visited_checkpoints(trigger_name) == false then
		checkpoint.set_checkpoint(player, trigger_name)
	end
	coreInstance:trace(tostring(checkpoint.is_activated))
	coreInstance:trace("checkpoint activated")
--	end

end

function set_puzzle_enemy_active(active)
	if active == "true" then
		player.enemy_puzzle_active = true
	else
		player.enemy_puzzle_active = false
	end
	coreInstance:trace("Enemy puzle active ".. tostring(player.enemy_puzzle_active))
end

function check_enemies_killed(num1, door)
	if player.enemy_puzzle_active then
		coreInstance:trace("----------------------------Enemies: ".. tostring(player.enemies_killed))
		if player.enemies_killed == tonumber(num1) then
			-- Aparece el collectible
			coreInstance:trace("----------------------------Cromo is here!!")
			local trigger_name ="Collectible1_UserData"
			local trigger = trigger_manager:get_resource(trigger_name)
			activate_collectible(trigger, "collectible", "Collectible1")
			player.enemy_puzzle_active = false
		end
		if player.enemies_killed == 5 then
			coreInstance:trace("----------------------------Opening door")
			open_door(door)
		end
	end
end

function ChuckyApear()
	coreInstance:trace("CHUKY APEARS!!!");
	local active_camera = cam_Controller:get_active_camera();
	local dirVec = active_camera:get_path_point(active_camera.m_nextWaypoint) - active_camera:get_path_point(active_camera.m_currentWaypoint);
	dirVec.y = 0;
	dirVec:normalize(1);
	local current_position = Vect3f(player.get_player_controller():get_position());
	local distance = 18;
	current_position = current_position - dirVec * distance;
	local pos = Vect3f(current_position.x,current_position.y + 4,current_position.z);
	local Chucky = enemy_manager:get_enemy("Chucky");
	Chucky:move_to_position(pos);
	local yaw = player.get_player_controller():get_yaw();
	Chucky.m_RenderableObject:set_yaw(-math.rad(90));
	Chucky:m_FSM():newState("Corriendo");
end

function ChuckyDesapears()
	coreInstance:trace("CHUKY goes out");
	local Chucky = enemy_manager:get_enemy("Chucky");
	Chucky:m_FSM():newState("Parado");
end

function ChuckyJump()
	coreInstance:trace("Chucky Jumping");
	local Chucky = enemy_manager:get_enemy("Chucky");
	Chucky:m_FSM():newState("Saltando");
end