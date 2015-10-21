--local coreInstance = CCoreLuaWrapper().m_CoreInstance

--local current_time = 0
--ai_controller = CAIController()
--ai_controller.m_Speed = 0.1
--local instance = CLuaGlobalsWrapper().m_CoreInstance;

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


function check_attack (_enemy)
	local player_position = player_controller:get_position()
	local player_distance = get_distance_to_player(_enemy:get_position(), player_position)
	if player_distance <= _enemy.m_AttackPlayerDistance and player.is_hit == false then
		return true
	end
	return false
end

function move_enemy(ElapsedTime, _point, Enemy, height)
	if player_controller.m_is3D == true then
		move_to(Enemy, ElapsedTime, _point, height)
	else
		rotate_or_move(Enemy, ElapsedTime, _point, height)
	end
end


function move_to(_enemy, ElapsedTime, _point, height)
	--if _point:distance(_enemy:get_position()) >= 2 then
		rotate_yaw (_enemy, ElapsedTime, _point)
		-- si no es estatico
		if _enemy:is_static() == false then
			_enemy.m_PhysicController:move(Vect3f(1, 0, 0):rotate_y(_enemy:get_yaw()) * _enemy.m_Speed * ElapsedTime, ElapsedTime)
			move_enemy_renderable(_enemy, height)	
		end
	--end
end


function rotate_or_move(_enemy, ElapsedTime, _point, height)
	--if _point:distance(_enemy:get_position()) >= 2 then
		local direction = Vect3f(_point - _enemy:get_position())
		direction = direction:normalize(1.0)
		local diff = Vect3f(1.0,0.0,0.0):rotate_y(_enemy:get_yaw())
		local angle = _enemy:get_angle_diff(direction, diff)
		if angle > 0.5 then
			rotate_yaw(_enemy, ElapsedTime, _point)
		else
			if _enemy:is_static() == false then
				_enemy.m_PhysicController:move(direction * _enemy.m_Speed  * ElapsedTime, ElapsedTime)
				move_enemy_renderable(_enemy, height)
			end
		end
	--end
end

function move_enemy_renderable(_enemy, height)
	local l_position = _enemy.m_PhysicController:get_position()
	_enemy:set_position(l_position)
	l_position.y = l_position.y - height;
	if _enemy.m_RenderableObject ~= nil then
		_enemy.m_RenderableObject:set_position(l_position)
	end		
end

function rotate_yaw(_enemy, ElapsedTime, _point)
	local direction = Vect3f(_point - _enemy:get_position())
	direction.y = 0
	direction = direction:normalize(1.0)
	local diff = Vect3f(1.0,0.0,0.0):rotate_y(_enemy:get_yaw())
	local angle = _enemy:get_angle_diff(direction, diff)
	rotate_renderable(ElapsedTime, angle, _enemy)
end

function rotate_renderable(ElapsedTime, angle, _enemy)
	_enemy:set_yaw(_enemy:get_yaw() - angle * _enemy.m_TurnSpeed * ElapsedTime)

	if _enemy.m_RenderableObject ~= nil then
		_enemy.m_RenderableObject:set_yaw(_enemy:get_yaw() - math.pi/2)
	end
end

function update_boss_shoot_cooldown(enemy, player_position)
	if not enemy.m_IsOnCooldown then
	  enemy.m_IsOnCooldown = true;
      enemy.m_CurrentCooldown = enemy.m_CooldownTimer;
	  boss_shoot(enemy:get_position(), enemy)
	  enemy.m_DireccionBala = player_position - enemy.m_PosicionBala
	  local vectPlayerXZ = Vect3f(player_position.x, 0, player_position.z);
	  local vectEnemyXZ = Vect3f(enemy:get_position().x, 0, enemy:get_position().z);
	  local vectDist = vectPlayerXZ - vectEnemyXZ;
	  local distPlayerEnemy = vectDist:length();
	  local inclinacion = ((distPlayerEnemy - 12) / 3.4)+0.5; -- valores hardcodeados 11.6, 3.4
	  enemy.m_DireccionBala = Vect3f(enemy.m_DireccionBala.x, enemy.m_DireccionBala.y + inclinacion, enemy.m_DireccionBala.z)
	  enemy.BalaActiva = true;
	end
end

function boss_shoot(position, enemy)
	if (enemy.m_isAlive) then	
		local enemyYaw = enemy:get_yaw() + (math.pi / 2);
		local vectRight = Vect3f(math.cos(enemyYaw), 0, -math.sin(enemyYaw));
		local projectile_position = Vect3f(position.x + 2 * vectRight.x, position.y+2.25, position.z + 2 * vectRight.z)
		enemy.m_PosicionBala = projectile_position
		local projectile_name = enemy.m_ProjectileName
--[[
		if not player.super_piky_active and check_random_action(6) then
			projectile_name = powerup_name
			current_shot_type = "powerup"
		else
			current_shot_type = "rock"
		end]]
		if current_shot_type == "powerup" then
			projectile_name = powerup_name
		end
		local renderable_shoot = get_renderable_object("enemies", 0, projectile_name)
		if renderable_shoot ~= nil then
			-- poner sonido y particulas de disparo	
			local playerRenderableMesh =  coreInstance:get_renderable_object_layer_manager():get_renderable_objects_manager_by_str_and_room(piky_layer, player_controller.m_Room):get_resource(piky_mesh_name);
	
			local qte_emmiter_name = playerRenderableMesh.m_ParticleEmitter
			if not inputm:has_game_pad(1) then
				qte_emmiter_name = playerRenderableMesh.m_ParticleEmitter2
			end
			local emitter = particle_manager:get_resource(qte_emmiter_name)
			if emitter ~= nil then
				emitter.m_vPos = enemy.m_RenderableObject:get_position()+ enemy.m_RenderableObject.m_EmitterOffset
				emitter.m_FireParticles = true 
			end
			
			
			renderable_shoot:set_position(enemy.m_PosicionBala)
			renderable_shoot:set_yaw(enemy:get_yaw());
			renderable_shoot.m_Printable = true
			renderable_shoot:set_visible(true)
		end
	  end
end

function update_cooldown(enemy, dt, player_position)
	if not enemy.m_IsOnCooldown then
	  enemy.m_IsOnCooldown = true;
      enemy.m_CurrentCooldown = enemy.m_CooldownTimer;
	  shoot_to_vector(dt, enemy:get_position(), enemy)
	  if not player.is_dead then
		sound_manager:PlayEvent("Pum_Attack", enemy.m_RenderableObject:get_name())
	  end
	  enemy.m_DireccionBala = player_position - enemy:get_position()
	  enemy.BalaActiva = true;
	end
end

function shoot_to_vector(dt, position, enemy)
  if (enemy.m_isAlive) then
	local projectile_position = Vect3f(position.x, position.y+0.25, position.z)
    enemy.m_PosicionBala = projectile_position
    local renderable_shoot = get_renderable_object("lava", enemy.m_RenderableObject.m_Room, enemy.m_ProjectileName)
	
	if renderable_shoot ~= nil then
		renderable_shoot:set_yaw(enemy:get_yaw())
		-- poner sonido y particulas de disparo	
		local emitter = particle_manager:get_resource(enemy.m_RenderableObject.m_ParticleEmitter)
		if emitter ~= nil then
			emitter.m_vPos = enemy.m_RenderableObject:get_position()+ enemy.m_RenderableObject.m_EmitterOffset
			emitter.m_FireParticles = true 
		end
		
		local emitter2 = particle_manager:get_resource(renderable_shoot.m_ParticleEmitter2)
		if emitter2 ~= nil then
			emitter2.m_vPos = enemy.m_PosicionBala
			emitter2:set_visible(true)
		end
		
		renderable_shoot:set_position(enemy.m_PosicionBala)
		renderable_shoot.m_Printable = true
		renderable_shoot:set_visible(true)
	end
  end
end

function update_shoot(dt, enemy)
 local renderable_shoot = get_renderable_object("lava", enemy.m_RenderableObject.m_Room, enemy.m_ProjectileName)
	local emitter2 = particle_manager:get_resource(renderable_shoot.m_ParticleEmitter2)
 if (enemy.m_IsOnCooldown) then
	coreInstance:trace("Updateando disparo pum!!")
	
    enemy.m_CurrentCooldown = enemy.m_CurrentCooldown - dt;
    if (enemy.m_CurrentCooldown < 0.0) then
      enemy.m_IsOnCooldown = false;
	  enemy.BalaActiva = false;
      delete_shooting(renderable_shoot)
		if emitter2 ~= nil then
			emitter2:set_visible(false)
		end
    else 
      enemy.m_PosicionBala = enemy:updtate_projectile_position(dt) 
	  renderable_shoot:set_position(enemy.m_PosicionBala)
	
		if emitter2 ~= nil then
			emitter2.m_vPos = enemy.m_PosicionBala
		end
      if (check_player_shoot_collision(enemy, renderable_shoot)) then
		enemy.BalaActiva = false;
		if emitter2 ~= nil then
			emitter2:set_visible(false)
		end
		delete_shooting(renderable_shoot)
        enemy:add_damage_player();
      end
    end
  end
	if player.is_dead then
	  enemy.m_IsOnCooldown = false;
	  enemy.BalaActiva = false;
	  if emitter2 ~= nil then
		emitter2:set_visible(false)
	  end
      delete_shooting(renderable_shoot)
	end
end

function update_shoot_boss(dt, enemy)
	local gravityShot = 0.3;
 if (enemy.m_IsOnCooldown) then
	local playerRenderable =  coreInstance:get_renderable_object_layer_manager():get_renderable_objects_manager_by_str_and_room(piky_layer, player_controller.m_Room):get_resource(piky_mesh_name);
				
	local projectile_name = enemy.m_ProjectileName
	if current_shot_type == "powerup" then
		projectile_name = powerup_name
	end
	local renderable_shoot = get_renderable_object("enemies", enemy.m_RenderableObject.m_Room, projectile_name)
    enemy.m_CurrentCooldown = enemy.m_CurrentCooldown - dt;
    if (enemy.m_CurrentCooldown < 0.0 or player.is_dead) then
      enemy.m_IsOnCooldown = false;
	  enemy.BalaSpeed = enemy.BalaOriginalSpeed;
	  current_speed_change = inicial_speed_change;
	  actual_speed_change = 0;
	  enemy.BalaActiva = false;
	  fire_boss_projectile_particles(renderable_shoot, enemy, false)
      delete_shooting(renderable_shoot)
	  enemy:m_FSM():newState("Parado")
    else 
		enemy.m_DireccionBala = Vect3f(enemy.m_DireccionBala.x, enemy.m_DireccionBala.y - gravityShot * dt, enemy.m_DireccionBala.z);
		enemy.m_PosicionBala = enemy:updtate_projectile_position(dt)
		renderable_shoot:set_position(enemy.m_PosicionBala)
		renderable_shoot:set_pitch(renderable_shoot:get_pitch() + 2 * dt);
		check_player_shoot_return(enemy, renderable_shoot);
		if (check_player_shoot_collision(enemy, renderable_shoot)) then
			enemy.m_IsOnCooldown = false;
			enemy.BalaActiva = false;
			--fire_boss_projectile_particles(renderable_shoot, enemy)
			delete_shooting(renderable_shoot)
			enemy.BalaSpeed = enemy.BalaOriginalSpeed;
			current_speed_change = inicial_speed_change;
			actual_speed_change = 0;
			enemy:m_FSM():newState("Parado")
			if current_shot_type =="powerup" then
				start_super_piky()
			else
				enemy:add_damage_player();
				fire_boss_projectile_particles(renderable_shoot, enemy, false)
				local qte_emmiter_name = playerRenderable.m_ParticleEmitter
				if not inputm:has_game_pad(1) then
					qte_emmiter_name = playerRenderable.m_ParticleEmitter2
				end
				local emitter = particle_manager:get_resource(qte_emmiter_name)
				if emitter:get_visible() == true then
					emitter:set_visible(false)
				end
			end
		end
		local posXZBala = Vect3f(enemy.m_PosicionBala.x, 0, enemy.m_PosicionBala.z)
		local posXZChucky = Vect3f(enemy:get_position().x, 0,enemy:get_position().z)
		local distance = get_distance_between_points(posXZBala, posXZChucky)
		--local posXZPlayer = Vect3f(player_controller:get_position().x,0,player_controller:get_position().z)
		--local distance2 = get_distance_between_points(posXZPlayer, posXZChucky)
		--coreInstance:trace("Distancia player al boss: "..tostring(distance2))
		--coreInstance:trace(tostring(enemy.m_PosicionBala.y))
		--coreInstance:trace("Distancia: "..tostring(distance))
		local vectPoly = renderable_objects_layer_manager:get_renderable_objects_manager_by_str_and_room("poly", player_controller.m_Room);
		local vectActivePoly = renderable_objects_layer_manager:get_renderable_objects_manager_by_str_and_room("enabled_poly", player_controller.m_Room);
		local numPolis = vectPoly:get_size();
		local numEnabledPolis = vectActivePoly:get_size();
		local choca = false;
		for i=0, numPolis-1 do
			local platform = vectPoly:get_resource_by_id(i);
			if platform ~= nil then
				if platform.m_Activated or platform.m_IsMoving then
					local platformPosXZ = Vect3f(platform:get_position().x, 0, platform:get_position().z);
					local platformSizeXZ = Vect3f(platform.m_PhysicsSize.x, 0, platform.m_PhysicsSize.z);
					local vectToBossXZ = platformPosXZ - posXZChucky;
					local vectToBalaXZ = platformPosXZ - posXZBala;
					local distanceToBala = vectToBalaXZ:length();
					if vectToBossXZ.x ~= 0 or vectToBossXZ.y ~= 0 or vectToBossXZ.z ~= 0 then
						vectToBossXZ:normalize(1);
					end
					if vectToBalaXZ.x ~= 0 or vectToBalaXZ.y ~= 0 or vectToBalaXZ.z ~= 0 then
						vectToBalaXZ:normalize(1);
					end
					local dot = vectToBossXZ * vectToBalaXZ;
					dot = math.abs(dot);
					local sizeLargo = 0;
					local sizeCorto = 0;
					if platformSizeXZ.x > platformSizeXZ.z then
						sizeLargo = platformSizeXZ.x;
						sizeCorto = platformSizeXZ.z;
					else
						sizeLargo = platformSizeXZ.z;
						sizeCorto = platformSizeXZ.x;
					end
					local valorCorto = sizeCorto * dot;
					local valorLargo = sizeLargo * (1-dot);
					local distanceToCrashAlCuadrao = valorCorto * valorCorto + valorLargo * valorLargo;
					distanceToCrashAlCuadrao = distanceToCrashAlCuadrao + (enemy.m_ProjectileHitbox * enemy.m_ProjectileHitbox);
					local distanceToBalaAlCuadrao = distanceToBala * distanceToBala;
					if distanceToBalaAlCuadrao <= distanceToCrashAlCuadrao then
						choca = true;
					end
				end
			end
		end
		for i=0, numEnabledPolis-1 do
			local platform = vectActivePoly:get_resource_by_id(i);
			if platform ~= nil then
				if platform.m_Activated or platform.m_IsMoving then
					local platformPosXZ = Vect3f(platform:get_position().x, 0, platform:get_position().z);
					local platformSizeXZ = Vect3f(platform.m_PhysicsSize.x, 0, platform.m_PhysicsSize.z);
					local vectToBossXZ = platformPosXZ - posXZChucky;
					local vectToBalaXZ = platformPosXZ - posXZBala;
					local distanceToBala = vectToBalaXZ:length();
					if vectToBossXZ.x ~= 0 or vectToBossXZ.y ~= 0 or vectToBossXZ.z ~= 0 then
						vectToBossXZ:normalize(1);
					end
					if vectToBalaXZ.x ~= 0 or vectToBalaXZ.y ~= 0 or vectToBalaXZ.z ~= 0 then
						vectToBalaXZ:normalize(1);
					end
					local dot = vectToBossXZ * vectToBalaXZ;
					dot = math.abs(dot);
					local sizeLargo = 0;
					local sizeCorto = 0;
					if platformSizeXZ.x > platformSizeXZ.z then
						sizeLargo = platformSizeXZ.x;
						sizeCorto = platformSizeXZ.z;
					else
						sizeLargo = platformSizeXZ.z;
						sizeCorto = platformSizeXZ.x;
					end
					local valorCorto = sizeCorto * dot;
					local valorLargo = sizeLargo * (1-dot);
					local distanceToCrashAlCuadrao = valorCorto * valorCorto + valorLargo * valorLargo;
					distanceToCrashAlCuadrao = distanceToCrashAlCuadrao + (enemy.m_ProjectileHitbox * enemy.m_ProjectileHitbox);
					local distanceToBalaAlCuadrao = distanceToBala * distanceToBala;
					if distanceToBalaAlCuadrao <= distanceToCrashAlCuadrao then
						choca = true;
					end
				end
			end
		end
		if ((distance > 450 and enemy.m_PosicionBala.y < -1.2) or (distance <=450 and enemy.m_PosicionBala.y < 1.8)) or choca then
			enemy.m_IsOnCooldown = false;
			enemy.BalaSpeed = enemy.BalaOriginalSpeed;
			current_speed_change = inicial_speed_change;
			actual_speed_change = 0;
			enemy.BalaActiva = false;
			if not choca then
				local dust_emitter = particle_manager:get_resource(renderable_shoot.m_ParticleEmitter)
				if dust_emitter ~= nil then
					coreInstance:trace("Proyectil Roto")
					dust_emitter.m_vPos = enemy.m_PosicionBala + renderable_shoot.m_EmitterOffset
					dust_emitter.m_FireParticles = true
				end
			end
			
			fire_boss_projectile_particles(renderable_shoot, enemy, true)
			delete_shooting(renderable_shoot)
			local qte_emmiter_name = playerRenderable.m_ParticleEmitter
			if not inputm:has_game_pad(1) then
				qte_emmiter_name = playerRenderable.m_ParticleEmitter2
			end
			local emitter = particle_manager:get_resource(qte_emmiter_name)
			if emitter:get_visible() == true then
				emitter:set_visible(false)
			end
			enemy:m_FSM():newState("Parado")
		end
	end
  end
end
function fire_boss_projectile_particles(renderable, enemy, use_offset)
	local projectile_particles = {renderable.m_ParticleEmitter2, renderable.m_ParticleEmitter3}
	local offsets = {renderable.m_EmitterOffset2, renderable.m_EmitterOffset3} 
	for i = 1 , table.getn(projectile_particles) do
		local particle_emitter = particle_manager:get_resource(projectile_particles[i])
		if use_offset then
			particle_emitter.m_vPos = enemy.m_PosicionBala + offsets[i]
		else
			particle_emitter.m_vPos = enemy.m_PosicionBala 
		end
		particle_emitter.m_FireParticles = true
	end
end

function update_horizontal_boss_shoot(dt, enemy)

 if (enemy.m_IsOnCooldown) then
	local renderable_shoot = get_renderable_object("enemies", enemy.m_RenderableObject.m_Room, enemy.m_ProjectileName)
	local playerRenderable =  coreInstance:get_renderable_object_layer_manager():get_renderable_objects_manager_by_str_and_room(piky_layer, player_controller.m_Room):get_resource(piky_mesh_name);
    --enemy.m_CurrentCooldown = enemy.m_CurrentCooldown - dt;
    if (player.is_dead) then
	  enemy.m_IsOnCooldown = false;
	  enemy.BalaSpeed = enemy.BalaOriginalSpeed;
	  current_speed_change = inicial_speed_change;
	  actual_speed_change = 0;
	  enemy.BalaActiva = false;
	  fire_boss_projectile_particles(renderable_shoot, enemy, false)
	  delete_shooting(renderable_shoot)
	  enemy:m_FSM():newState("Parado")
	  local qte_emmiter_name = playerRenderable.m_ParticleEmitter
	  if not inputm:has_game_pad(1) then
		qte_emmiter_name = playerRenderable.m_ParticleEmitter2
	  end
	  local emitter = particle_manager:get_resource(qte_emmiter_name)
	  if emitter:get_visible() == true then
		emitter:set_visible(false)
	  end
    else 
      enemy.m_PosicionBala = enemy:updtate_projectile_position(dt) 
	  renderable_shoot:set_position(enemy.m_PosicionBala)
	  renderable_shoot:set_roll(renderable_shoot:get_roll() + 2 * dt * enemy.BalaSpeed)
	  check_player_shoot_return(enemy, renderable_shoot);
      if (check_player_shoot_collision(enemy, renderable_shoot)) then
		enemy.m_IsOnCooldown = false;
		enemy.BalaSpeed = enemy.BalaOriginalSpeed;
		current_speed_change = inicial_speed_change;
		actual_speed_change = 0;
		enemy.BalaActiva = false;
		fire_boss_projectile_particles(renderable_shoot, enemy, false)
		delete_shooting(renderable_shoot)
		
		local qte_emmiter_name = playerRenderable.m_ParticleEmitter
		if not inputm:has_game_pad(1) then
			qte_emmiter_name = playerRenderable.m_ParticleEmitter2
		end
		local emitter = particle_manager:get_resource(qte_emmiter_name)
		if emitter:get_visible() == true then
			emitter:set_visible(false)
		end
        enemy:add_damage_player();
		enemy:m_FSM():newState("Parado")
      end
		local posXZBala = Vect3f(enemy.m_PosicionBala.x, 0, enemy.m_PosicionBala.z)
		local posXZChucky = Vect3f(enemy:get_position().x, 0,enemy:get_position().z)
		local distance = get_distance_between_points(posXZBala, posXZChucky)
		local vectPoly = renderable_objects_layer_manager:get_renderable_objects_manager_by_str_and_room("poly", player_controller.m_Room);
		local vectActivePoly = renderable_objects_layer_manager:get_renderable_objects_manager_by_str_and_room("enabled_poly", player_controller.m_Room);
		local numPolis = vectPoly:get_size();
		local numEnabledPolis = vectActivePoly:get_size();
		local choca = false;
		for i=0, numPolis-1 do
			local platform = vectPoly:get_resource_by_id(i);
			if platform ~= nil then
				if platform.m_Activated or platform.m_IsMoving then
					local platformPosXZ = Vect3f(platform:get_position().x, 0, platform:get_position().z);
					local platformSizeXZ = Vect3f(platform.m_PhysicsSize.x, 0, platform.m_PhysicsSize.z);
					local vectToBossXZ = platformPosXZ - posXZChucky;
					local vectToBalaXZ = platformPosXZ - posXZBala;
					local distanceToBala = vectToBalaXZ:length();
					if vectToBossXZ.x ~= 0 or vectToBossXZ.y ~= 0 or vectToBossXZ.z ~= 0 then
						vectToBossXZ:normalize(1);
					end
					if vectToBalaXZ.x ~= 0 or vectToBalaXZ.y ~= 0 or vectToBalaXZ.z ~= 0 then
						vectToBalaXZ:normalize(1);
					end
					local dot = vectToBossXZ * vectToBalaXZ;
					dot = math.abs(dot);
					local sizeLargo = 0;
					local sizeCorto = 0;
					if platformSizeXZ.x > platformSizeXZ.z then
						sizeLargo = platformSizeXZ.x;
						sizeCorto = platformSizeXZ.z;
					else
						sizeLargo = platformSizeXZ.z;
						sizeCorto = platformSizeXZ.x;
					end
					local valorCorto = sizeCorto * dot;
					local valorLargo = sizeLargo * (1-dot);
					local distanceToCrashAlCuadrao = valorCorto * valorCorto + valorLargo * valorLargo;
					distanceToCrashAlCuadrao = distanceToCrashAlCuadrao + (enemy.m_ProjectileHitbox * enemy.m_ProjectileHitbox);
					local distanceToBalaAlCuadrao = distanceToBala * distanceToBala;
					if distanceToBalaAlCuadrao <= distanceToCrashAlCuadrao then
						choca = true;
					end
				end
			end
		end
		for i=0, numEnabledPolis-1 do
			local platform = vectActivePoly:get_resource_by_id(i);
			if platform ~= nil then
				if platform.m_Activated or platform.m_IsMoving then
					local platformPosXZ = Vect3f(platform:get_position().x, 0, platform:get_position().z);
					local platformSizeXZ = Vect3f(platform.m_PhysicsSize.x, 0, platform.m_PhysicsSize.z);
					local vectToBossXZ = platformPosXZ - posXZChucky;
					local vectToBalaXZ = platformPosXZ - posXZBala;
					local distanceToBala = vectToBalaXZ:length();
					if vectToBossXZ.x ~= 0 or vectToBossXZ.y ~= 0 or vectToBossXZ.z ~= 0 then
						vectToBossXZ:normalize(1);
					end
					if vectToBalaXZ.x ~= 0 or vectToBalaXZ.y ~= 0 or vectToBalaXZ.z ~= 0 then
						vectToBalaXZ:normalize(1);
					end
					local dot = vectToBossXZ * vectToBalaXZ;
					dot = math.abs(dot);
					local sizeLargo = 0;
					local sizeCorto = 0;
					if platformSizeXZ.x > platformSizeXZ.z then
						sizeLargo = platformSizeXZ.x;
						sizeCorto = platformSizeXZ.z;
					else
						sizeLargo = platformSizeXZ.z;
						sizeCorto = platformSizeXZ.x;
					end
					local valorCorto = sizeCorto * dot;
					local valorLargo = sizeLargo * (1-dot);
					local distanceToCrashAlCuadrao = valorCorto * valorCorto + valorLargo * valorLargo;
					distanceToCrashAlCuadrao = distanceToCrashAlCuadrao + (enemy.m_ProjectileHitbox * enemy.m_ProjectileHitbox);
					local distanceToBalaAlCuadrao = distanceToBala * distanceToBala;
					if distanceToBalaAlCuadrao <= distanceToCrashAlCuadrao then
						choca = true;
					end
				end
			end
		end
	  if ((distance > 450 and enemy.m_PosicionBala.y < -1.2) or (distance <=450 and enemy.m_PosicionBala.y < 1.8)) or choca then
			enemy.m_IsOnCooldown = false;
			enemy.BalaSpeed = enemy.BalaOriginalSpeed;
			current_speed_change = inicial_speed_change;
			actual_speed_change = 0;
			enemy.BalaActiva = false;
			fire_boss_projectile_particles(renderable_shoot, enemy, true)
			delete_shooting(renderable_shoot)
			local qte_emmiter_name = playerRenderable.m_ParticleEmitter
			if not inputm:has_game_pad(1) then
				qte_emmiter_name = playerRenderable.m_ParticleEmitter2
			end
			local emitter = particle_manager:get_resource(qte_emmiter_name)
			if emitter:get_visible() == true then
				emitter:set_visible(false)
			end
			enemy:m_FSM():newState("Parado")
		end
    end
  end

end

function delete_shooting(mesh)
	--coreInstance:trace("Borrando disparo")
	if mesh.m_Printable then	
		mesh.m_Printable = false
		mesh:set_visible(false)
	end
end

function enemy_set_alive(name)
	local enemy =  enemy_manager:get_enemy(name)
	enemy:set_position(enemy.m_OriginalPosition)
	enemy.m_PhysicController:set_position(enemy.m_OriginalPosition)
	enemy.m_RenderableObject:set_position(enemy.m_OriginalPosition)
	--enemy:move_to_position(enemy.m_OriginalPosition)
	--enemy.m_PhysicController:set_radius(0.5)
	enemy.m_MovedToDiePosition = false
	enemy.m_IsDying = false
	--coreInstance:trace("Enemy "..name.." position: " ..tostring(enemy.m_PhysicController:get_position().x)..","..tostring(enemy.m_PhysicController:get_position().y)..","..tostring(enemy.m_PhysicController:get_position().z))
	--coreInstance:trace("Enemy "..name.. "position Renderable: " ..tostring(enemy.m_RenderableObject:get_position().x)..","..tostring(enemy.m_RenderableObject:get_position().y)..","..tostring(enemy.m_RenderableObject:get_position().z))
	enemy:m_FSM():newState("Parado")
end

function check_player_shoot_collision(enemy, mesh) 
	if (mesh.m_Printable and enemy.m_PosicionBala:distance(player_controller:get_position()) < enemy.m_ProjectileHitbox) then
		return true
	end
  return false
end

function check_player_shoot_return(enemy,mesh)
	local playerRenderable =  coreInstance:get_renderable_object_layer_manager():get_renderable_objects_manager_by_str_and_room(piky_layer, player_controller.m_Room):get_resource(piky_mesh_name);
	local qte_emmiter_name = playerRenderable.m_ParticleEmitter
	if not inputm:has_game_pad(1) then
		qte_emmiter_name = playerRenderable.m_ParticleEmitter2
	end
	local emitter = particle_manager:get_resource(qte_emmiter_name)
	local distance_to_bala = enemy.m_PosicionBala:distance(player_controller:get_position());
	if (mesh.m_Printable and distance_to_bala < enemy.m_ProjectileReturnDist) and boss_projectile_returned == false and current_shot_type == "rock" then
		if player.super_piky_active and player.pressed_return then
			player.execute_return_pr = true;
			boss_projectile_returned = true;
			boss_projectile_returned_by_chucky = false;
			local enemyPos = Vect3f(enemy:get_position().x, enemy:get_position().y + 2, enemy:get_position().z);
			enemy.m_DireccionBala = enemyPos - player_controller:get_position();
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
			emitter:set_visible(false)
		elseif player.super_piky_active and not player.pressed_return then
			local pikyYaw = enemy:get_yaw() + (math.pi / 2);
			local vectRight = Vect3f(math.cos(pikyYaw), 0, -math.sin(pikyYaw));
			local position = player_controller:get_position()
			local emitter_pos = Vect3f(position.x - (2 * vectRight.x), position.y, position.z + -(2 * vectRight.z))
			emitter.m_vPos =emitter_pos + playerRenderable.m_EmitterOffset
			emitter:set_visible(true)
			
		end
	elseif emitter:get_visible() == true then
			emitter:set_visible(false)
	end
end

function check_hitbox(ElapsedTime, player_position, enemy)
	
end

function show_or_hide(enemy, offset_position, ElapsedTime)
	--local direction = Vect3f(0, 1, 0)
	local temp_position = Vect3f(enemy:get_position().x, enemy:get_position().y,enemy:get_position().z)
	--coreInstance:trace(tostring(temp_position.x) .. ", ".. tostring(temp_position.y))
	if not enemy.m_Hide then
	--	direction.y = -1
		if enemy:get_position().y > enemy.m_OriginalPosition.y then
			-- mover enemigo
			enemy.m_Hiding = true
			temp_position.y = temp_position.y - 3* ElapsedTime	
		else
			enemy.m_Hide = true
			enemy.m_Hiding = false
		end
	else
		if enemy:get_position().y <  enemy.m_OriginalPosition.y + offset_position then
			-- mover enemigo
			temp_position.y = temp_position.y + 3* ElapsedTime
			enemy.m_Hiding = true
		else
			enemy.m_Hide = false
			enemy.m_Hiding = false
		end
	end
	enemy:set_position(temp_position)
	enemy.m_RenderableObject:set_position(temp_position)
end

function get_distance_to_player(current_position, _player_position)
	-- calcular distancia hacia player
	local distance = 0
	distance = ((_player_position.x - current_position.x) ^2 + (_player_position.y - current_position.y) ^2 + (_player_position.z - current_position.z) ^2)
	return distance
end