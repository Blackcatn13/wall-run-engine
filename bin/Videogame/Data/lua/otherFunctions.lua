--local coreInstance = CCoreLuaWrapper().m_CoreInstance
--local physx_manager = coreInstance:get_phisics_manager()
math.randomseed( os.time() )
math.random(); math.random(); math.random()
function show_cinematic_particles(particle_emitters, visible)
	if (visible and not particle_emitters[1]:get_visible()) or not visible then
		set_emitters_visible(particle_emitters, visible)
	end
end

function fire_cinematic_particles(array_emitters)
	for i =1, table.getn(array_emitters) do
		if array_emitters[i] ~= nil then
			array_emitters[i]:set_visible(true)
			array_emitters[i].m_FireParticles = true
		end
	end
end

function fade(fade_time)
	gui_manager:start_fade(fade_time)
end

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

function set_emitters_visible(array_emitters, visible)
	for i =1, table.getn(array_emitters) do
		if array_emitters[i] ~= nil then
			array_emitters[i]:set_visible(visible)
		end
	end

end

function open_door(_objectName)
	--coreInstance:trace("Opening door ".. _objectName)
	local door = get_renderable_object("puzzle",player_controller.m_Room, _objectName)
	if door ~= nil then
		--door.m_Printable = false
		local emitter = particle_manager:get_resource(door.m_ParticleEmitter)
		if emitter ~= nil then
			emitter:set_visible(true)
			emitter.m_vPos = door:get_position()+ door.m_EmitterOffset
			emitter:set_yaw(door:get_yaw())
			emitter.m_FireParticles = true
		end
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
	coreInstance:getWWSoundManager():PlayEvent("Door_opening", "Door");
	if door ~= nil then
		--door.m_Printable = false
		local emitter = particle_manager:get_resource(door.m_ParticleEmitter)
		if emitter ~= nil then
			emitter:set_visible(true)
			emitter.m_vPos = door:get_position()+ door.m_EmitterOffset
			emitter:set_yaw(door:get_yaw())
			emitter.m_FireParticles = true
		end
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
		door2:set_position(door2.m_OriginalPosition)
		door2.m_Actor:set_global_position(door2.m_OriginalPosition)
		--door.m_Actor:activate(false)
	end
	--door.m_IsOpening =true
	--play animacion subir puerta
end

function check_shoot_collision(enemy, mesh, controller) 
	if (mesh.m_Printable and enemy.m_PosicionBala:distance(controller:get_position()) < enemy.m_ProjectileHitbox) then
		return true
	end
  return false
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
	close_door(door1, door2, room);
	renderable_objects_layer_manager:get_renderable_objects_manager_by_str_and_room("puzzle", room):reset_puzzle_renderables()
	puzzle_manager:reset_puzzle_triggers(puzzle_name)
end

function reset_game()
	
	--CheckPoints
	reset_checkpoints()
	
	--Player Position + 3D
	
	player.set_super_piky(false)
	player.super_piky_timer = 0.0
	
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
	player.num_lifes = 3;
	player.count_pixelites = 0.0
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
	reset_cards()
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
	close_door("Puerta_arriba002", "Puerta_abajo002", 6);
	
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
	player.stickers = 0
	gui_manager:set_first_poly_message_displayed(false);
	gui_manager:set_visibility_message(false);
	activate_invisible_wall("INVISIBLE_WALL_001",1)
	--gui_manager:set_visible_gui_element("LifeGUI", true)
	--gui_manager:set_visible_gui_element("VidesGUI", true)
	set_visible_gui_elements(true)
	
	local emitter = particle_manager:get_resource("DustPikyEmitter")
	if emitter:get_visible() == true then
		emitter:set_visible(false)
	end
	--CINEMATICS
	--cinematic_controller:restart("start")
	--cinematic_controller:restart("Door")
	cinematic_controller:restart_all_cinematics()
	--BOSS
	player.set_super_piky(false)
	player.super_piky_attack = false
	player.super_piky_timer = 0.0
	set_boss_polis_visible(false)
	local cadira = get_renderable_object("solid",0, "CADIRA")
	cadira:set_visible(true)
	get_renderable_object("solid",0, "ORO2"):set_visible(true)
	get_renderable_object("solid",0, "ORO3"):set_visible(true)
	get_renderable_object("solid",0, "ORO004"):set_visible(false)
	get_renderable_object("solid",0, "ORO1"):set_visible(false)
	
	get_renderable_object("solid",0, "PilarQuad001"):set_visible(true)
	get_renderable_object("solid",0, "PilarQuad002"):set_visible(true)
	--get_renderable_object("solid",0, "ChukyBossPosition"):set_visible(false)
	local ruinas = get_renderable_object("solid",0, "Ruinas")
	particle_manager:get_resource("SuperPikyAuraEmitter"):set_visible(false)
	particle_manager:get_resource("SuperPikyAura2Emitter"):set_visible(false)
	ruinas:set_visible(false)
	ruinas.m_Printable = false
	get_renderable_object("solid",0, "ORO006"):set_visible(false)
	get_renderable_object("solid",0, "ORO005"):set_visible(false)

	local array_mik = {"MikMik007", "MikMik007", "MikMik008", "MikMik009", "MikMik010", "MikMik011"}

	for i = 1, table.getn(array_mik) do
		enemy_manager:get_enemy(array_mik[i]):m_FSM():newState("Waiting")
		--sound_manager:UnregisterGameObject(array_mik[i])
	end
	all_boss_miks_killed = true
	boss_miks_killed = 0
	local boss = enemy_manager:get_enemy(boss_mesh_name)
	boss.m_RenderableObject:set_visible(false)
	--if boss.m_BossRunning then
		boss.m_Phases = boss.m_OriginalPhases
		boss.m_Lifes = boss.m_OriginalLifes
		boss.m_BossRunning = false
	--end
	--boss_dead = false
	get_renderable_object("animationLayerAux",7, "Piky2"):set_visible(false)
end

