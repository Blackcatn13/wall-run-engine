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
				enemy:m_FSM():newState("Buscar_next_WP")
			end
						
		else --En caso de no tener waypoints que mire al player
			if enemy.m_CurrentWp == enemy.m_OriginalPosition and enemy.m_Returning == true then
				enemy:m_FSM():newState("Andar_WP")
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
			enemy:m_FSM():newState("Perseguir_Player")
		end
		enemy.m_CurrentTime = enemy.m_CurrentTime +1 * ElapsedTime
		enemy:actualizar_disparo(ElapsedTime)
		enemy:actualizar_hitbox()
	else
		enemy:m_FSM():newState("Parado")
	end
	
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
	else
		enemy:m_FSM():newState("Parado")
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
			enemy:m_FSM():newState("Perseguir_Player")
		else
		
			local wp_distance = get_distance_between_points(enemy:get_position(), enemy.m_CurrentWp)
			if wp_distance < 4 then
				--coreInstance:trace("Ya he llegado y a por otro")
				enemy:m_FSM():newState("Buscar_next_WP")
			end
		end
		enemy:actualizar_disparo(ElapsedTime)
		enemy:actualizar_hitbox()
	else
		enemy:m_FSM():newState("Parado")
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
		enemy:m_FSM():newState("Andar_WP")
	else
		enemy:m_FSM():newState("Parado")
	end
end

function pumpum_exit_calcwp(name)
--core:trace("Saliendo Buscar_next_WP");
-- OnExit Buscar_next_WP
	--local enemy = coreInstance:get_renderable_object_layer_manager():get_renderable_objects_manager_by_str("enemies"):get_resource(name)
	local enemy = coreInstance:get_enemy_manager():get_enemy(name)
	if (enemy ~= nil) and (enemy.m_isAlive == true) then
		 enemy.m_CurrentTime = 0
	else
		enemy:m_FSM():newState("Parado")
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
		enemy:m_FSM():newState("Andar_WP")
	else
		enemy:m_FSM():newState("Parado")
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
			enemy:m_FSM():newState("Parado")
		--	enemy.m_Speed = enemy.m_Speed / speed_modifier
		end
		if player_distance < 1 then
		-- Aqui meter impacto del ataque
			enemy:m_FSM():newState("Parado")
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


function pumpum_enter_take_damage(name)
	return 0
end

function pumpum_exit_take_damage(name)
	return 0
end

function pumpum_update_take_damage(ElapsedTime, doComprobation, name)
	local enemy = enemy_manager:get_enemy(name)
	enemy.m_Life = enemy.m_Life - 1
	if(enemy.m_Life <= 0) then
		enemy:m_FSM():newState("Dead")
	else
		enemy:m_FSM():newState("Parado")
	end
end

function pumpum_enter_dead(name)
	local enemy = enemy_manager:get_enemy(name)
	enemy.m_isAlive = false
	enemy.m_RenderableObject.m_Printable=false
	local dead_pos = enemy.m_PhysicController:get_position()
	dead_pos.y = dead_pos.y + 1000
	enemy:set_position(dead_pos)
	enemy.m_PhysicController:set_position(dead_pos)
	coreInstance:trace("HECHOMIKMIK");
end

function pumpum_exit_dead(name)
	enemy.m_RenderableObject.m_Printable=true
end

function pumpum_update_dead(ElapsedTime, doComprobation, name)
	return 0
end