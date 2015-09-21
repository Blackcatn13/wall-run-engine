--local coreInstance = CCoreLuaWrapper().m_CoreInstance
--local physx_manager = coreInstance:get_phisics_manager()

function change_object_position(_object, _position)
	--local switch_mesh = get_renderable_object("switches", trigger_name)
	--local new_position = Vect3f(switch_mesh:get_position().x,switch_mesh:get_position().y - 0.3,switch_mesh:get_position().z )
	_object:set_position(_position)
	_object.m_Actor:set_global_position(_position)
end

function get_distance_to_floor(_pos)
	local info = SCollisionInfo();
	local _dirRay = Vect3f(0,-1,0);
	local auxpos = Vect3f(_pos.x,_pos.y-0.9,_pos.z);
	coreInstance:get_phisics_manager():raycast_closest_actor(auxpos, _dirRay, 1, info, 1000);
	local floor_point = info.m_CollisionPoint;
	local distance = get_distance_between_points(floor_point,_pos);
	return distance;
end

function get_renderable_object(layer_name,room, _objectName)	
	local object_manager = renderable_objects_layer_manager:get_renderable_objects_manager_by_str_and_room(layer_name, room)
	local object = object_manager:get_resource(_objectName)
	return object
end

function open_door(_objectName)
	--coreInstance:trace("Opening door ".. _objectName)
	local door = get_renderable_object("puzzle",player_controller.m_Room, _objectName)
	if door ~= nil then
		--door.m_Printable = false
		local emitter = particle_manager:get_resource(door.m_ParticleEmitter)
		if emitter ~= nil then
			emitter.m_vPos = door:get_position()+ door.m_EmitterOffset
			emitter:set_yaw(door:get_yaw())
			emitter.m_FireParticles = true
		end
		coreInstance:trace("Door Final Position: " .. tostring(door.m_FinalPosition.y) )
		door.m_IsOpening = true
		inputm:set_game_pad_left_motor_speed(20000, 1);
		--door:set_position(door.m_FinalPosition)
		--door.m_Actor:set_global_position(door.m_FinalPosition)
		--door.m_Actor:activate(false)
	end
	--door.m_IsOpening =true
	--play animacion subir puerta
end

function open_door(_objectName, _objectName2)
	--coreInstance:trace("Opening door ".. _objectName.. " and " .. _objectName2 )
	local door = get_renderable_object("puzzle",player_controller.m_Room, _objectName)
	inputm:set_game_pad_left_motor_speed(20000, 1);
	if door ~= nil then
		--door.m_Printable = false
		local emitter = particle_manager:get_resource(door.m_ParticleEmitter)
		if emitter ~= nil then
			emitter.m_vPos = door:get_position()+ door.m_EmitterOffset
			emitter:set_yaw(door:get_yaw())
			emitter.m_FireParticles = true
		end
		coreInstance:trace("Door Final Position: " .. tostring(door.m_FinalPosition.y) )
		door.m_IsOpening = true
		if door.m_Speed == 0.0 then
			door:set_position(door.m_FinalPosition)
			door.m_Actor:set_global_position(door.m_FinalPosition)
			door.m_Actor:activate(false)
		end
	end
	local door2 = get_renderable_object("puzzle",player_controller.m_Room, _objectName2)
	if door2 ~= nil then
		--door.m_Printable = false
		coreInstance:trace("Door Final Position: " .. tostring(door2.m_FinalPosition.y) )
		door2.m_IsOpening = true
		if door2.m_Speed == 0.0 then
			door2:set_position(door2.m_FinalPosition)
			door2.m_Actor:set_global_position(door2.m_FinalPosition)
			door2.m_Actor:activate(false)
		end
	end
	--door.m_IsOpening =true
	--play animacion subir puerta
end

function close_door(_objectName, room)
	--coreInstance:trace("Opening door ".. _objectName)
	local door = get_renderable_object("puzzle",room, _objectName)
	
	if door ~= nil then
		--door.m_Printable = false
		coreInstance:trace("Door Final Position: " .. tostring(door.m_OriginalPosition.y) )
		if door.m_IsOpening then
			door.m_IsOpening = false
		end
		if door.m_Speed == 0.0 then
			door:set_position(door.m_OriginalPosition)
			door.m_Actor:set_global_position(door.m_OriginalPosition)
			--door.m_Actor:activate(false)
		end
	end
	--door.m_IsOpening =true
	--play animacion subir puerta
end

function close_door(_objectName, _objectName2, room)
	--coreInstance:trace("Opening door ".. _objectName.. " and " .. _objectName2 )
	local door = get_renderable_object("puzzle",room , _objectName)
	
	if door ~= nil then
		--door.m_Printable = false
		if door.m_IsOpening then
			door.m_IsOpening = false
		end
		coreInstance:trace("Closing Door Final Position: " .. tostring(door.m_OriginalPosition.y) )
		door:set_position(door.m_OriginalPosition)
		door.m_Actor:set_global_position(door.m_OriginalPosition)
		--door.m_Actor:activate(false)
	end
	local door2 = get_renderable_object("puzzle",room, _objectName2)
	
	if door2 ~= nil then
		--door.m_Printable = false
		if door2.m_IsOpening then
			door2.m_IsOpening = false
		end
		coreInstance:trace("Closing Door Final Position: " .. tostring(door2.m_OriginalPosition.y) )
		door2:set_position(door2.m_OriginalPosition)
		door2.m_Actor:set_global_position(door2.m_OriginalPosition)
		--door.m_Actor:activate(false)
	end
	--door.m_IsOpening =true
	--play animacion subir puerta
end



function get_distance_between_points(current_position, _player_position)
	-- calcular distancia hacia player
	local distance = 0
	distance = ((_player_position.x - current_position.x) ^2 + (_player_position.y - current_position.y) ^2 + (_player_position.z - current_position.z) ^2)
	return distance
end

function clear_array(Array_Name)
	for i = 1, table.getn(Array_Name) do
		table.remove(Array_Name)
	end
end

function search(vector, name)
	for i = 1, table.getn(vector) do
		if vector[i].name == name then
			return vector[i]
		end
	end
end

function split_str(inputstr, sep)
        if sep == nil then
                sep = "%s"
        end
        local t={} ; i=1
        for str in string.gmatch(inputstr, "([^"..sep.."]+)") do
				--coreInstance:trace(str)
                t[i] = str
                i = i + 1
        end
        return t
end

function check_damage_direction (axis, player_direction, other_direction)
	local direction = Vect3f(0,0,0)
	if axis == "x" then
		direction.x = check_position(player_direction.x, other_direction.x, 0.5)
	elseif axis == "z" then
		direction.z = check_position(player_direction.z, other_direction.z, 0.5)	
	end
	return direction
end

function check_position (position1, position2, margin)
	if (position1 + margin > position2) then
		return margin
	else
		return -margin
	end
	return 0
end

function toboolean(string_to_parse)
	if string_to_parse == "true" or string_to_parse=="TRUE" then
		return true
	else
		return false
	end
	return 0
end
function reset_puzzle(puzzle_name, room, door1, door2)
	close_door(door1, door2, 5);
	renderable_objects_layer_manager:get_renderable_objects_manager_by_str_and_room("puzzle", room):reset_puzzle_renderables()
	puzzle_manager:reset_puzzle_triggers(puzzle_name)
end

function reset_game()
	
	--CheckPoints
	reset_checkpoints()
	
	--Player Position + 3D
	player.reset_position()
	player_controller.m_is3D = true;
	local cam = coreInstance.m_CameraController:get_resource("3DCam");
	cam.m_eTypeCamera = 6;
	cam.m_currentWaypoint = 0;
	cam.m_nextWaypoint = 1;
	coreInstance.m_CameraController:set_active_camera("3DCam");
	player_controller.m_isTurned = false;
	player.attack_enabled = false
	--player.set_room(0)
	set_player_room(1, true)
	reset_crushing_sides()
		
	--Player Lifes + Pixelites
	player.num_hearts = 3;
	gui_manager:set_image('LifeGUI','Life3')
	player.num_lives = 3;
	--player.visited_checkpoints = {}
	
	player.zone = 1
	gui_manager:set_is_displayed_heart(true);
	gui_manager:set_count_heart(0.0);
	gui_manager:set_num_heart( 3 );	
	player.pixelites = 0;
	gui_manager:set_is_displayed_pixelite(true);
	gui_manager:set_count_pixelite(0.0);
	gui_manager:set_num_pixelite( 0 );	
	
	--Pixelites and cards
	collectible_manager:reset_collectibles();
	player.enemy_puzzle_active = false
	
	--Enemies
	player.enemies_killed = 0;
	enemy_manager:reload_enemies();
	ChuckyDesapears()
	
	--Invisible walls
	renderable_objects_layer_manager:deactivate_objects_by_layer("invisible");
	
	--puzzle
	reset_puzzle("puzzle001", 6, "Puerta_arriba002", "Puerta_abajo002")
	--Doors
	close_door("Puerta_arriba", "Puerta_abajo", 3);
	
	--Traps
	reset_wall_trap(0, "WALL_TRAP1_RIGHT")
	reset_wall_trap(0, "WALL_TRAP1_LEFT")
	local trap = search(trap_vector, "ArrowTrap1")
	--trap:reset_position()
	 restore_broken_platforms_by_layer("breakable")
	
	--
	--Others
	gui_manager:activate_pause(false);
	gui_manager:set_is_paused(false);
	gui_manager:set_is_displayed_poly_message(false);
	gui_manager:set_is_displayed_unlock_message(false);
	gui_manager:set_first_poly_message_displayed(false);
	gui_manager:set_visibility_message(false);
	--gui_manager:set_visible_gui_element("LifeGUI", true)
	--gui_manager:set_visible_gui_element("VidesGUI", true)
	set_visible_gui_elements(true)
	coreInstance:trace("Game Reset")
	local emitter = particle_manager:get_resource("DustPikyEmitter")
	if emitter:get_visible() == true then
		emitter:set_visible(false)
	end
end

