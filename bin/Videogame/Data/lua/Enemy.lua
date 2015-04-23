local coreInstance = CCoreLuaWrapper().m_CoreInstance

--local current_time = 0
--ai_controller = CAIController()
--ai_controller.m_Speed = 0.1
local instance = CLuaGlobalsWrapper().m_CoreInstance;

--local wp_manager = core:get_wp_manager()
--local id_next_wp
--local id_destino_wp
--local id_path_next_wp

--local enemy = CEasyEnemy("Enemy4", "MikMikTest001", Vect3f(-3.0,2.0,10.0), 0.1, 2.5, 13, 0.0)	
--local enemy_manager = core:get_enemy_manager():insert_enemy(enemy)
--local wp1 = Vect3f(-3.0,2.0,15.0) 
--local wp2 = Vect3f(-3.0,2.0,-15.0)
--local currentwp = Vect3f(0.0,0.0,0.0)

--[[function set_enemy(_enemy)
	enemy = _enemy
end
--]]
function enemy_enter_stopped(name)
	--local enemy = coreInstance:get_enemy_manager():get_enemy(name)
	--currentwp = wp1
	--coreInstance:trace("Entro y Estoy parado")
	return 0
end

function enemy_exit_stopped(name)
	local enemy = coreInstance:get_enemy_manager():get_enemy(name)
	enemy.m_CurrentTime = 0
end

function enemy_update_stopped(ElapsedTime, doComprobation, name)
	--local enemy = coreInstance:get_renderable_object_layer_manager():get_renderable_objects_manager_by_str("enemies"):get_resource(name)
	--coreInstance:trace("Estoy parado")
	local enemy = coreInstance:get_enemy_manager():get_enemy(name)
	
	if enemy ~= nil then
		-- En caso de acabar de perseguir o llegar a un WP si la distancia al siguiente es muy corta vuelve a la posicion original
		if enemy.m_CurrentWp ~= nil then
			local wp_distance = get_distance_between_points(enemy.m_CurrentWp, enemy:get_next_wp())
		--	coreInstance:trace(tostring(wp_distance))
			if wp_distance < 4 then
				enemy.m_CurrentWp = enemy:get_original_position()
			end
		end
		
		if enemy.m_CurrentTime >= 0.1 then
			instance.m_string = "Andar_WP"
		end
		
		enemy.m_CurrentTime = enemy.m_CurrentTime +1 * ElapsedTime
			
		-- Si le Player se acerca atacaarl
		if check_attack(enemy) == true then
		--	coreInstance:trace("Vamos a perseguir")
			instance.m_string = "Perseguir_Player"
		end
	else
		instance.m_string = "Parado"
	end
	
end

function check_attack (_enemy)
	local player_position = coreInstance:get_player_controller():get_position()
	--core:trace(tostring(player_position.x));
		
	local player_distance = get_distance_to_player(_enemy:get_position(), player_position)
	--coreInstance:trace(tostring(player_distance))
	if player_distance <= 49 then
		coreInstance:trace("Attack!!")
		coreInstance:trace(_enemy:get_name())
		return true
	end
	return false
end


function enemy_enter_moving(name)
	return 0;
end

function enemy_exit_moving(name)
	--local enemy = coreInstance:get_renderable_object_layer_manager():get_renderable_objects_manager_by_str("enemies"):get_resource(name)
	local enemy = coreInstance:get_enemy_manager():get_enemy(name)
	--instance.m_string = "Buscar_next_WP"
	if enemy ~= nil then
		enemy.m_CurrentTime = 0 
	else
		instance.m_string = "Parado"
	end
end

function enemy_update_moving(ElapsedTime, doComprobation, name)

	--local enemy = coreInstance:get_renderable_object_layer_manager():get_renderable_objects_manager_by_str("enemies"):get_resource(name)
	local enemy = coreInstance:get_enemy_manager():get_enemy(name)
	if enemy ~= nil then
		if enemy.m_CurrentWp == nil then
		--	coreInstance:trace("I have a breakpoint")
			enemy.m_CurrentWp = enemy:get_next_wp()
		end
		
	--	coreInstance:trace(tostring(currentwp.x))
		
		enemy:move_to(ElapsedTime, enemy.m_CurrentWp)
	--		coreInstance:trace("Am I moving??")
		if check_attack(enemy) == true then
		--	coreInstance:trace("Vamos a perseguir")
			instance.m_string = "Perseguir_Player"
		else
		
			local wp_distance = get_distance_between_points(enemy:get_position(), enemy.m_CurrentWp)
			if wp_distance < 4 then
				--coreInstance:trace("Ya he llegado y a por otro")
				instance.m_string = "Buscar_next_WP"
			end
		end
	else
		instance.m_string = "Parado"
	end
		
end

function enemy_enter_calcwp(name)
	
	--local enemy = coreInstance:get_renderable_object_layer_manager():get_renderable_objects_manager_by_str("enemies"):get_resource(name)
	local enemy = coreInstance:get_enemy_manager():get_enemy(name)
	
	if enemy ~= nil then
		--[[if currentwp.z == wp2.z then
			currentwp = wp
		else
			currentwp = wp2
		end--]]
		enemy.m_CurrentWp = enemy:get_next_wp()
		instance.m_string = "Andar_WP"
	else
		instance.m_string = "Parado"
	end
end

function enemy_exit_calcwp(name)
--core:trace("Saliendo Buscar_next_WP");
-- OnExit Buscar_next_WP
	--local enemy = coreInstance:get_renderable_object_layer_manager():get_renderable_objects_manager_by_str("enemies"):get_resource(name)
	local enemy = coreInstance:get_enemy_manager():get_enemy(name)
	if enemy ~= nil then
		 enemy.m_CurrentTime = 0
	else
		instance.m_string = "Parado"
	end
end

function enemy_update_calcwp(ElapsedTime, doComprobation, name)
	--local enemy = coreInstance:get_renderable_object_layer_manager():get_renderable_objects_manager_by_str("enemies"):get_resource(name)
	local enemy = coreInstance:get_enemy_manager():get_enemy(name)
	if enemy ~= nil then
		--[[if enemy.wp.z == 15 then
			enemy.wp = Vect3f(2.0,2.0,-15.0)
		else
			enemy.wp = Vect3f(2.0,2.0,15.0)
		end--]]
		instance.m_string = "Andar_WP"
	else
		instance.m_string = "Parado"
	end
end

function enemy_enter_attack_player(name)

	--coreInstance:trace("Entering Perseguir_PLayer");
	local enemy = coreInstance:get_enemy_manager():get_enemy(name)
	
end

function enemy_exit_attack_player(name)
	local enemy = coreInstance:get_enemy_manager():get_enemy(name)
	enemy.m_Speed = enemy.m_OriginalSpeed
	enemy.m_SpeedModified = false
	enemy.m_CurrentTime = 0
end

function enemy_update_attack_player(ElapsedTime, doComprobation, name)
--player_position = Vect3f(10,0,10)
	--coreInstance:trace("Persiguiendo");
	local player_position = coreInstance:get_player_controller():get_position()
	local enemy = coreInstance:get_enemy_manager():get_enemy(name)
	if enemy.m_SpeedModified == false then
		enemy.m_Speed = enemy.m_AttackSpeed
		enemy.m_SpeedModified = true	
	end
	
	enemy:move_to(ElapsedTime, coreInstance:get_player_controller():get_position())
	
	--if doComprobation == 1 then
		local player_distance = get_distance_to_player(enemy:get_position(), player_position)
		if player_distance > 225 then
			instance.m_string = "Parado"
		--	enemy.m_Speed = enemy.m_Speed / speed_modifier
		end
		if player_distance < 1 then
		-- Aqui meter impacto del ataque
			instance.m_string = "Parado"
		--	enemy.m_Speed = enemy.m_Speed / speed_modifier
		end
		
	--end
end


function get_distance_to_player(current_position, _player_position)
	-- calcular distancia hacia player
	local distance = 0
	distance = ((_player_position.x - current_position.x) ^2 + (_player_position.y - current_position.y) ^2 + (_player_position.z - current_position.z) ^2)
	return distance
end