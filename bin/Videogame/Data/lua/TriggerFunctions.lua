local teleporting = false
function set_teleport_ready(player_position)
	if (player_controller.m_isAttack == false) and (player.is_hit == false and not player.is_dead) then
		renderable_objects_layer_manager:change_between_layers(piky_layer, "vanishing", player_controller.m_Room, "Piky")
		local player_renderable = get_renderable_object("vanishing", player_controller.m_Room, "Piky")
		player_renderable.m_VanishActive =true
		player_renderable.m_Vanishing = true
	--	player.can_move = false
		player_renderable.m_Modifier = 1.0
		player.vanishing = true
		local teleportRenderable = get_renderable_object("solid", 6, "Teleport")
		if teleportRenderable.m_ParticleEmitter ~= "" and teleportRenderable.m_ParticleEmitter2 ~= "" then
			local emitter = particle_manager:get_resource(teleportRenderable.m_ParticleEmitter)
			local pos = player_renderable:get_position()+ teleportRenderable.m_EmitterOffset
			update_emitter_status(emitter, pos, true)
			local emitter2 = particle_manager:get_resource(teleportRenderable.m_ParticleEmitter2)
			local pos2 = player_renderable:get_position()+ teleportRenderable.m_EmitterOffset2
			update_emitter_status(emitter2, pos2, true)
			
		end
	end
--	activate_teleport(player_position)
end

function update_emitter_status(emitter, position, visible)
	emitter.m_vPos = position
	emitter:set_visible(visible)
	--coreInstance:trace("Emitter position: "..tostring(position.x)..", "..tostring(position.y)..", "..tostring(position.z))
end
function vanish_player(name, room)
	if (player_controller.m_isAttack == false) and (player.is_hit == false and not player.is_dead) and player.vanishing then
		--local player = coreInstance:get_player_controller()
		local player_renderable = get_renderable_object("vanishing", player_controller.m_Room, "Piky")
		if (player_renderable.m_Vanishing) then
			player_renderable.m_Modifier = player_renderable.m_Modifier - 0.5* coreInstance.m_ElapsedTime
		--	coreInstance:trace("Alpha Modifier: ".. tostring(player_renderable.m_Modifier))
			if (player_renderable.m_Modifier <= 0) then
				if not teleporting then
					activate_teleport(name, room)
					teleporting = true
					
				end
				player_renderable.m_Vanishing = false;
						
			end
		end
	end
end
function player_teleported()
	local teleportRenderable = get_renderable_object("solid", 6, "Teleport")
	if teleportRenderable.m_ParticleEmitter ~= "" and teleportRenderable.m_ParticleEmitter2 ~= "" then
		local emitter = particle_manager:get_resource(teleportRenderable.m_ParticleEmitter)
		local emitter_offset = Vect3f(0,1,0)
		local pos = teleportRenderable:get_position() +emitter_offset
		update_emitter_status(emitter, pos, false)
		local emitter2 = particle_manager:get_resource(teleportRenderable.m_ParticleEmitter2)
		local pos2 = teleportRenderable:get_position() +emitter_offset
		update_emitter_status(emitter2, pos2, false)
		
	end
end

function unvanish_player()
	if teleporting == true then
		local player_renderable = get_renderable_object("vanishing", player_controller.m_Room, "Piky")
		if not player_renderable.m_Vanishing then
			player_renderable.m_Modifier = player_renderable.m_Modifier +0.5*  coreInstance.m_ElapsedTime
			if ( player_renderable.m_Modifier >= 1) then
				player.can_move = true
				player_renderable.m_Vanishing = true
				player_renderable.m_VanishActive =false
				renderable_objects_layer_manager:change_between_layers("vanishing", "player", player_controller.m_Room, "Piky")
				player.vanishing = false
				player_renderable.m_Modifier = 1.0
				teleporting = false
				player_teleported()
			end
		end
	end
end

function activate_teleport(name, room)
-- una vez se desvanece del todo
--	coreInstance:trace("Teletransportando: ".. player_position)
	local teleport_back = get_renderable_object("solid", tonumber(room), name)
	local position = teleport_back:get_position()
	local final_pos = Vect3f(position.x, position.y+2, position.z)
	player_controller.m_PhysicController:set_position(final_pos)
	local camera = coreInstance.m_CameraController:get_active_camera()
	--	coreInstance:trace("laaaaaa")
	--camera.m_pObject3D:set_position(Vect3f(camera.m_pObject3D:get_position().x, camera.m_pObject3D:get_position().y, vec3f_array.z - 10))
	local cam_object =  camera.m_pObject3D
	local PlayerYaw =  - cam_object:get_yaw() + 1.57
	if player_controller.m_is3D == true then
		player_controller:set_yaw(PlayerYaw)
	else
		player_controller.m_isTurned = false
		player_controller:set_yaw(PlayerYaw + 1.57); -- 90ยบ
	end
	local teleportRenderable = get_renderable_object("solid", 6, "Teleport")
	if teleportRenderable.m_ParticleEmitter ~= "" and teleportRenderable.m_ParticleEmitter2 ~= ""  then
		local teleportRenderable2 = get_renderable_object("solid", 6, name)
		local emitter = particle_manager:get_resource(teleportRenderable.m_ParticleEmitter)
		local pos = teleportRenderable2:get_position()+ teleportRenderable.m_EmitterOffset
		update_emitter_status(emitter, pos, true)
		local emitter2 = particle_manager:get_resource(teleportRenderable.m_ParticleEmitter2)
		local pos2 = teleportRenderable2:get_position()+ teleportRenderable.m_EmitterOffset2
		update_emitter_status(emitter2, pos2, true)
		
	end
	coreInstance:getWWSoundManager():PlayEvent("Teleport", "Music");
	--camera.m_pObject3D:set_position(Vect3f(3.118870, 20.0, 271.008423))
end

function set_is_3D()
	--local player = coreInstance:get_player_controller();
	player_controller.m_is3D = true;
	coreInstance.m_CameraController:set_active_camera("3DCam");

end

function set_player_zone(zone_number)
--set_player_room("3", "false")
	player.zone = zone_number
	--renderable_objects_layer_manager:change_between_vectors("player", "Piky", tonumber(zone_number))
end

function unlock_image(image_name)
	for i = 1, table.maxn(Cards_Vector) do
		if Cards_Vector[i]:get_name() == image_name then
			Cards_Vector[i]:set_unlocked(true)
			return 0
		end
	end
end

function toogle_switch(trigger_name, puzzle_name)
	--local player = Player:get_instance()
	--coreInstance:trace(tostring(player.get_controller().m_isJumping))
	local puzzle = coreInstance:get_puzzle_manager():get_resource(puzzle_name)
	local total_switches = puzzle.m_MinActivatedSwitches
	
	if player ~= nil then
		--local player_controller = player.get_controller()
		--if player_controller.m_isAttack == true or player_controller.m_isJumping == true then
			--local trigger_manager = coreInstance:get_trigger_manager()
			local trigger = trigger_manager:get_resource(trigger_name)
			--coreInstance:trace("Toogle switch")
			if trigger ~= nil then		
				local switch_mesh = get_renderable_object("puzzle",6, trigger_name)
				if trigger.m_IsSwitched ~= true then
					trigger.m_IsSwitched = true
				--	coreInstance:trace("Trigger activado?" ..tostring(trigger.m_IsSwitched))
					puzzle.m_ActivatedSwitches = puzzle.m_ActivatedSwitches + 1	
					coreInstance:getWWSoundManager():PlayEvent("SwitchActivate", "Music");
				--	local new_position = Vect3f(switch_mesh:get_position().x,switch_mesh:get_position().y - 1,switch_mesh:get_position().z )
				--	change_object_position(switch_mesh, new_position)
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
	if puzzle.m_ActivatedSwitches == total_switches and not puzzle.m_Done then
		local trigger = trigger_manager:get_resource("Collectible4_UserData")
		coreInstance:getWWSoundManager():PlayEvent("AppearCard", "Music");
		activate_collectible(trigger, "collectible", "Collectible4")
	end
	--coreInstance:trace(tostring(puzzle.m_ActivatedSwitches))
	--[[if activated_switches == total_switches then --TODO obtener datos del puzzle en si
		coreInstance:trace("all switches activated")
		open_door(puzzle.m_SceneElement, puzzle.m_SceneElement2)
	end]]
end

function activate_trigger_update(trigger_name)
	local coreInstance = CCoreLuaWrapper().m_CoreInstance;
	--local trigger_manager = coreInstance:get_trigger_manager()
	local trigger = trigger_manager:get_resource(trigger_name)
	trigger.m_Update=true
end 

function trigger_set_2D(offset_axis)
	coreInstance:getWWSoundManager():PlayEvent("_2DZone", "Music");
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
	--coreInstance:trace(offset_axis)
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
	--coreInstance:get_cinematic_controller():execute("Chucky-appears");
end

function trigger_set_3D()
	coreInstance:getWWSoundManager():PlayEvent("_3DZone", "Music");
	local player_controller = coreInstance:get_player_controller();
	player_controller.m_is3D = true;
	local cam = coreInstance.m_CameraController:get_resource("3DCam");
	cam.m_eTypeCamera = 6;
	coreInstance.m_CameraController:set_active_camera("3DCam");

end

function player_die(burning)
	--local coreInstance = CCoreLuaWrapper().m_CoreInstance;
	--coreInstance:trace("dieeeeee: ".. name)
	--local player = Player:get_instance()
	--coreInstance:trace("Piky burning?: "..tostring(burning))
	player.has_ass_burned = toboolean(burning)
	--coreInstance:trace("Stop Follow Camera previo del trigger: "..tostring(player.stop_follow_camera))
	if not player.has_ass_burned and player.activating_triggers then
		player.dead_in_hole = true
		sound_manager:PlayEvent("Piky_Fall","Piky");
		player.stop_follow_camera = true
		--coreInstance:trace("Stop Follow Camera del trigger: "..tostring(player.stop_follow_camera))
	end
	player.player_die()
end

function set_player_respawn_spikes_point()
	--local player = Player:get_instance()
	player.last_spikes_position = Vect3f(player.get_player_controller():get_position())
	player.spikes_yaw = player_controller:get_yaw()
end

function set_checkpoint(trigger_name, renderable_object)
	--local coreInstance = CCoreLuaWrapper().m_CoreInstance;
	--local player = Player:get_instance()
	local current_position = Vect3f(player.get_player_controller():get_position())
	
	--coreInstance:trace(tostring(checkpoint:is_activated()))
	--if checkpoint:is_activated() == false then
	if player.check_visited_checkpoints(trigger_name) == false then
		local checkpoint = CheckPoint.new()
		--coreInstance:trace(renderable_object)
		local checkpoint_mesh = nil
		if renderable_object ~= nil then
			checkpoint_mesh = get_renderable_object("solid", player_controller.m_Room, renderable_object)
			if checkpoint_mesh == nil then
				checkpoint_mesh = get_renderable_object("solid", player_controller.m_Room +1, renderable_object)
				if checkpoint_mesh == nil then -- Para el caso del fallo aleatorio en el reset
					checkpoint_mesh = get_renderable_object("solid", 1, renderable_object)
				end
			end
		end
		checkpoint.set_checkpoint(player, trigger_name, checkpoint_mesh)
	end
end

function test_on_enter()
	--coreInstance:trace("Entrando en trigger")
end

function test_on_exit( param)
	--coreInstance:trace("Saliendo de trigger " .. param)
end

function test_on_stay( param)
	--coreInstance:trace("Dentro de trigger ".. param)
end
-------------------------------------------------------------------
-- Esta es temporal hasta que se pongan los renderables en la mashup
--------------------------------------------------------------------
--[[function set_checkpoint(trigger_name)
	--local coreInstance = CCoreLuaWrapper().m_CoreInstance;
		coreInstance:trace("enter checkpoint")
	--local player = Player:get_instance()
	local current_position = Vect3f(player.get_player_controller():get_position())
	coreInstance:trace("Player Position set")
	
	--coreInstance:trace(tostring(checkpoint:is_activated()))
	--if checkpoint:is_activated() == false then
	if player.check_visited_checkpoints(trigger_name) == false then
		local checkpoint = CheckPoint.new()
		coreInstance:trace("Checkpoint created")
		checkpoint.set_checkpoint(player, trigger_name, nil)
		coreInstance:trace(tostring(checkpoint.is_activated))
		coreInstance:trace("checkpoint activated")
	end
end
--]]
function set_puzzle_enemy_active(active)
	if active == "true" then
		player.enemy_puzzle_active = true
		player.attack_enabled = true
	else
		player.enemy_puzzle_active = false
	end
end

function activate_invisible_wall(name, room_number)
	local wall = get_renderable_object("invisible",tonumber(room_number), name)
	if wall ~= nil then
		wall:activate_phisic(true)
	end
end

function deactivate_invisible_wall(name,room_number)
	local wall = get_renderable_object("invisible",tonumber(room_number), name)
	if wall ~= nil then
		wall:activate_phisic(false)
	end
end

function check_enemies_killed(num1, door)
	if player.enemy_puzzle_active then
		if player.enemies_killed == tonumber(num1) then
			-- Aparece el collectible
			local trigger_name ="Collectible1_UserData"
			local trigger = trigger_manager:get_resource(trigger_name)
			if(trigger ~= nil) then
				activate_collectible(trigger, "collectible", "Collectible1")
			end
			player.enemy_puzzle_active = false
		end
		if player.enemies_killed == 2 then
			open_door(door)
		end
	end
end

function check_enemies_killed(num1, door, door2)
	if player.enemy_puzzle_active then
		if player.enemies_killed == tonumber(num1) then
			-- Aparece el collectible
			local trigger_name ="Collectible1_UserData"
			local trigger = trigger_manager:get_resource(trigger_name)
			if(trigger ~= nil) then
				activate_collectible(trigger, "collectible", "Collectible1")
				coreInstance:getWWSoundManager():PlayEvent("AppearCard", "Music");
			end
			player.enemy_puzzle_active = false
		end
		if player.enemies_killed == 2 then
			open_door(door, door2)
		end
	end
end

function ChuckyApear(distance)
	if player_controller.m_Room == 2 or player_controller.m_Room == 5 or player_controller.m_Room == 7 then
		local Chucky = enemy_manager:get_enemy("Chucky");
		if player.activating_triggers and not Chucky.m_Appeared then
			chucky_able_to_catch = true
			local active_camera = cam_Controller:get_active_camera();
			local dirVec = active_camera:get_path_point(active_camera.m_nextWaypoint) - active_camera:get_path_point(active_camera.m_currentWaypoint);
			dirVec.y = 0;
			dirVec:normalize(1);
			local current_position = Vect3f(player.get_player_controller():get_position());
			current_position = current_position - dirVec * tonumber(distance);
			local pos = Vect3f(current_position.x,current_position.y + 4,current_position.z);
			Chucky:move_to_position(pos);
			local yaw = coreInstance:get_renderable_object_layer_manager():get_renderable_objects_manager_by_str_and_room("player", player_controller.m_Room):get_resource(piky_mesh_name):get_yaw();
			Chucky.m_RenderableObject:set_yaw(yaw);
			Chucky.m_Appeared = true
			set_chucky_need_to_jump(false)
			Chucky:m_FSM():newState("Corriendo");
			Chucky:set_visible(true)
			inputm:set_game_pad_left_motor_speed(7000, 1);
		end
	end
end

function ChuckyReApears(distance)
	local Chucky = enemy_manager:get_enemy("Chucky");
	Chucky.m_Appeared = false
	if not chucky_reapeared then
		ChuckyApear(distance)
		chucky_reapeared = true
	end
end

function ChuckyDesapears()
	local Chucky = enemy_manager:get_enemy("Chucky");
	--if(Chucky.m_Appeared == true) then
		local pos = Vect3f(1000.30, 1000.0, -1000.75)
		Chucky:move_to_position(pos);
		Chucky:set_yaw(0.0);
		Chucky:m_FSM():newState("Parado");
		Chucky.m_Appeared = false
		chucky_reapeared = false
		chucky_falling = false
		inputm:set_game_pad_left_motor_speed(0, 1);
		local emitter = particle_manager:get_resource(Chucky.m_RenderableObject.m_ParticleEmitter)
		local emitter2 = particle_manager:get_resource(Chucky.m_RenderableObject.m_ParticleEmitter2)
		if emitter ~= nil and emitter2 ~= nil then
			emitter:set_visible(false)
			emitter2:set_visible(false)
		end
	--end
end

function ChuckyJump()
	--coreInstance:trace("Chucky Jumping");
	local Chucky = enemy_manager:get_enemy("Chucky");
	Chucky:m_FSM():newState("Saltando");
	set_chucky_need_to_jump(true)
end

function chucky_fall_into_hole()
	local Chucky = enemy_manager:get_enemy("Chucky");
	Chucky:m_FSM():newState("Saltando");
	showing_emitter = false
	chucky_falling = true
	particle_manager:get_resource(Chucky.m_RenderableObject.m_ParticleEmitter):set_visible(false)
end

function ChuckyStop(able_to_cach)
	local Chucky = enemy_manager:get_enemy("Chucky")
	if tostring(able_to_cach) == "false" then
		chucky_able_to_catch = false
	end
	Chucky:m_FSM():newState("Stopping")
end

function chucky_check_jump()
	if tostring(player.inside_obstacle_area) == "false" and tostring(get_chuky_jumped()) =="false" then
		set_chuky_jumped(true)
		ChuckyJump()
	end
end

function chuky_can_jump()
	set_chuky_jumped(false)
end	

function set_player_obstacle_area(is_inside)
	player.inside_obstacle_area = is_inside
end

function set_player_room(room, chucky_appears)
	set_player_zone(room ..".0")
	local num_room = tonumber(room)
	renderable_objects_layer_manager:change_between_vectors(piky_layer, "Piky", num_room)

	renderable_objects_layer_manager:change_between_vectors("solid","scriptedController", num_room)
	if tostring(chucky_appears) == "true" and chuky_last_room ~= num_room then
		renderable_objects_layer_manager:change_between_vectors("enemies", "Chucky",chuky_last_room, num_room)
		chuky_last_room = num_room
	end
		
	player_controller.m_Room = num_room
	
	if room == "5" then
		local trigger = trigger_manager:get_resource("Collectible3_UserData")
		activate_collectible(trigger, "collectible", "Collectible3")
	elseif room == "6" then
		local trigger1 = trigger_manager:get_resource("Collectible5_UserData")
		activate_collectible(trigger1, "collectible", "Collectible5")
	elseif room == "7" then
		local trigger = trigger_manager:get_resource("Collectible6_UserData")
		activate_collectible(trigger, "collectible", "Collectible6")
	end
end

function activate_wall_traps(room_number)
	if room_number == "0" then
		activate_wall_trap(tonumber(room_number), "WALL_TRAP1_RIGHT")
		activate_wall_trap(tonumber(room_number), "WALL_TRAP1_LEFT")
	end

end

-- Se encarga de comprovar si el Player está dentro y si le toca por tiempo pierde corazon
function check_arrows_trap(trap_name, trap_trigger_name)
	local trigger_trap = trigger_manager:get_resource(trap_trigger_name)
	local trap = search(trap_vector, trap_name)
	local trap_time = trap.time_out
	if trigger_trap.m_Time > trap_time +0.1 and trigger_trap.m_Time < trap_time +0.4 then--tonumber(damage_time) and trigger_trap.m_Time < tonumber(damage_time) + 0.3 then
		local player_position = player_controller:get_position()
		
		local axis = trap.damage_axis
		local direction = check_damage_direction(axis, player_position, trigger_trap:get_position() )
		player.player_take_damage(direction)
		--[[local damage_vector_x = player_position.x - trigger_trap:get_position().x
		local damage_vector_z = player_position.z - trigger_trap:get_position().z
		--]]
		--local trap = search(trap_vector, trap_name)
		--local damage_vector = Vect3f(damage_vector_x,player_position.y, damage_vector_z )
		--luaUtil:normalize(damage_vector)
	end
end

-- actualiza el tiempo para la trampa de flechas además de moverla
function update_arrows_trap(trigger_name, trap_name)
	local trap = search(trap_vector, trap_name)
	if trap ~= nil then
		local trigger = trigger_manager:get_resource(trigger_name)
		trigger.m_Time = trigger.m_Time +1 * coreInstance.m_ElapsedTime
		if trigger.m_Time > trap.time_out and trigger.m_Time < (trap.time_out + 1) then
			--coreInstance:trace("Trigger Trap Time: ".. tostring(trigger.m_Time))
			trap:move()
		elseif trigger.m_Time > trap.time_out +1 then
			trap:reset_position()
			--trap.m_Time = 0
			trigger.m_Time = 0
			--coreInstance:trace("Reset Trap")
		end		
	end
end

function reset_arrow_trap(trap_name, trigger_name)
	local trap = search(trap_vector, trap_name)
	trap:reset_position()
	local trigger = trigger_manager:get_resource(trigger_name)
	trigger.m_Time = 0
	
end

function set_infinite_player_attack(can_finish)
	player.can_finish_atack = toboolean(can_finish)
end

function trigger_zone4() 
	set_player_room(4, "false")
	trigger = trigger_manager:get_resource("Collectible2_UserData")
	activate_collectible(trigger, "collectible", "Collectible2")
end

function trigger_puzzle()
	set_puzzle_enemy_active("true");
	coreInstance:get_cinematic_controller():execute("Door");
end

function trigger_zone6(room_number, wall_name)
	cambiozona_2d_to_3d(room_number, wall_name)
	coreInstance:get_cinematic_controller():execute("PuzzleRoom");
end

function cambiozona_3d_to_2d(offset_axis, room_number)
	trigger_set_2D(offset_axis)
	set_player_room(room_number, "true")
	--activate_invisible_wall(wall_name, room_number)
	--set_checkpoint(trigger_name, renderable_object)
end

function cambiozona_2d_to_3d(room_number, wall_name)
	trigger_set_3D()
	set_player_room(room_number, "false")
	activate_invisible_wall(wall_name, room_number)
	
	--set_checkpoint(trigger_name, renderable_object)
end

function trigger_damage_player()
	player.player_take_damage(Vect3f(0,0,0))
end

function init_boss_trigger(a)
	--[[player_controller.m_is3D = true;
	local cam = coreInstance.m_CameraController:get_resource("3DCam");
	cam.m_eTypeCamera = 6;
	coreInstance.m_CameraController:set_active_camera("3DCam");
	start_boss()--]]
	local aux_renderable = coreInstance:get_renderable_object_layer_manager():get_renderable_objects_manager_by_str_and_room("animationLayerAux", 7):get_resource("Piky2");
	aux_renderable:set_visible(true)
	local player_renderable = coreInstance:get_renderable_object_layer_manager():get_renderable_objects_manager_by_str_and_room("player", 7):get_resource("Piky");
	if player_renderable ~= nil then
		player_renderable:set_visible(false)
		--player_renderable.m_Printable = false
	end
	--local final_position = Vect3f(position.x, position.y, position.z)
	--player_controller.m_PhysicController:set_position(final_position)
	--player_controller:set_position(final_position)
	coreInstance:get_cinematic_controller():execute("AngelFall");
end

function activate_angel_animation()
	set_player_room("7", "false")
	coreInstance:get_cinematic_controller():execute("AngelFallTest");
end
function nail_platform(obj_name, layer)
	--coreInstance:trace("Unsetting kinematic")
	local platform = get_renderable_object(layer, player_controller.m_Room, obj_name)
	if platform ~= nil and not platform.m_Nailed then
	--	coreInstance:trace("Platform found")
		platform.m_Nailed = true
		local position = platform:get_position()
		local emitter1 = particle_manager:get_resource("PlatformBrokenEmitter")
		local emitter1_pos = Vect3f(position.x -2, position.y +0.5, position.z)
		local emitter2 = particle_manager:get_resource("PlatformBroken2Emitter")
		local emitter2_pos = Vect3f(position.x +2, position.y +0.5, position.z)
		if emitter1 ~= nil then
			emitter1:set_visible(true)
			emitter1.m_vPos = emitter1_pos
			emitter1.m_FireParticles = true
		end
		if emitter2 ~= nil then
			emitter2:set_visible(true)
			emitter2.m_vPos = emitter2_pos
			emitter2.m_FireParticles = true
		end
		platform.m_Actor:set_kinematic(true)
		platform.m_Actor:activate(false)
	end
end

function activate_actor(obj_name, layer)
	local obj = get_renderable_object(layer, player_controller.m_Room, obj_name)
	if obj ~= nil then
		obj.m_Actor:activate(true)
		obj.m_Nailed = false
	end
end