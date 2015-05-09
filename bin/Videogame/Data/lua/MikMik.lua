function mikmik_enter_stopped(name)
	--local enemy = enemy_manager:get_enemy(name)
	--currentwp = wp1
	--coreInstance:trace("Entro y Estoy parado")
	return 0
end

function mikmik_exit_stopped(name)
	local enemy = enemy_manager:get_enemy(name)
	enemy.m_CurrentTime = 0
end

function mikmik_update_stopped(ElapsedTime, doComprobation, name)
	--local enemy = coreInstance:get_renderable_object_layer_manager():get_renderable_objects_manager_by_str("enemies"):get_resource(name)
	--coreInstance:trace("Estoy parado")
	local enemy = enemy_manager:get_enemy(name)
	
	if enemy ~= nil then
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
				local player_position = player_controller:get_position()
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
	else
		instance.m_string = "Parado"
	end
	
end

function mikmik_enter_moving(name)
	return 0;
end

function mikmik_exit_moving(name)
	--local enemy = coreInstance:get_renderable_object_layer_manager():get_renderable_objects_manager_by_str("enemies"):get_resource(name)
	local enemy = enemy_manager:get_enemy(name)
	--instance.m_string = "Buscar_next_WP"
	if enemy ~= nil then
		enemy.m_CurrentTime = 0 
	else
		instance.m_string = "Parado"
	end
end

function mikmik_update_moving(ElapsedTime, doComprobation, name)

	--local enemy = coreInstance:get_renderable_object_layer_manager():get_renderable_objects_manager_by_str("enemies"):get_resource(name)
	local enemy = enemy_manager:get_enemy(name)
	if enemy ~= nil then
		if enemy.m_CurrentWp == nil then
		--	coreInstance:trace("I have a breakpoint")
			enemy.m_CurrentWp = enemy:get_next_wp()
		end
		
	--	coreInstance:trace(tostring(currentwp.x))
		
	--	enemy:move_to(ElapsedTime, enemy.m_CurrentWp)
		move_enemy(ElapsedTime, enemy.m_CurrentWp, enemy)
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

function mikmik_enter_calcwp(name)
	
	--local enemy = coreInstance:get_renderable_object_layer_manager():get_renderable_objects_manager_by_str("enemies"):get_resource(name)
	local enemy = enemy_manager:get_enemy(name)
	
	if enemy ~= nil and enemy:get_wp_vector_size() > 0 then
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

function mikmik_exit_calcwp(name)
--core:trace("Saliendo Buscar_next_WP");
-- OnExit Buscar_next_WP
	--local enemy = coreInstance:get_renderable_object_layer_manager():get_renderable_objects_manager_by_str("enemies"):get_resource(name)
	local enemy = enemy_manager:get_enemy(name)
	if enemy ~= nil then
		 enemy.m_CurrentTime = 0
	else
		instance.m_string = "Parado"
	end
end

function mikmik_update_calcwp(ElapsedTime, doComprobation, name)
	--local enemy = coreInstance:get_renderable_object_layer_manager():get_renderable_objects_manager_by_str("enemies"):get_resource(name)
	local enemy = enemy_manager:get_enemy(name)
	if enemy ~= nil and enemy:get_wp_vector_size() > 0 then
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

function mikmik_enter_attack_player(name)
	--coreInstance:trace("Entering Perseguir_PLayer");
	--local enemy = enemy_manager:get_enemy(name)
	return 0
end

function mikmik_exit_attack_player(name)
	local enemy = enemy_manager:get_enemy(name)
	enemy.m_Speed = enemy.m_OriginalSpeed
	enemy.m_SpeedModified = false
	enemy.m_CurrentTime = 0
	if enemy:get_wp_vector_size() == 0 then
		enemy.m_CurrentWp = enemy.m_OriginalPosition
		enemy.m_Returning = true
	end
end

function mikmik_update_attack_player(ElapsedTime, doComprobation, name)
--player_position = Vect3f(10,0,10)
	--coreInstance:trace("Persiguiendo");
	local player_position = player_controller:get_position()
	local enemy = enemy_manager:get_enemy(name)
	if enemy.m_SpeedModified == false then
		enemy.m_Speed = enemy.m_AttackSpeed
		enemy.m_SpeedModified = true	
	end
	
	move_enemy(ElapsedTime, player_position, enemy)
	
	--if doComprobation == 1 then
		local player_distance = get_distance_to_player(enemy:get_position(), player_position)
		if player_distance > 225 then
			instance.m_string = "Parado"
		--	enemy.m_Speed = enemy.m_Speed / speed_modifier
		end
		if player_distance < 1 then
		-- Aqui meter impacto del ataque
			instance.m_string = "Parado"
			check_hitbox(ElapsedTime, player_position, enemy)
		--	enemy.m_Speed = enemy.m_Speed / speed_modifier
		end
		
	--end
end