--local min_player_distance = 49
local base_yaw = math.pi/2
function mikmik_enter_stopped(name)
	--local enemy = enemy_manager:get_enemy(name)
	--currentwp = wp1
	--coreInstance:trace("Entro y Estoy parado")
	local enemy = enemy_manager:get_enemy(name)
	enemy.m_CurrentTime = 0
	enemy.m_RenderableObject:blend_cycle(0,1,0);
end

function mikmik_exit_stopped(name)
	local enemy = enemy_manager:get_enemy(name)
	enemy.m_CurrentTime = 0
	enemy.m_RenderableObject:clear_cycle(0,0.3);
end

function mikmik_update_stopped(ElapsedTime, doComprobation, name)
	local enemy = enemy_manager:get_enemy(name)
	local player_position = player_controller:get_position()
	local player_distance = get_distance_to_player(enemy:get_position(), player_position)
				
	if enemy ~= nil then
		if player_distance < 3500 then
			if player_distance > enemy.m_AttackPlayerDistance  then --min_player_distance => enemy.m_AttackPlayerDistance
				if enemy:get_wp_vector_size() > 0 then --Si tiene waypoints para moverse
					-- En caso de acabar de perseguir o llegar a un WP si la distancia al siguiente es muy corta vuelve a la posicion original
								
					if enemy.m_CurrentTime >= 0.1 then
						enemy:m_FSM():newState("Buscar_next_WP")
					end
								
				else --En caso de no tener waypoints que mire al player
					if enemy.m_CurrentWp == enemy.m_OriginalPosition and enemy.m_Returning == true then
						enemy:m_FSM():newState("Andar_WP")
						enemy.m_Returning = false
					else 
						
						if player_distance < 500 then
							-- Animacion de andar
							rotate_yaw(enemy, ElapsedTime, player_position)
						end
					end
				end	
			-- Si le Player se acerca atacar
			elseif player.is_hit == false then
				enemy:m_FSM():newState("Perseguir_Player")
			end
		end
		enemy.m_CurrentTime = enemy.m_CurrentTime + ElapsedTime
	end
	
end

function mikmik_enter_moving(name)
	local enemy = enemy_manager:get_enemy(name)
	enemy.m_RenderableObject:blend_cycle(1,1,0.2);
	return 0;
end

function mikmik_exit_moving(name)
	local enemy = enemy_manager:get_enemy(name)
	if enemy ~= nil then
		enemy.m_CurrentTime = 0
	end
	enemy.m_RenderableObject:clear_cycle(1,0.2);
end

function mikmik_update_moving(ElapsedTime, doComprobation, name)
	local enemy = enemy_manager:get_enemy(name)
	if enemy ~= nil then
		if enemy.m_CurrentWp == nil then
			enemy.m_CurrentWp = enemy:get_next_wp()
		end
		
		move_enemy(ElapsedTime, enemy.m_CurrentWp, enemy)
		
		if check_attack(enemy) == true then
			enemy:m_FSM():newState("Perseguir_Player")
		else
			local wp_distance = get_distance_between_points(enemy:get_position(), enemy.m_CurrentWp)
			if wp_distance < 4 then
				if enemy:get_wp_vector_size() > 0 then
					enemy:m_FSM():newState("Buscar_next_WP")
				else
					enemy:m_FSM():newState("Parado")
				end
			end
		end
	end
end

function mikmik_enter_calcwp(name)
	return 0
end

function mikmik_exit_calcwp(name)
--core:trace("Saliendo Buscar_next_WP");
-- OnExit Buscar_next_WP
	local enemy = enemy_manager:get_enemy(name)
	if enemy ~= nil then
		 enemy.m_CurrentTime = 0
	end
end

function mikmik_update_calcwp(ElapsedTime, doComprobation, name)
	local enemy = enemy_manager:get_enemy(name)
	if enemy ~= nil and enemy:get_wp_vector_size() > 0 then
		enemy.m_CurrentWp = enemy:get_next_wp()
		enemy:m_FSM():newState("Andar_WP")
	else
		enemy:m_FSM():newState("Parado")
	end
end

function mikmik_enter_attack_player(name)
	local enemy = enemy_manager:get_enemy(name)
	if enemy ~= nil and enemy:is_static() == false then -- en caso de no ser estatico
		enemy.m_Speed = enemy.m_AttackSpeed
		enemy.m_SpeedModified = true
		enemy.m_RenderableObject:blend_cycle(1,1,0.2);
	end
	return 0
end

function mikmik_exit_attack_player(name)
	local enemy = enemy_manager:get_enemy(name)
	if enemy ~= nil and enemy:is_static() == false then -- en caso de no ser estatico
		enemy.m_Speed = enemy.m_OriginalSpeed
		enemy.m_SpeedModified = false
		enemy.m_CurrentTime = 0
		if enemy:get_wp_vector_size() == 0 then
			enemy.m_CurrentWp = enemy.m_OriginalPosition
			enemy.m_Returning = true
		end
		enemy.m_RenderableObject:clear_cycle(1,0.2);
	end
end

function mikmik_update_attack_player(ElapsedTime, doComprobation, name)
--player_position = Vect3f(10,0,10)
	local player_position = player_controller:get_position()
	local enemy = enemy_manager:get_enemy(name)
	if enemy ~= nil then
		if player.is_hit == false then
			move_enemy(ElapsedTime, player_position, enemy) -- en caso de no ser estatico
		end
		
		local player_distance = get_distance_to_player(enemy:get_position(), player_position)
		
		if player_distance > 225 then
			enemy:m_FSM():newState("Parado")
		else
			enemy:actualizar_hitbox() --esto setea take damage si le pegan
		end
	end
end

function mikmik_enter_take_damage(name)
	return 0
end

function mikmik_exit_take_damage(name)
	local enemy = enemy_manager:get_enemy(name)
	if enemy ~= nil then
		enemy.m_CurrentTime = 0
	end
	return 0
end

function mikmik_update_take_damage(ElapsedTime, doComprobation, name)
	local enemy = enemy_manager:get_enemy(name)
	if enemy ~= nil then
		enemy.m_Life = enemy.m_Life - 1
		if(enemy.m_Life <= 0) then
			enemy:m_FSM():newState("Dead")
		else
			enemy:m_FSM():newState("Parado")
		end
	end
end

function mikmik_enter_dead(name)
	local enemy = enemy_manager:get_enemy(name)
	if enemy ~= nil then
		enemy.m_isAlive = false
		enemy.m_RenderableObject.m_Printable=false
		local dead_pos = enemy.m_PhysicController:get_position()
		coreInstance:trace(name);
		dead_pos.y = dead_pos.y + 1000
		enemy:set_position(dead_pos)
		enemy.m_PhysicController:set_position(dead_pos)
		enemy:move_to_position(dead_pos)
		player.enemies_killed = player.enemies_killed + 1
		check_enemies_killed(5, "door_001")
	end
end

function mikmik_exit_dead(name)
	local enemy = enemy_manager:get_enemy(name)
	if enemy ~= nil then
		enemy.m_RenderableObject.m_Printable=true
		enemy.m_CurrentTime = 0
	end
end

function mikmik_update_dead(ElapsedTime, doComprobation, name)
	local enemy = enemy_manager:get_enemy(name)
	if enemy ~= nil then
		if enemy.m_isAlive == true then
			enemy:m_FSM():newState("Parado")
		end
	end
end