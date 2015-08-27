function activate_teleport(player_position)
	--local player = coreInstance:get_player_controller()
	coreInstance:trace(player_position)
	local vec_array = split_str(player_position, "%s")
	local vec3f_array = Vect3f(tonumber(vec_array[1]), tonumber(vec_array[2]), tonumber(vec_array[3]))
	player_controller.m_PhysicController:set_position(vec3f_array)
	local camera = coreInstance.m_CameraController:get_active_camera()
	--	coreInstance:trace("laaaaaa")
	camera.m_pObject3D:set_position(Vect3f(camera.m_pObject3D:get_position().x, camera.m_pObject3D:get_position().y, vec3f_array.z - 10))
	local cam_object =  camera.m_pObject3D
	local PlayerYaw =  - cam_object:get_yaw() + 1.57
	if player_controller.m_is3D == true then
		player_controller:set_yaw(PlayerYaw)
	else
		player_controller.m_isTurned = false
		player_controller:set_yaw(PlayerYaw + 1.57); -- 90ยบ
	end
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
				local switch_mesh = get_renderable_object("puzzle",player_controller.m_Room, trigger_name)
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
	coreInstance:trace("Trigger update " .. tostring(trigger.m_Update))
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
	player.has_ass_burned = true
	player.player_die()
end

function set_player_respawn_spikes_point()
	--local player = Player:get_instance()
	player.last_spikes_position = Vect3f(player.get_player_controller():get_position())
end

function set_checkpoint(trigger_name, renderable_object)
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
		coreInstance:trace(renderable_object)
		local checkpoint_mesh = get_renderable_object("solid", player_controller.m_Room, renderable_object)
		if checkpoint_mesh == nil then
			checkpoint_mesh = get_renderable_object("solid", player_controller.m_Room +1, renderable_object)
			if checkpoint_mesh == nil then -- Para el caso del fallo aleatorio en el reset
				checkpoint_mesh = get_renderable_object("solid", 1, renderable_object)
			end
		end
		checkpoint.set_checkpoint(player, trigger_name, checkpoint_mesh)
		coreInstance:trace(tostring(checkpoint.is_activated))
		coreInstance:trace("checkpoint activated")
	end
end

function test_on_enter()
	coreInstance:trace("Entrando en trigger")
end

function test_on_exit( param)
	coreInstance:trace("Saliendo de trigger " .. param)
end

function test_on_stay( param)
	coreInstance:trace("Dentro de trigger ".. param)
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
	coreInstance:trace("Enemy puzle active ".. tostring(player.enemy_puzzle_active))
end

function activate_invisible_wall(name)
	local wall = get_renderable_object("invisible",player_controller.m_Room, name)
	if wall ~= nil then
		wall:activate_phisic(true)
	end
end

function deactivate_invisible_wall(name)
	local wall = get_renderable_object("invisible",player_controller.m_Room, name)
	if wall ~= nil then
		wall:activate_phisic(false)
	end
end

function check_enemies_killed(num1, door)
	if player.enemy_puzzle_active then
		coreInstance:trace("----------------------------Enemies: ".. tostring(player.enemies_killed))
		if player.enemies_killed == tonumber(num1) then
			-- Aparece el collectible
			coreInstance:trace("----------------------------Cromo is here!!")
			local trigger_name ="Collectible1_UserData"
			local trigger = trigger_manager:get_resource(trigger_name)
			if(trigger ~= nil) then
				activate_collectible(trigger, "collectible", "Collectible1")
			end
			player.enemy_puzzle_active = false
		end
		if player.enemies_killed == 2 then
			coreInstance:trace("----------------------------Opening door")
			open_door(door)
		end
	end
end

function check_enemies_killed(num1, door, door2)
	if player.enemy_puzzle_active then
		coreInstance:trace("----------------------------Enemies: ".. tostring(player.enemies_killed))
		if player.enemies_killed == tonumber(num1) then
			-- Aparece el collectible
			coreInstance:trace("----------------------------Cromo is here!!")
			local trigger_name ="Collectible1_UserData"
			local trigger = trigger_manager:get_resource(trigger_name)
			if(trigger ~= nil) then
				activate_collectible(trigger, "collectible", "Collectible1")
			end
			player.enemy_puzzle_active = false
		end
		if player.enemies_killed == 2 then
			coreInstance:trace("----------------------------Opening door")
			open_door(door, door2)
		end
	end
end

function ChuckyApear(distance)
coreInstance:trace("CHUKY APEARS!!!");
local Chucky = enemy_manager:get_enemy("Chucky");
	if player.activating_triggers and not Chucky.m_Appeared then
		
		local active_camera = cam_Controller:get_active_camera();
		local dirVec = active_camera:get_path_point(active_camera.m_nextWaypoint) - active_camera:get_path_point(active_camera.m_currentWaypoint);
		dirVec.y = 0;
		dirVec:normalize(1);
		local current_position = Vect3f(player.get_player_controller():get_position());
		current_position = current_position - dirVec * tonumber(distance);
		local pos = Vect3f(current_position.x,current_position.y + 4,current_position.z);
		Chucky:move_to_position(pos);
		local yaw = coreInstance:get_renderable_object_layer_manager():get_renderable_objects_manager_by_str_and_room("player", player_controller.m_Room):get_resource("Piky"):get_yaw();
		Chucky.m_RenderableObject:set_yaw(yaw);
		Chucky.m_Appeared = true
		set_chucky_need_to_jump(false)
		Chucky:m_FSM():newState("Corriendo");
		Chucky:set_visible(true)
	end
end

function ChuckyDesapears()
	coreInstance:trace("CHUKY goes out");
	local Chucky = enemy_manager:get_enemy("Chucky");
	if(Chucky.m_Appeared == true) then
		local pos = Vect3f(12.30, 0.0, -2.75)
		Chucky:move_to_position(pos);
		Chucky:m_FSM():newState("Parado");
		Chucky.m_Appeared = false
		local emitter = particle_manager:get_resource(Chucky.m_RenderableObject.m_ParticleEmitter)
		emitter:set_visible(false)
	end
end

function ChuckyJump()
	--coreInstance:trace("Chucky Jumping");
	local Chucky = enemy_manager:get_enemy("Chucky");
	Chucky:m_FSM():newState("Saltando");
	set_chucky_need_to_jump(true)
end

function chucky_check_jump()
	if tostring(player.inside_obstacle_area) == "false" and tostring(get_chuky_jumped()) =="false" then
		coreInstance:trace("Chuky Check Jump Correct")
		set_chuky_jumped(true)
		ChuckyJump()
	end
end

function chuky_can_jump()
	set_chuky_jumped(false)
end	

function set_player_obstacle_area(is_inside)
	coreInstance:trace("Player Inside Obstacle area".. tostring(is_inside))
	player.inside_obstacle_area = is_inside
end

function set_player_room(room, chucky_appears)
	set_player_zone(room ..".0")
	local num_room = tonumber(room)
	renderable_objects_layer_manager:change_between_vectors("player", "Piky", num_room)

	renderable_objects_layer_manager:change_between_vectors("solid","scriptedController", num_room)
	if chucky_appears == "true" and chuky_last_room ~= num_room then
		renderable_objects_layer_manager:change_between_vectors("enemies", "Chucky", num_room)
		chuky_last_room = num_room
	end
	
	player_controller.m_Room = num_room
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
		coreInstance:trace("Hit")
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