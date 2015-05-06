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
--local pumpum_manager = core:get_enemy_manager():insert_enemy(enemy)
--local wp1 = Vect3f(-3.0,2.0,15.0) 
--local wp2 = Vect3f(-3.0,2.0,-15.0)
--local currentwp = Vect3f(0.0,0.0,0.0)

--[[function set_enemy(_enemy)
	enemy = _enemy
end
--]]
function pumpum_enter_stopped(name)
	--local enemy = coreInstance:get_enemy_manager():get_enemy(name)
	--currentwp = wp1
	--coreInstance:trace("Entro y Estoy parado")
	return 0
end

function pumpum_exit_stopped(name)
	local enemy = coreInstance:get_enemy_manager():get_enemy(name)
	enemy.m_CurrentTime = 0
end

function pumpum_update_stopped(ElapsedTime, doComprobation, name)
	--local enemy = coreInstance:get_renderable_object_layer_manager():get_renderable_objects_manager_by_str("enemies"):get_resource(name)
	--coreInstance:trace("Estoy parado")
	local enemy = coreInstance:get_enemy_manager():get_enemy(name)
	if (enemy ~= nil) and (enemy.m_isAlive == true) then
		if enemy:get_wp_vector_size() > 0 then --Si tiene waypoints para moverse
			-- En caso de acabar de perseguir o llegar a un WP si la distancia al siguiente es muy corta vuelve a la posicion original
						
			if enemy.m_CurrentTime >= 0.1 then
				instance.m_string = "Buscar_next_WP"
			end
						
		else --En caso de no tener waypoints que mire al player
			if enemy.m_CurrentWp == enemy.m_OriginalPosition and enemy.m_Returning == true then
				instance.m_string = "Andar_WP"
				enemy.m_Returning = false
			else 
				local player_position = coreInstance:get_player_controller():get_position()
				local player_distance = get_distance_to_player(enemy:get_position(), player_position)
				if player_distance < 500 then
					-- Animacion de andar
					enemy:rotate_yaw(ElapsedTime, player_position)
				end
			end
		end	
		-- Si le Player se acerca atacaarl
		if check_attack(enemy) == true and enemy.m_CurrentTime >= 2 then
		--	coreInstance:trace("Vamos a perseguir")
			instance.m_string = "Perseguir_Player"
		end
		enemy.m_CurrentTime = enemy.m_CurrentTime +1 * ElapsedTime
		enemy:actualizar_disparo(ElapsedTime)
		enemy:actualizar_hitbox()
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


function pumpum_enter_moving(name)
	return 0;
end

function pumpum_exit_moving(name)
	--local enemy = coreInstance:get_renderable_object_layer_manager():get_renderable_objects_manager_by_str("enemies"):get_resource(name)
	local enemy = coreInstance:get_enemy_manager():get_enemy(name)
	--instance.m_string = "Buscar_next_WP"
	if (enemy ~= nil) and (enemy.m_isAlive == true) then
		enemy.m_CurrentTime = 0 
		enemy:actualizar_disparo(ElapsedTime)
		enemy:actualizar_hitbox()
	else
		instance.m_string = "Parado"
	end
end

function pumpum_update_moving(ElapsedTime, doComprobation, name)

	--local enemy = coreInstance:get_renderable_object_layer_manager():get_renderable_objects_manager_by_str("enemies"):get_resource(name)
	local enemy = coreInstance:get_enemy_manager():get_enemy(name)
	if (enemy ~= nil) and (enemy.m_isAlive == true) then
		if enemy.m_CurrentWp == nil then
		--	coreInstance:trace("I have a breakpoint")
			enemy.m_CurrentWp = enemy:get_next_wp()
		end
		
	--	coreInstance:trace(tostring(currentwp.x))
		
	--	enemy:move_to(ElapsedTime, enemy.m_CurrentWp)
		--move_enemy_pumpum(ElapsedTime, enemy.m_CurrentWp, enemy)
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
		enemy:actualizar_disparo(ElapsedTime)
		enemy:actualizar_hitbox()
	else
		instance.m_string = "Parado"
	end
		
end

function pumpum_enter_calcwp(name)
	
	--local enemy = coreInstance:get_renderable_object_layer_manager():get_renderable_objects_manager_by_str("enemies"):get_resource(name)
	local enemy = coreInstance:get_enemy_manager():get_enemy(name)
	
	if (enemy ~= nil) and (enemy:get_wp_vector_size() > 0) and (enemy.m_isAlive == true) then
		--[[if currentwp.z == wp2.z then
			currentwp = wp
		else
			currentwp = wp2
		end--]]
		enemy.m_CurrentWp = enemy:get_next_wp()
		instance.m_string = "Andar_WP"
		enemy:actualizar_disparo(ElapsedTime)
		enemy:actualizar_hitbox()
	else
		instance.m_string = "Parado"
	end
end

function pumpum_exit_calcwp(name)
--core:trace("Saliendo Buscar_next_WP");
-- OnExit Buscar_next_WP
	--local enemy = coreInstance:get_renderable_object_layer_manager():get_renderable_objects_manager_by_str("enemies"):get_resource(name)
	local enemy = coreInstance:get_enemy_manager():get_enemy(name)
	if (enemy ~= nil) and (enemy.m_isAlive == true) then
		 enemy.m_CurrentTime = 0
		 enemy:actualizar_disparo(ElapsedTime)
		 enemy:actualizar_hitbox()
	else
		instance.m_string = "Parado"
	end
end

function pumpum_update_calcwp(ElapsedTime, doComprobation, name)
	--local enemy = coreInstance:get_renderable_object_layer_manager():get_renderable_objects_manager_by_str("enemies"):get_resource(name)
	local enemy = coreInstance:get_enemy_manager():get_enemy(name)
	if (enemy ~= nil) and (enemy.m_isAlive == true) and enemy:get_wp_vector_size() > 0 then
		enemy:actualizar_disparo(ElapsedTime)
		enemy:actualizar_hitbox()
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

function pumpum_enter_attack_player(name)

	--coreInstance:trace("Entering Perseguir_PLayer");
	local enemy = coreInstance:get_enemy_manager():get_enemy(name)
	
end

function pumpum_exit_attack_player(name)
	local enemy = coreInstance:get_enemy_manager():get_enemy(name)
	enemy.m_Speed = enemy.m_OriginalSpeed
	enemy.m_SpeedModified = false
	enemy.m_CurrentTime = 0
	if enemy:get_wp_vector_size() == 0 then
		enemy.m_CurrentWp = enemy.m_OriginalPosition
		enemy.m_Returning = true
	end
	if (enemy ~= nil) and (enemy.m_isAlive == true) then
		enemy:actualizar_disparo(ElapsedTime)	
		enemy:actualizar_hitbox()
	end
end

function pumpum_update_attack_player(ElapsedTime, doComprobation, name)
--player_position = Vect3f(10,0,10)
	--coreInstance:trace("Persiguiendo");
	local player_position = coreInstance:get_player_controller():get_position()
	local enemy = coreInstance:get_enemy_manager():get_enemy(name)
	
	if enemy.m_SpeedModified == false then
		enemy.m_Speed = enemy.m_AttackSpeed
		enemy.m_SpeedModified = true	
	end
	--local random_destino = Vect3f(10,0,10)
	--move_enemy_pumpum(ElapsedTime, random_destino, enemy)
	move_enemy_pumpum(ElapsedTime, player_position, enemy)
	
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
	if (enemy ~= nil) then
		enemy:actualizar_disparo(ElapsedTime)	
		enemy:actualizar_hitbox()
	end
	--end
end

function move_enemy_pumpum(ElapsedTime, _point, Enemy)
	local player =  coreInstance:get_player_controller()
	if player.m_is3D == true then 
		Enemy:only_rotate(ElapsedTime, _point)
	else
		Enemy:only_rotate(ElapsedTime, _point)
	end
end

function get_distance_to_player(current_position, _player_position)
	-- calcular distancia hacia player
	local distance = 0
	distance = ((_player_position.x - current_position.x) ^2 + (_player_position.y - current_position.y) ^2 + (_player_position.z - current_position.z) ^2)
	return distance
end