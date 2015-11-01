local boss_timer = 0.0
local contador = 0
local executedReturn = false;
local execute_hurt = false;
local doing_hurt = false;
local min_return_factor = 7
local return_factor = min_return_factor
local max_return_times = 3
local max_timer = 4.0
local current_max_timer = max_timer
local dust_particles_fired = false
local boss_dead = false

local current_sequence =  {}
local last_action_id = 1
local actions = {{"mik", "sp","roca","roca"}, 
{"mik", "roca","sp", "roca"},
{"mik","roca", "roca","sp"},
{"mik","sp", "roca","mik","roca"},
{"mik","roca", "sp","mik","roca"},
{"mik","roca", "roca","mik","sp"},
{"mik","sp", "mik","roca","roca"},
{"mik","roca", "mik","sp","roca"},
{"mik","roca", "mik","roca","sp"},}

local array_mik = {"MikMik007", "MikMik008", "MikMik009", "MikMik010", "MikMik011"}

local mik_positions = {{Vect3f(31.434, 1, -16.009), Vect3f(26.444, 1, -8.715), Vect3f(15.389, 2.288, -0.857), Vect3f(26.137, 1, 7.399), Vect3f(30.282, 1, 16.712)}, 
{Vect3f(15.914, 1, 25.021), Vect3f(-20.02, 1, 20.878), Vect3f(9.836, 1, 34.8), Vect3f(-12.751, 1, 30.243), Vect3f(-1.255, 2.288, 15.628)},
{Vect3f(-28.807, 1, 14.852), Vect3f(-27.51, 1, 3.497), Vect3f(-27.153, 1, -17.837), Vect3f(-17.463, 2.288, -5.043), Vect3f(-33.338, 1, -7.951)},
{Vect3f(-18.206, 1,-30.661), Vect3f(-7.39, 1, -26.59), Vect3f(15.948, 1, -22.901), Vect3f(7.94, 1, -35.235), Vect3f(0.409, 2.288, -15.715)}} --{Section{MikPos},{MikPos2}...},{Section2{MikPos},{MikPos2}...}

function get_room_section()
	local poly1 = renderable_objects_layer_manager:get_resource_from_layers_and_room("poly", "enabled_poly", "Poly_Sala0_003", 0)
	local poly2 = renderable_objects_layer_manager:get_resource_from_layers_and_room("poly", "enabled_poly", "Poly_Sala0_002", 0)
	local poly3 = renderable_objects_layer_manager:get_resource_from_layers_and_room("poly", "enabled_poly", "Poly_Sala0_004", 0)
	local poly4 = renderable_objects_layer_manager:get_resource_from_layers_and_room("poly", "enabled_poly", "Poly_Sala0_001", 0)
	local array_polys = {poly1, poly2, poly3, poly4}
	
	local min_distance = nil
	local current_section = 0
	for i = 1, table.getn(array_polys) do
		local temp_distance = get_distance_between_points(array_polys[i]:get_position(), player_controller:get_position())
		if min_distance == nil or temp_distance < min_distance then
			min_distance = temp_distance
			current_section = i
		end
	end
	return current_section
end

function switch_boss_layer(layer)
	local old_layer = chucky_boss_layer
	chucky_boss_layer = layer
	renderable_objects_layer_manager:change_between_layers(old_layer, chucky_boss_layer, 0, boss_mesh_name)
	coreInstance:trace("Switch Boss Layer from " .. old_layer.. " to "..chucky_boss_layer)
end

function start_boss()
	set_player_room("0", false)
	cam_Controller:set_active_camera("BossCam")
	local activeCam = cam_Controller:get_active_camera()
	coreInstance:getWWSoundManager():PlayEvent("_BossZone", "Music");
	
	local position = Vect3f(30,1,2)
	player_controller.m_PhysicController:set_position(position)
	local playerRenderableMesh = renderable_objects_layer_manager:get_renderable_objects_manager_by_str_and_room(piky_layer, 0):get_resource(piky_mesh_name);
	playerRenderableMesh:set_position(position)
	
	if chucky_boss_layer ~= "enemies" then
		switch_boss_layer("enemies")
	end
	
	local chucky = enemy_manager:get_enemy(boss_mesh_name)
	local chucky_position = Vect3f(0.0, 4.48, 0.0)
	--chucky:set_position(chucky_position)
	chucky.m_RenderableObject:set_position(chucky_position)
	chucky.m_RenderableObject:set_visible(true)
	local objCam = activeCam.m_pObject3D;
	objCam:set_position(chucky_position);
	local cadira = get_renderable_object("solid",0, "CADIRA")
	--get_renderable_object("solid",0, "CADIRA"):set_visible(false)
	if cadira:get_visible() then
		cadira:set_visible(false)
		get_renderable_object("solid",0, "ORO2"):set_visible(false)
		get_renderable_object("solid",0, "ORO3"):set_visible(false)
		get_renderable_object("solid",0, "ORO1"):set_visible(false)
		get_renderable_object("solid",0, "ORO004"):set_visible(false)
		
		get_renderable_object("solid",0, "PilarQuad001"):set_visible(false)
		get_renderable_object("solid",0, "PilarQuad002"):set_visible(false)
		--get_renderable_object("solid",0, "ChukyBossPosition"):set_visible(true)
		get_renderable_object("solid",0, "Ruinas"):set_visible(true)
		get_renderable_object("solid",0, "ORO006"):set_visible(true)
		get_renderable_object("solid",0, "ORO005"):set_visible(true)
	end 
	chucky:m_FSM():newState("Parado")
	return_factor = min_return_factor
	returned_times = 0
	current_max_timer = max_timer
	set_boss_polis_visible(true)
	all_boss_miks_killed = true
	boss_miks_killed = 0
	chucky.m_BossRunning = true

	for i = 1, table.getn(array_mik) do
		enemy_manager:get_enemy(array_mik[i]):m_FSM():newState("Waiting")
		sound_manager:RegisterGameObject(array_mik[i])
	end

	if player.super_piky_active then
		player.set_super_piky(false)
		--player.super_piky_timer = 0.0
	end
	player.super_piky_timer = 0.0
	particle_manager:get_resource("SuperPikyAuraEmitter"):set_visible(false)
	particle_manager:get_resource("SuperPikyAura2Emitter"):set_visible(false)
	chucky.m_Phases = chucky.m_OriginalPhases
	coreInstance:trace("Vidas Boss" .. tostring(chucky.m_OriginalLifes))
	chucky.m_Life = chucky.m_OriginalLifes
	last_action_id = 1
	current_sequence =  {}
	dust_particles_fired = false
	set_checkpoint("boss_checkpoint", nil)
	player.attack_enabled = true
	boss_dead = false
end


function set_boss_polis_visible(visible)
	local poly1 = renderable_objects_layer_manager:get_resource_from_layers_and_room("poly", "enabled_poly", "Poly_Sala0_001", 0)
	local poly2 = renderable_objects_layer_manager:get_resource_from_layers_and_room("poly", "enabled_poly", "Poly_Sala0_002", 0)
	local poly3 = renderable_objects_layer_manager:get_resource_from_layers_and_room("poly", "enabled_poly", "Poly_Sala0_003", 0)
	local poly4 = renderable_objects_layer_manager:get_resource_from_layers_and_room("poly", "enabled_poly", "Poly_Sala0_004", 0)
	local array_polys = {poly1, poly2, poly3, poly4}
	
	
	for i = 1, table.getn(array_polys) do
		array_polys[i]:set_visible(visible)
		array_polys[i].m_Printable = visible
	end
end

function summon_mik(mik_name)
	local enemy = enemy_manager:get_enemy(mik_name)
	if not enemy.m_isAlive then
		enemy.m_isAlive = true
		enemy.m_RenderableObject.m_Printable =true
		enemy.m_RenderableObject:set_visible(true)
		enemy_set_alive(mik_name)
	else
		enemy.m_RenderableObject.m_Printable =true
		enemy.m_RenderableObject:set_visible(true)
	end
	enemy:m_FSM():newState("Parado")
end

function fire_mik_summon_particles(mik_name)
	local enemy = enemy_manager:get_enemy(mik_name)
	local emitter = particle_manager:get_resource(enemy.m_RenderableObject.m_ParticleEmitter)
	if emitter ~= nil then
		local position = Vect3f(enemy.m_RenderableObject:get_position().x, enemy.m_RenderableObject:get_position().y, enemy.m_RenderableObject:get_position().z) 
		if position.y > 500 then
			position.y = position.y -1000
		end
		emitter.m_vPos = position + enemy.m_RenderableObject.m_EmitterOffset
		emitter.m_FireParticles = true 
	end
end

function start_super_piky()
	local renderable = get_renderable_object(piky_layer, player_controller.m_Room, piky_mesh_name)
	local emitter4 = particle_manager:get_resource(renderable.m_ParticleEmitter4)
	emitter4.m_vPos = renderable:get_position()
	emitter4:set_visible(true)
	renderable:execute_action(anim_poly,0.1,0,1,false)
	transition_super_piky = true		

end
---- IA----
----PARADO----
function chucky_boss_enter_stopped(name)
	coreInstance:trace("Estado Parado")
	local enemy = enemy_manager:get_enemy(name)
	enemy.m_RenderableObject:remove_action(5)
	enemy.m_RenderableObject:blend_cycle(0,0.1,0);
end

function chucky_boss_exit_stopped(name)
	local enemy = enemy_manager:get_enemy(name)
	enemy.m_RenderableObject:clear_cycle(0,0.3);
	boss_timer = 0.0
end

function chucky_boss_update_stopped(ElapsedTime, doComprobation, name)
	local enemy = enemy_manager:get_enemy(name)
	local player_position = player_controller:get_position()
	--local player_distance = get_distance_to_player(chucky:get_position(), player_position)
	
	rotate_yaw(enemy, ElapsedTime, player_position)
	
	boss_timer = boss_timer + (1 * ElapsedTime)
	if boss_timer >= current_max_timer then
		--[[local check_call_miks = false 
		if enemy.m_Phases == 1 and all_boss_miks_killed then
			check_call_miks = check_random_action (6)
		end
		
		if check_call_miks then
			enemy:m_FSM():newState("Llamar")
		else
			enemy:m_FSM():newState("Lanzar")
		end]]
		local action = call_next_action()
		if action == "mik" then
			if enemy.m_Phases == 1 and all_boss_miks_killed then
				enemy:m_FSM():newState("Llamar")
			else
				action = call_next_action()
			end
		end
		if action =="sp" then
			if player.super_piky_active then
				action = "roca"
			else
				current_shot_type = "powerup"
				enemy:m_FSM():newState("Lanzar")
			end
		end
		if action == "roca" then
			current_shot_type = "rock"
			enemy:m_FSM():newState("Lanzar")
		end
	end
	
end

function call_next_action() 
	
	if next(current_sequence) == nil or last_action_id > table.getn(current_sequence) then
		local rand = math.random(table.getn(actions)) 
		current_sequence = actions[rand]
		
		coreInstance:trace("CurrentSequence "..tostring(rand) )
		last_action_id = 1
	end

	local ret = current_sequence[last_action_id]
	last_action_id = last_action_id +1
	return ret

end


----LANZAR----
function chucky_boss_enter_shoot(name)
	coreInstance:trace("Estado Lanzar")
	boss_projectile_returned = false
	boss_projectile_returned_by_chucky = false;
	local enemy = enemy_manager:get_enemy(name)
	--local player_position = player_controller:get_position()
	enemy.m_RenderableObject:execute_action(5,0.1,0,1,true);
end

function chucky_boss_exit_shoot(name)
	
end

function chucky_boss_update_shoot(ElapsedTime, doComprobation, name)
--	coreInstance:trace("Boss disparandooo")
	local player_position = player_controller:get_position()
	local enemy = enemy_manager:get_enemy(name)
	rotate_yaw(enemy, ElapsedTime, player_position)
--	coreInstance:trace(tostring(enemy.m_RenderableObject:get_animation_time()))
	if enemy.m_RenderableObject:get_animation_time() > 2 then
		update_boss_shoot_cooldown(enemy, player_position)
	end
	if (enemy ~= nil) then
		--enemy:actualizar_disparo(ElapsedTime)	

		if boss_projectile_returned or boss_projectile_returned_by_chucky then
			update_horizontal_boss_shoot(ElapsedTime, enemy)
		else
			update_shoot_boss(ElapsedTime, enemy)
		end
		--enemy:actualizar_hitbox()

	end
	if not enemy.m_RenderableObject:is_action_animation_active() then
		enemy:m_FSM():newState("EsperandoImpacto")
	end
end


----DEVOLVER----
function chucky_boss_enter_return(name)
	coreInstance:trace("Devuelvo");
	local enemy = enemy_manager:get_enemy(name)
	contador = 0;
	executedReturn = false;
	enemy.m_RenderableObject:remove_action(5);
	enemy.m_RenderableObject:clear_cycle(0,0.1);
	enemy.m_RenderableObject:execute_action(9,0.1,0,1,true);				
	returned_times = returned_times +1
end

function chucky_boss_exit_return(name)
	local enemy = enemy_manager:get_enemy(name)
	enemy.m_RenderableObject:remove_action(9)
	contador = 0;
	executedReturn = false;
end

function chucky_boss_update_return(ElapsedTime, doComprobation, name)
	local enemy = enemy_manager:get_enemy(name)
	if (enemy ~= nil) then
		contador = contador + ElapsedTime;
		local enemyPosXZ = Vect3f(enemy:get_position().x, 0, enemy:get_position().z);
		local balaPosXZ = Vect3f(enemy.m_PosicionBala.x, 0, enemy.m_PosicionBala.z);
		local distance_to_bala = get_distance_between_points(enemyPosXZ, balaPosXZ);
		if not executedReturn and distance_to_bala < 7 then
			boss_projectile_returned = false
			boss_projectile_returned_by_chucky = true
			local enemyPos = Vect3f(enemy:get_position().x, enemy:get_position().y + 2, enemy:get_position().z);
			enemy.m_DireccionBala = player_controller:get_position() - enemyPos;
			if actual_speed_change < max_speed_change then
				local maxReached = false
				if actual_speed_change + current_speed_change > max_speed_change then
					current_speed_change = max_speed_change - actual_speed_change;
					maxReached = true;
				end
				enemy.BalaSpeed = enemy.BalaSpeed + current_speed_change;
				if maxReached then
					actual_speed_change = max_speed_change;
				else
					actual_speed_change = actual_speed_change + current_speed_change;
					current_speed_change = current_speed_change / 2;
				end
			end
			executedReturn = true;
		end
		if boss_projectile_returned or boss_projectile_returned_by_chucky then
			update_horizontal_boss_shoot(ElapsedTime, enemy)
		else
			update_shoot_boss(ElapsedTime, enemy)
		end
		if not enemy.m_RenderableObject:is_action_animation_active() and enemy.BalaActiva == false then
			enemy:m_FSM():newState("Parado");
		end
		if boss_projectile_returned_by_chucky and not enemy.m_RenderableObject:is_action_animation_active() then
			enemy:m_FSM():newState("EsperandoImpacto")
		elseif executedReturn and not boss_projectile_returned_by_chucky and boss_projectile_returned and distance_to_bala < 7 then
			enemy:m_FSM():newState("Hurt")
			coreInstance:trace("HUUUUUUURT");
		end
	end
end
--WAITING--
function chucky_boss_enter_waiting(name)
	coreInstance:trace("Estado Esperando")
	local enemy = enemy_manager:get_enemy(name)
	enemy.m_RenderableObject:remove_action(5)
	enemy.m_RenderableObject:blend_cycle(0,1,0);
end

function chucky_boss_exit_waiting(name)
	local enemy = enemy_manager:get_enemy(name)
	--enemy.m_RenderableObject:clear_cycle(0,0.3);

end

function chucky_boss_update_waiting(ElapsedTime, doComprobation, name)
	local enemy = enemy_manager:get_enemy(name)
	if (enemy ~= nil) then

		local player_position = player_controller:get_position()
		rotate_yaw(enemy, ElapsedTime, player_position)
		if boss_projectile_returned or boss_projectile_returned_by_chucky then
			update_horizontal_boss_shoot(ElapsedTime, enemy)
		else
			update_shoot_boss(ElapsedTime, enemy)
		end
		--enemy:actualizar_hitbox()
		if boss_projectile_returned and get_distance_between_points(enemy:get_position(), enemy.m_PosicionBala) < 250 then
			if check_random_action(return_factor) and returned_times < max_return_times then
				enemy:m_FSM():newState("Devolver")
			else
				enemy:m_FSM():newState("Hurt")
			end
		end
		if enemy.BalaActiva == false then
			enemy:m_FSM():newState("Parado")
		end
	end
end

function check_random_action(bonus)
	local rand = tonumber(math.random(bonus))
	if rand >3  then
		return true
	end
	return false
end
----LLAMAR----
function chucky_boss_enter_call(name)
	coreInstance:trace("Estado Llamando Miks")
	local enemy = enemy_manager:get_enemy(name)
	if (enemy ~= nil) then
		enemy.m_RenderableObject:execute_action(7,0.1,0,1,true)
	end
end

function chucky_boss_exit_call(name)
	
end

function chucky_boss_update_call(ElapsedTime, doComprobation, name)
	local enemy = enemy_manager:get_enemy(name)
	if (enemy ~= nil) then
		if not enemy.m_RenderableObject:is_action_animation_active() then
			enemy.m_RenderableObject:remove_action(7)
			local array_pos = mik_positions[get_room_section()] 
			for i = 1, table.getn(array_mik) do
				summon_mik(array_mik[i])
				enemy_manager:get_enemy(array_mik[i]):move_to_position(array_pos[i])
				--coreInstance:trace("Mik ".. array_mik[i] .. " position: " .. tostring(enemy_manager:get_enemy(array_mik[i]):get_position().x) .. ", "..tostring(enemy_manager:get_enemy(array_mik[i]):get_position().y).. ", "..tostring(enemy_manager:get_enemy(array_mik[i]):get_position().z))
				fire_mik_summon_particles(array_mik[i])
				coreInstance:trace("Mik ".. array_mik[i] .. " final position: " .. tostring(enemy_manager:get_enemy(array_mik[i]):get_position().x) .. ", "..tostring(enemy_manager:get_enemy(array_mik[i]):get_position().y).. ", "..tostring(enemy_manager:get_enemy(array_mik[i]):get_position().z))
			end
			all_boss_miks_killed = false
			boss_miks_killed = 0
			dust_particles_fired = false
			enemy:m_FSM():newState("Parado")
		elseif enemy.m_RenderableObject:get_animation_time() > 1.2 and not dust_particles_fired then
			dust_particles_fired = true
			local enemyYaw = enemy:get_yaw() + (math.pi / 2);
			local vectRight = Vect3f(math.cos(enemyYaw), 0, -math.sin(enemyYaw));
			local position = enemy:get_position()
			local emitter_position = Vect3f(position.x + 1 * vectRight.x, position.y, position.z + 1 * vectRight.z)	
			local dust_emitter = particle_manager:get_resource(enemy.m_RenderableObject.m_ParticleEmitter)
			dust_emitter.m_vPos = emitter_position
			dust_emitter.m_FireParticles = true
		end
	end
end

----HURT----
function chucky_boss_enter_hurt(name)
	doing_hurt = false
	execute_hurt = false
	returned_times = 0 
	if current_max_timer > 1 then
		current_max_timer = current_max_timer - 1
	end
	coreInstance:trace("Toy Hurt")
end

function chucky_boss_exit_hurt(name)
	local enemy = enemy_manager:get_enemy(name);
	--enemy.m_RenderableObject:remove_action(6);
	enemy.m_RenderableObject:blend_cycle(0,0.1,0);
	
end

function chucky_boss_update_hurt(ElapsedTime, doComprobation, name)
	local enemy = enemy_manager:get_enemy(name)
	if (enemy ~= nil) then
		if not doing_hurt then
			update_horizontal_boss_shoot(ElapsedTime, enemy)
			local mesh = get_renderable_object("enemies", enemy.m_RenderableObject.m_Room, enemy.m_ProjectileName)
			local enemyPosXZ = Vect3f(enemy:get_position().x, 0, enemy:get_position().z);
			local balaPosXZ = Vect3f(enemy.m_PosicionBala.x, 0, enemy.m_PosicionBala.z);
			local distance_to_bala = get_distance_between_points(enemyPosXZ, balaPosXZ);
			--coreInstance:trace("distance to bala: "..tostring(distance_to_bala))
			if distance_to_bala < 7 then
				execute_hurt = true
				boss_projectile_returned = false
				boss_projectile_returned_by_chucky = false;
				enemy.m_IsOnCooldown = false;
				enemy.BalaSpeed = enemy.BalaOriginalSpeed;
				current_speed_change = inicial_speed_change;
				actual_speed_change = 0;
				enemy.BalaActiva = false;
				fire_boss_projectile_particles(mesh, enemy, false)
				delete_shooting(mesh)
			end
		end
		if execute_hurt then
			execute_hurt = false
			enemy.m_Life = enemy.m_Life -1
			return_factor = return_factor + 1
			coreInstance:trace("vidas: "..tostring(enemy.m_Life))
			local animation_hurt = true
			if enemy.m_Life <=0 then
				enemy.m_Phases = enemy.m_Phases -1
				current_sequence = {}
				enemy.m_Life = 3;
				if enemy.m_Phases <= 0 then
					if chucky_boss_layer ~= "enemies" then
						switch_boss_layer("enemies")
					end
					enemy:m_FSM():newState("Dead")
					animation_hurt = false;
				end
				if enemy.m_Phases == 1 then
					coreInstance:trace("CHUCKY ANGRY")
					switch_boss_layer("angry")
				end
			end
			if animation_hurt then
				coreInstance:trace("doing animation hurt")
				doing_hurt = true
				enemy.m_RenderableObject:clear_cycle(0,0.3);
				enemy.m_RenderableObject:execute_action(6,0.1,0,1,true)
			end
		end
		if doing_hurt and not enemy.m_RenderableObject:is_action_animation_active() then
			enemy.m_RenderableObject:remove_action(6)
			enemy.m_RenderableObject:blend_cycle(0,0.1,0);
			coreInstance:trace("stoping hurt")
			doing_hurt = false
			enemy:m_FSM():newState("Parado")
		end
	end
end

----DEAD----
function chucky_boss_enter_dead(name)
	local enemy = enemy_manager:get_enemy(name)
	if (enemy ~= nil) then
		enemy.m_RenderableObject:execute_action(8,0.1,0,1,false)
	end	
end

function chucky_boss_exit_dead(name)
	boss_dead = false
end

function chucky_boss_update_dead(ElapsedTime, doComprobation, name)
	local enemy = enemy_manager:get_enemy(name)
	if (enemy ~= nil) then
		if not enemy.m_RenderableObject:is_action_animation_active() and not boss_dead then
			coreInstance:trace("YAAAAY WALL DONE!!")
			CCoreLuaWrapper().m_CoreInstance:getGUIManager():push_windows('Succeed.xml')
			-- Llamar al fin de demo /cinematica final
			boss_dead = true
		end
	end
end
