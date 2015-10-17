--local min_player_distance = 49
local base_yaw = math.pi/2
local flySpeed = 10;
local flyInclination = 3;
local flyDecay = 5;
local visibleTime = 0.2;
local MikHeight = 0.7;

function mikmik_enter_stopped(name)
	--local enemy = enemy_manager:get_enemy(name)
	--currentwp = wp1
--	coreInstance:trace("Entro y Estoy parado: " .. name)
	local enemy = enemy_manager:get_enemy(name)
	enemy.m_CurrentTime = 0
	mesh = enemy.m_RenderableObject
	mesh:blend_cycle(0,1,0);
	if enemy:get_platform() ~= nil and enemy.m_isAlive then
		local temp_pose = Vect3f(enemy:get_platform():get_position().x-1, enemy:get_platform():get_position().y+2, enemy:get_platform():get_position().z-1)
		mesh:set_position(temp_pose)
		enemy.m_PhysicController:set_position(temp_pose)
	--	coreInstance:trace("Enemy stopped on platform position: "..tostring(mesh:get_position().x)..", "..tostring(mesh:get_position().y)..", "..tostring(mesh:get_position().z))
	end
	exclamation = particle_manager:get_resource(mesh.m_ParticleEmitter2)
	enemy.m_PhysicController:move(Vect3f(1, 0, 0) * enemy.m_Speed * coreInstance.m_ElapsedTime, coreInstance.m_ElapsedTime)
	move_enemy_renderable(enemy, MikHeight);
	if exclamation ~= nil then
		exclamation:set_visible(true)
		exclamation.m_FireParticles = true;
		exclamation.m_vPos = mesh:get_position() + mesh.m_EmitterOffset2;
		enemy.m_visibleTime = 0;
	end
	dir = Vect3f(math.cos(mesh:get_yaw()), 0, math.sin(mesh:get_yaw()))
	sound_manager:SetGameObjectPosition(name, mesh:get_position(), dir)
	sound_manager:PlayEvent("Mik_Idle", name);
end

function mikmik_exit_stopped(name)
	local enemy = enemy_manager:get_enemy(name)
	enemy.m_CurrentTime = 0
	if not enemy:is_static() then
		enemy.m_RenderableObject:clear_cycle(0,0.3);
	end
end

function mikmik_update_stopped(ElapsedTime, doComprobation, name)
	local enemy = enemy_manager:get_enemy(name)
	local player_position = player_controller:get_position()
	local player_distance = get_distance_to_player(enemy:get_position(), player_position)
	if enemy ~= nil then
		exclamation = particle_manager:get_resource(enemy.m_RenderableObject.m_ParticleEmitter2)
		move_enemy_renderable(enemy, MikHeight);
		if exclamation ~= nil then
			if (exclamation:get_visible() == true) then
				exclamation:set_visible(false)
			end
		end
		local platform = enemy:get_platform()
		if platform ~= nil and platform.m_Direction ~= nil and enemy.m_isAlive then
			--if enemy:is_on_platform() then
				-- Si está sobre una plataforma que se desplace con ella
				--coreInstance:trace("Platform direction: "..tostring(platform.m_Direction.x)..", "..tostring(platform.m_Direction.y)..", "..tostring(platform.m_Direction.z) )
				enemy.m_PhysicController:move(platform.m_Direction * platform.m_Speed * ElapsedTime, ElapsedTime)
				local mik_position = Vect3f(enemy.m_PhysicController:get_position().x, enemy.m_RenderableObject:get_position().y, enemy.m_PhysicController:get_position().z)
				--coreInstance:trace("Mik on platform position: "..tostring(mik_position.x)..", "..tostring(mik_position.y)..", "..tostring(mik_position.z) )
				if mik_position.y < platform:get_position().y then
				
					mik_position= Vect3f( platform:get_position().x-1, platform:get_position().y +2, platform:get_position().z-1)
					enemy.m_PhysicController:set_position(mik_position)
				end
				enemy.m_RenderableObject:set_position(mik_position)
			--	coreInstance:trace("Enemy updated on platform position: "..tostring(enemy.m_RenderableObject:get_position().x)..", "..tostring(enemy.m_RenderableObject:get_position().y)..", "..tostring(enemy.m_RenderableObject:get_position().z))
				--coreInstance:trace("Enemy platform position: "..tostring(platform:get_position().x)..", "..tostring(platform:get_position().y)..", "..tostring(platform:get_position().z))
			--end
		end
		
		if player_distance < 3500 then
		
			if player_distance > enemy.m_AttackPlayerDistance then --min_player_distance => enemy.m_AttackPlayerDistance
				if platform == nil then				
					if enemy:get_wp_vector_size() > 0 then --Si tiene waypoints para moverse
						-- En caso de acabar de perseguir o llegar a un WP si la distancia al siguiente es muy corta vuelve a la posicion original
						if enemy.m_CurrentTime >= 0.1 then
							enemy:m_FSM():newState("Buscar_next_WP")
						end
									
					else --En caso de no tener waypoints que mire al player
						if enemy.m_CurrentWp ~= nil and enemy.m_CurrentWp == enemy.m_OriginalPosition and enemy.m_Returning == true then
						--	coreInstance:trace("Resucitando y andando!!!")
							enemy:m_FSM():newState("Andar_WP")
							enemy.m_Returning = false
						else 
							if player_distance < 700 then
								-- Animacion de andar
								rotate_yaw(enemy, ElapsedTime, player_position)
								mesh = enemy.m_RenderableObject
								dir = Vect3f(math.cos(mesh:get_yaw()), 0, math.sin(mesh:get_yaw()))
								sound_manager:SetGameObjectPosition(name, mesh:get_position(), dir)
								--local billboard = billboard_manager:get_resource(enemy.m_RenderableObject.m_Billboard)
								--if billboard ~= nil then
									--billboard.m_position = enemy.m_RenderableObject:get_position()+ enemy.m_RenderableObject.m_BillboardOffset
								--end
							end
						end
					end	
				else
					if player_distance < 700 then
						-- Animacion de andar
						rotate_yaw(enemy, ElapsedTime, player_position)
						mesh = enemy.m_RenderableObject
						dir = Vect3f(math.cos(mesh:get_yaw()), 0, math.sin(mesh:get_yaw()))
						sound_manager:SetGameObjectPosition(name, mesh:get_position(), dir)
								
					end
				end
			-- Si le Player se acerca atacar
			else
				local temp_zone = enemy.m_Zone
				if temp_zone == enemy.m_RenderableObject.m_Room then
					--coreInstance:trace("Sala = zona")
					temp_zone = temp_zone..".0"
					--coreInstance:trace(tostring(temp_zone))
				end
				if player.is_hit == false and tostring(temp_zone) == tostring(player.zone) then
					enemy:m_FSM():newState("Perseguir_Player")
				--	coreInstance:trace("A perseguiiiir")
				end
			end
		end
		enemy.m_CurrentTime = enemy.m_CurrentTime + ElapsedTime
	end
	
end

function mikmik_enter_moving(name)
--coreInstance:trace("Moviendose " .. name)
	local enemy = enemy_manager:get_enemy(name)
	enemy.m_RenderableObject:blend_cycle(1,1,0.2);
	move_enemy_renderable(enemy, MikHeight);
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
		
		move_enemy(ElapsedTime, enemy.m_CurrentWp, enemy, MikHeight)
		mesh = enemy.m_RenderableObject
		dir = Vect3f(math.cos(mesh:get_yaw()), 0, math.sin(mesh:get_yaw()))
		sound_manager:SetGameObjectPosition(name, mesh:get_position(), dir)
		
		local temp_zone = enemy.m_Zone
		if temp_zone == enemy.m_RenderableObject.m_Room then
			--coreInstance:trace("Sala = zona"
			temp_zone = temp_zone..".0"
			--coreInstance:trace(tostring(temp_zone))
		end
		
		if check_attack(enemy) == true and tostring(temp_zone) == tostring(player.zone) then
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
--coreInstance:trace("Calculando: " .. name)
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
--coreInstance:trace("Atacando: " .. name)
	local enemy = enemy_manager:get_enemy(name)
	if enemy ~= nil then
		if not enemy:is_static() then -- en caso de no ser estatico
			enemy.m_Speed = enemy.m_AttackSpeed
			enemy.m_SpeedModified = true
			enemy.m_RenderableObject:blend_cycle(1,0.1,0.2);
		end
		exclamation = particle_manager:get_resource(enemy.m_RenderableObject.m_ParticleEmitter2)
		if exclamation ~= nil then
			exclamation:set_visible(true)
			exclamation.m_FireParticles = true;
			exclamation.m_vPos = enemy.m_RenderableObject:get_position() + enemy.m_RenderableObject.m_EmitterOffset2;
			enemy.m_visibleTime = 0;
			sound_manager:PlayEvent("Mik_Saw", name);
		end
	end
	return 0
end

function mikmik_exit_attack_player(name)
	local enemy = enemy_manager:get_enemy(name)
	if enemy ~= nil and enemy:is_static() == false then -- en caso de no ser estatico
		enemy.m_Speed = enemy.m_OriginalSpeed
		enemy.m_SpeedModified = false
		enemy.m_CurrentTime = 0
		enemy.m_isAttacking = false
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
	if enemy ~= nil and not player.is_dead then
	--local billboard = billboard_manager:get_resource(enemy.m_RenderableObject.m_Billboard)
	--if billboard ~= nil then
	--coreInstance:trace("Mik Ataque Position: ".. tostring(enemy:get_position().x) .. ", " .. tostring(enemy:get_position().y) .. ", " ..tostring(enemy:get_position().z))
		--billboard.m_position = enemy.m_RenderableObject:get_position()+ enemy.m_RenderableObject.m_BillboardOffset
		--coreInstance:trace("BillBoard Position: ".. tostring(billboard.m_position.x) .. ", " .. tostring(billboard.m_position.y) .. ", " ..tostring(billboard.m_position.z))
	--end
		local exclamation = particle_manager:get_resource(enemy.m_RenderableObject.m_ParticleEmitter2)
		if (exclamation ~= nil) then
			
			if (enemy.m_visibleTime > visibleTime) then
				exclamation:set_visible(false);
			else
				enemy.m_visibleTime = enemy.m_visibleTime + ElapsedTime;
				exclamation.m_vPos = enemy.m_RenderableObject:get_position() + enemy.m_RenderableObject.m_EmitterOffset2;
				--coreInstance:trace(tostring(enemy.m_visibleTime));
				--coreInstance:trace("Moving billboard?");
			end
		end
		local platform = enemy:get_platform()
		if platform ~= nil and platform.m_Direction ~= nil and enemy.m_isAlive then
			
				enemy.m_PhysicController:move(platform.m_Direction * platform.m_Speed * ElapsedTime, ElapsedTime)
				local mik_position = Vect3f(enemy.m_PhysicController:get_position().x, enemy.m_RenderableObject:get_position().y, enemy.m_PhysicController:get_position().z)
			
				if mik_position.y < platform:get_position().y then
				
					mik_position= Vect3f( platform:get_position().x-1, platform:get_position().y +2, platform:get_position().z-1)
					enemy.m_PhysicController:set_position(mik_position)
				end
				enemy.m_RenderableObject:set_position(mik_position)
				mesh = enemy.m_RenderableObject
				dir = Vect3f(math.cos(mesh:get_yaw()), 0, math.sin(mesh:get_yaw()))
				sound_manager:SetGameObjectPosition(name, mesh:get_position(), dir)
			
		end
	--coreInstance:trace("Enemy Zone" .. tostring (enemy.m_Zone))
	--coreInstance:trace("player Zone" .. tostring (player.zone))
		if enemy.m_isAttacking == false  and player.is_dead == false then
			local enemyPosXZ = Vect3f(enemy:get_position().x, 0, enemy:get_position().z)
			local playerPosXZ = Vect3f(player_position.x, 0, player_position.z)
			local player_distance = get_distance_to_player(enemyPosXZ, playerPosXZ)
			local platform_distance = nil
			if platform ~= nil then
				local platformPosXZ = Vect3f(platform:get_position().x, 0, platform:get_position().z)
				platform_distance = get_distance_to_player(enemyPosXZ, platformPosXZ )
			end
			
			local temp_zone = enemy.m_Zone
			if temp_zone == enemy.m_RenderableObject.m_Room then
				--coreInstance:trace("Sala = zona")
				temp_zone = temp_zone..".0"
				--coreInstance:trace(tostring(temp_zone))
			end
			
			if player.is_hit == false and tostring(temp_zone) == tostring(player.zone) and not enemy:is_static() then
				if (player.on_air == false) or (player.on_air == true and player_distance > 2) then
					move_enemy(ElapsedTime, player_position, enemy, MikHeight) -- en caso de no ser estatico
					mesh = enemy.m_RenderableObject
					dir = Vect3f(math.cos(mesh:get_yaw()), 0, math.sin(mesh:get_yaw()))
					sound_manager:SetGameObjectPosition(name, mesh:get_position(), dir)
				end
			end
			
			if player_distance > (enemy.m_AttackPlayerDistance * 2.5) or (platform_distance ~= nil and platform_distance > 9 ) then
				enemy:m_FSM():newState("Parado")
			else
				--local damageType = enemy:actualizar_hitbox() --esto setea take damage si le pegan
				local damageType = enemy:check_player_collision()
				-- damage 1 : player pega con ataque
				-- damage 2 : player pega con salto
				-- damage 3 : enemy pega player
				-- damage 4 : no hay damage, el player se imanta a la posicion del enemy
				-- damage 0 : nada
				if damageType == 3 and not player.super_piky_active then
					enemy.m_RenderableObject:clear_cycle(0,0.2);
					enemy.m_RenderableObject:clear_cycle(1,0.2);
					enemy.m_RenderableObject:execute_action(2,0.1,0,1,true);
					enemy.m_isAttacking = true;
					enemy:add_damage_player()
					sound_manager:PlayEvent("Mik_Attack", name);
				elseif damageType == 1 or (damageType == 3 and player.super_piky_active)then
					coreInstance:trace("Damage type: "..tostring(damageType))
					coreInstance:trace("SuperPiky: "..tostring(player.super_piky_active))
					enemy.m_time_to_fly = true;
					enemy.m_playAnimationDead = false;
					enemy.m_flyVec = Vect3f(enemyPosXZ.x - playerPosXZ.x, 0,enemyPosXZ.z - playerPosXZ.z);
					enemy.m_flyVec:normalize(1);
					enemy.m_flyVec = Vect3f(enemy.m_flyVec.x, flyInclination, enemy.m_flyVec.z);
					enemy:m_FSM():newState("Take_Damage")
					if enemy:get_wp_vector_size() > 0 then
						local dead_pos = enemy.m_PhysicController:get_position()
						enemy.m_OriginalPosition = Vect3f(dead_pos.x,dead_pos.y,dead_pos.z)
					--coreInstance:trace("Mik pillando posicion: " ..tostring(enemy.m_RenderableObject:get_position().x)..", "..tostring(enemy.m_RenderableObject:get_position().y)..", "..tostring(enemy.m_RenderableObject:get_position().z))
					end
				elseif damageType == 2 then
					player_controller.m_executeDoubleJump = true;
					enemy.m_playAnimationDead = true;
					enemy:m_FSM():newState("Take_Damage")
					if enemy:get_wp_vector_size() > 0 then
						local dead_pos = enemy.m_PhysicController:get_position()
						enemy.m_OriginalPosition = Vect3f(dead_pos.x,dead_pos.y,dead_pos.z)
					--coreInstance:trace("Mik pillando posicion: " ..tostring(enemy.m_RenderableObject:get_position().x)..", "..tostring(enemy.m_RenderableObject:get_position().y)..", "..tostring(enemy.m_RenderableObject:get_position().z))
					end
				elseif damageType == 4 then
					player.iman_pos = Vect3f(enemy:get_position().x,enemy:get_position().y,enemy:get_position().z);
					player.use_iman = true;
				end
			end
			
			if tostring(temp_zone) ~= tostring(player.zone) then
				enemy:m_FSM():newState("Parado")
			end
		else
			if not enemy.m_RenderableObject:is_cycle_animation_active() then
				enemy.m_RenderableObject:remove_action(2);
				if not enemy:is_static() then
					enemy.m_RenderableObject:blend_cycle(1,1,0.2);
				else
					enemy.m_RenderableObject:blend_cycle(0,1,0.2);
				end
				enemy.m_isAttacking = false;
			end
		end
	end
	
	
end

function mikmik_enter_take_damage(name)
	local enemy = enemy_manager:get_enemy(name)
	if enemy ~= nil then
		enemy.m_CurrentTime = 0
		enemy.m_Life = enemy.m_Life - 1
		local exclamation = particle_manager:get_resource(enemy.m_RenderableObject.m_ParticleEmitter2)
		if exclamation ~= nil then
			exclamation:set_visible(false)
			enemy.m_visibleTime = 0;
		end
		
		if(enemy.m_Life <= 0) then
			enemy.m_IsDying = true
		end
		
		if(enemy.m_Life <= 0 and enemy.m_time_to_fly == false) then
			enemy:m_FSM():newState("Dead")
		else 
			if enemy.m_RenderableObject:is_action_animation_active() then
				enemy.m_RenderableObject:remove_action(2)
			end
			if	enemy.m_RenderableObject:is_cycle_animation_active() then
				enemy.m_RenderableObject:clear_cycle(0,0.2);
				enemy.m_RenderableObject:clear_cycle(1,0.2);
			end
			enemy.m_RenderableObject:execute_action(4,0.1,0,1,false)
		end
	end
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
	if(enemy.m_Life <= 0 and not enemy.m_IsDying ) then
		enemy.m_IsDying = true
	end
	if enemy.m_time_to_fly == true then
		local auxVec = enemy.m_flyVec * flySpeed;
		local isGrounded = enemy.m_PhysicController:move(auxVec * ElapsedTime, ElapsedTime);
		move_enemy_renderable(enemy, MikHeight);
		if not enemy.m_RenderableObject:is_action_animation_active() then
			enemy.m_time_to_fly = false;
			enemy.m_flyVec = Vect3f(0,0,0);
			if enemy.m_Life <= 0 then
				enemy:m_FSM():newState("Dead")
			else
				enemy:m_FSM():newState("Parado")
			end
		else
			enemy.m_flyVec = Vect3f(enemy.m_flyVec.x, enemy.m_flyVec.y - flyDecay * ElapsedTime, enemy.m_flyVec.z);
			enemy.m_CurrentTime = enemy.m_CurrentTime + ElapsedTime;
		end
	elseif not enemy.m_RenderableObject:is_action_animation_active() then
		if enemy.m_Life <= 0 then
			enemy:m_FSM():newState("Dead")
		else
			enemy:m_FSM():newState("Parado")
		end
	end
end

function mikmik_enter_dead(name)
	local enemy = enemy_manager:get_enemy(name)
	if enemy ~= nil then
		sound_manager:PlayEvent("Mik_Dead", name);
		--coreInstance:trace("Init muerto Mik")
		enemy.m_RenderableObject:clear_cycle(0,0.2);
		enemy.m_RenderableObject:clear_cycle(1,0.2);
		enemy.m_RenderableObject:remove_action(2);
		if enemy.m_playAnimationDead == true then
			enemy.m_RenderableObject:execute_action(3,0.1,0,1,true);
		end
		
		enemy.m_isAlive = false
		if player_controller.m_Room == 3 then		
			player.enemies_killed = player.enemies_killed + 1
		--check_enemies_killed(5, "door_001")
			check_enemies_killed(5, "Puerta_arriba", "Puerta_abajo")
		elseif player_controller.m_Room == 0 then
			boss_miks_killed = boss_miks_killed +1
			if boss_miks_killed == 5 then
				all_boss_miks_killed = true
			end
		end
		--enemy.m_PhysicController:set_radius(0.01)
		 
	end
end



function mikmik_exit_dead(name)
	local enemy = enemy_manager:get_enemy(name)
	if enemy ~= nil then
		enemy.m_RenderableObject.m_Printable=true
		enemy.m_CurrentTime = 0
		enemy.m_Returning = false
		enemy.m_RenderableObject:remove_action(3);
	--	coreInstance:trace("setting alive")
	end
end

function mikmik_update_dead(ElapsedTime, doComprobation, name)
	local enemy = enemy_manager:get_enemy(name)
	if enemy ~= nil then
		if not enemy.m_RenderableObject:is_action_animation_active() and not enemy.m_isAlive and not enemy.m_MovedToDiePosition then
			enemy.m_CurrentTime = enemy.m_CurrentTime + 1 *ElapsedTime
			--coreInstance:trace("Update muerto Mik sin accion")
			enemy.m_RenderableObject:blend_cycle(0,0.1,0)
			local emitter = particle_manager:get_resource(enemy.m_RenderableObject.m_ParticleEmitter)
			if emitter ~= nil then
				emitter.m_vPos = enemy.m_RenderableObject:get_position()+ enemy.m_RenderableObject.m_EmitterOffset
				emitter.m_FireParticles = true 
			end
			if enemy.m_CurrentTime > 0.1 then
				enemy.m_RenderableObject.m_Printable=false
				local dead_pos = enemy.m_PhysicController:get_position()
				dead_pos.y = dead_pos.y + 1000
				enemy:set_position(dead_pos)
				enemy.m_PhysicController:set_position(dead_pos)
				enemy:move_to_position(dead_pos)
				enemy.m_MovedToDiePosition  = true
				enemy.m_RenderableObject:clear_cycle(0,0.1)
			end
		--elseif enemy.m_RenderableObject:is_action_animation_active() and not enemy.m_isAlive then
			--enemy.m_RenderableObject:execute_action(3,0.1,0,1,false);
		end
		
		
	end
end

function mikmik_enter_waiting(name)
	local enemy = enemy_manager:get_enemy(name)
	local position = enemy.m_PhysicController:get_position()
	--enemy.m_OriginalPosition = position
	position.y = position.y + 1000
	enemy:set_position(position)
	enemy.m_PhysicController:set_position(position)
	enemy:move_to_position(position)
	coreInstance:trace("Entro en waiting: "..name)
end

function mikmik_exit_waiting(name)
	local enemy = enemy_manager:get_enemy(name)
	enemy:set_position(enemy.m_OriginalPosition)
	enemy.m_PhysicController:set_position(enemy.m_OriginalPosition)
	enemy.m_RenderableObject:set_position(enemy.m_OriginalPosition)
end

function mikmik_update_waiting(ElapsedTime, doComprobation, name)
	
end