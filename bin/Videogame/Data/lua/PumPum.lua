local coreInstance = CCoreLuaWrapper().m_CoreInstance
local base_yaw = math.pi/2
local flySpeed = 20;
local flyInclination = 3;
local flyDecay = 0.1;
--local current_time = 0
--ai_controller = CAIController()
--ai_controller.m_Speed = 0.1
local instance = CLuaGlobalsWrapper().m_CoreInstance;
local hide_offset = 1.3

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
	local enemy = enemy_manager:get_enemy(name)
	enemy.m_CurrentTime = 0
	--enemy.m_RenderableObject:blend_cycle(0,1,0);
	return 0
end

function pumpum_exit_stopped(name)
	local enemy = enemy_manager:get_enemy(name)
	enemy.m_CurrentTime = 0
	--enemy.m_RenderableObject:clear_cycle(0,0.3);
end

function pumpum_update_stopped(ElapsedTime, doComprobation, name)
	local enemy = enemy_manager:get_enemy(name)
	local player_position = player_controller:get_position()
	local player_distance = get_distance_to_player(enemy:get_position(), player_position)
				
	if enemy ~= nil then
		
		if player_distance < 3500 then
		update_shoot(ElapsedTime, enemy)
		
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
							if enemy.m_Hide or enemy.m_Hiding then
								show_or_hide(enemy, hide_offset, ElapsedTime)
							end
							rotate_yaw(enemy, ElapsedTime, player_position)
						--	local billboard = billboard_manager:get_resource(enemy.m_RenderableObject.m_Billboard)
						--	if billboard ~= nil then
						--		billboard.m_position = enemy.m_RenderableObject:get_position()+ enemy.m_RenderableObject.m_BillboardOffset
						--	end
						else
							if not enemy.m_Hide or enemy.m_Hiding then
								show_or_hide(enemy, hide_offset, ElapsedTime)
							end
						end
					end
				end	
			-- Si le Player se acerca atacar
			
			else
				local temp_zone = enemy.m_Zone
				if temp_zone == enemy.m_RenderableObject.m_Room then
					coreInstance:trace("Sala = zona")
					temp_zone = temp_zone..".0"
					coreInstance:trace(tostring(temp_zone))
				end
				if player.is_hit == false and tostring(temp_zone) == tostring(player.zone) and not enemy.m_Hide and not enemy.m_Hiding then
					coreInstance:trace("Pum listo para atacar")
					enemy:m_FSM():newState("Perseguir_Player")
				end
			end
		else
			local renderable_shoot = get_renderable_object("enemies", enemy.m_RenderableObject.m_Room,  enemy.m_ProjectileName)
			delete_shooting(renderable_shoot)
		end
		enemy.m_CurrentTime = enemy.m_CurrentTime + ElapsedTime
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
		update_shoot(ElapsedTime, enemy)
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
--	local enemy =  enemy_manager:get_enemy(name)
	--enemy.m_IsOnCoolDown = true
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
	local enemy = enemy_manager:get_enemy(name)
	local player_position = player_controller:get_position()
	
	local enemyPosXZ = Vect3f(enemy:get_position().x, 0, enemy:get_position().z)
	local playerPosXZ = Vect3f(player_position.x, 0, player_position.z)
	local player_distance = get_distance_to_player(enemyPosXZ, playerPosXZ)
	
	if enemy.m_SpeedModified == false then
		enemy.m_Speed = enemy.m_AttackSpeed
		enemy.m_SpeedModified = true	
	end
	--local random_destino = Vect3f(10,0,10)
	--move_enemy_pumpum(ElapsedTime, random_destino, enemy)
	--move_enemy_pumpum(ElapsedTime, player_position, enemy)

	rotate_yaw(enemy, ElapsedTime, player_position)
	--if doComprobation == 1 then
	--local player_distance = get_distance_to_player(enemy:get_position(), player_position)
	update_cooldown(enemy, ElapsedTime, player_position)
	if player_distance > 225 then
		enemy:m_FSM():newState("Parado")
		--	enemy.m_Speed = enemy.m_Speed / speed_modifier
	else
		local damageType = enemy:actualizar_hitbox() --esto setea take damage si le pegan
		-- damage 1 : player pega con ataque
		-- damage 2 : player pega con salto
		-- damage 3 : enemy pega player
		-- damage 0 : nada
		if damageType == 3 then
			--[[enemy.m_RenderableObject:clear_cycle(0,0.2);
			enemy.m_RenderableObject:clear_cycle(1,0.2);
			enemy.m_RenderableObject:execute_action(2,0.1,0,1,true);--]]
			--enemy.m_isAttacking = true;
			coreInstance:trace("Tocado")
		elseif damageType == 1 then
			enemy:add_damage_player()
			--[[enemy.m_time_to_fly = true;
			enemy.m_flyVec = Vect3f(enemyPosXZ.x - playerPosXZ.x, 0,enemyPosXZ.z - playerPosXZ.z);
			enemy.m_flyVec:normalize(1);
			enemy.m_flyVec = Vect3f(enemy.m_flyVec.x, flyInclination, enemy.m_flyVec.z);]]
			--local dead_pos = enemy.m_PhysicController:get_position()
			--enemy.m_OriginalPosition = Vect3f(dead_pos.x,dead_pos.y,dead_pos.z)
			coreInstance:trace("Tocado con ataque")
		elseif damageType == 2 then
			player_controller.m_executeDoubleJump = true;
			--local dead_pos = enemy.m_PhysicController:get_position()
			--enemy.m_OriginalPosition = Vect3f(dead_pos.x,dead_pos.y,dead_pos.z)
		end
	end
	if player_distance < 1 then
		-- Aqui meter impacto del ataque
		enemy:m_FSM():newState("Parado")
		--	enemy.m_Speed = enemy.m_Speed / speed_modifier
	end
		
		
		
	if (enemy ~= nil) then
		--enemy:actualizar_disparo(ElapsedTime)	
		update_shoot(ElapsedTime, enemy)
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
	local enemy = enemy_manager:get_enemy(name)
	if enemy ~= nil then
		enemy.m_CurrentTime = 0
		enemy.m_Life = enemy.m_Life - 1
		
		
		if(enemy.m_Life <= 0) then
			enemy.m_IsDying = true
		end
		
		if(enemy.m_Life <= 0 and enemy.m_time_to_fly == false) then
			enemy:m_FSM():newState("Dead")
		--[[else 
			if enemy.m_RenderableObject:is_action_animation_active() then
				enemy.m_RenderableObject:remove_action(2)
			end
			if	enemy.m_RenderableObject:is_cycle_animation_active() then
				enemy.m_RenderableObject:clear_cycle(0,0.2);
				enemy.m_RenderableObject:clear_cycle(1,0.2);
			end
			enemy.m_RenderableObject:execute_action(4,0.1,0,1,false)
			]]
		end
	end
	return 0
end

function pumpum_exit_take_damage(name)
	local enemy = enemy_manager:get_enemy(name)
	if enemy ~= nil then
		enemy.m_CurrentTime = 0
	end
	return 0
end

function pumpum_update_take_damage(ElapsedTime, doComprobation, name)
	local enemy = enemy_manager:get_enemy(name)
	if(enemy.m_Life <= 0 and not enemy.m_IsDying ) then
		enemy.m_IsDying = true
	end
	if enemy.m_time_to_fly == true then
		local auxVec = enemy.m_flyVec * flySpeed;
		local isGrounded = enemy.m_PhysicController:move(auxVec * ElapsedTime, ElapsedTime);
		move_enemy_renderable(enemy, 0.5);
		if isGrounded == true or enemy.m_CurrentTime > 3 then
			enemy.m_time_to_fly = false;
			enemy.m_flyVec = Vect3f(0,0,0);
		else
			enemy.m_flyVec = Vect3f(enemy.m_flyVec.x, enemy.m_flyVec.y - flyDecay, enemy.m_flyVec.z);
			enemy.m_CurrentTime = enemy.m_CurrentTime + ElapsedTime;
		end
	else --if not enemy.m_RenderableObject:is_action_animation_active() then
		if enemy.m_Life <= 0 then
			enemy:m_FSM():newState("Dead")
		else
			enemy:m_FSM():newState("Parado")
		end
	end
end

function pumpum_enter_dead(name)
	local enemy = enemy_manager:get_enemy(name)
		
	if enemy ~= nil then
		--coreInstance:trace("Init muerto Mik")
		--[[enemy.m_RenderableObject:clear_cycle(0,0.2);
		enemy.m_RenderableObject:clear_cycle(1,0.2);
		enemy.m_RenderableObject:remove_action(2);
		enemy.m_RenderableObject:execute_action(3,0.1,0,1,true);
		--]]
		enemy.m_isAlive = false
		
	end
end

function pumpum_exit_dead(name)
	local enemy = enemy_manager:get_enemy(name)
	if enemy ~= nil then
		enemy.m_RenderableObject.m_Printable=true
		enemy.m_CurrentTime = 0
		--enemy.m_RenderableObject:remove_action(3);
		coreInstance:trace("setting alive")
	end
end

function pumpum_update_dead(ElapsedTime, doComprobation, name)
	local enemy = enemy_manager:get_enemy(name)
	if enemy ~= nil then
	update_shoot(ElapsedTime, enemy)
	if --[[not enemy.m_RenderableObject:is_action_animation_active() and]] not enemy.m_isAlive and not enemy.m_MovedToDiePosition then
			enemy.m_CurrentTime = enemy.m_CurrentTime + 1 *ElapsedTime
			--coreInstance:trace("Update muerto Mik sin accion")
			local emitter = particle_manager:get_resource(enemy.m_RenderableObject.m_ParticleEmitter)
			if emitter ~= nil then
				emitter.m_vPos = enemy.m_RenderableObject:get_position()+ enemy.m_RenderableObject.m_EmitterOffset
				emitter.m_FireParticles = true 
			end
			if enemy.m_CurrentTime > 0.5 then
				enemy.m_RenderableObject.m_Printable=false
				local dead_pos = enemy.m_PhysicController:get_position()
				dead_pos.y = dead_pos.y + 1000
				enemy:set_position(dead_pos)
				enemy.m_PhysicController:set_position(dead_pos)
				enemy:move_to_position(dead_pos)
				enemy.m_MovedToDiePosition  = true
			end
		--elseif enemy.m_RenderableObject:is_action_animation_active() and not enemy.m_isAlive then
			--enemy.m_RenderableObject:execute_action(3,0.1,0,1,false);
		end
		
		
	end
end