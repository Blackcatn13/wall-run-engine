local boss_timer = 0.0
local contador = 0
local executedReturn = false;
local execute_hurt = false;
local doing_hurt = false;

function start_boss()
	set_player_room("0", true)
	cam_Controller:set_active_camera("BossCam")
	local activeCam = cam_Controller:get_active_camera()
	
	local position = Vect3f(30,1,2)
	player_controller.m_PhysicController:set_position(position)
	local playerRenderableMesh = renderable_objects_layer_manager:get_renderable_objects_manager_by_str_and_room("player", 0):get_resource(piky_mesh_name);
	playerRenderableMesh:set_position(position)
	
	local chucky = enemy_manager:get_enemy(boss_mesh_name)
	local chucky_position = Vect3f(0.0, 4.48, 0.0)
	--chucky:set_position(chucky_position)
	chucky.m_RenderableObject:set_position(chucky_position)
	chucky.m_RenderableObject:set_visible(true)
	local objCam = activeCam.m_pObject3D;
	objCam:set_position(chucky_position);
	local cadira = get_renderable_object("solid",0, "CADIRA")
	get_renderable_object("solid",0, "CADIRA")
	get_renderable_object("solid",0, "ORO2"):set_visible(false)
	get_renderable_object("solid",0, "ORO3"):set_visible(false)
	get_renderable_object("solid",0, "PilarQuad001"):set_visible(false)
	get_renderable_object("solid",0, "PilarQuad002"):set_visible(false)
	get_renderable_object("solid",0, "ChukyBossPosition"):set_visible(true)
	chucky:m_FSM():newState("Parado")
	cadira:set_visible(false)
	set_boss_polis_visible(true)
	all_boss_miks_killed = true
	boss_miks_killed = 0
	chucky.m_BossRunning = true
	
	set_checkpoint("boss_checkpoint", nil)
	player.attack_enabled = true
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
	local renderable = get_renderable_object("player", player_controller.m_Room, piky_mesh_name)
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
	enemy.m_RenderableObject:blend_cycle(0,1,0);
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
	if boss_timer >= enemy.m_CooldownTimer then
		local check_call_miks = false 
		if enemy.m_Phases == 1 and all_boss_miks_killed then
			check_call_miks = check_random_action (6)
		end
		
		if check_call_miks then
			enemy:m_FSM():newState("Llamar")
		else
			enemy:m_FSM():newState("Lanzar")
		end
	end
	
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
		if not executedReturn and get_distance_between_points(enemy:get_position(), enemy.m_PosicionBala) < 5 then
			boss_projectile_returned = false
			boss_projectile_returned_by_chucky = true
			local enemyPos = Vect3f(enemy:get_position().x, enemy:get_position().y + 2, enemy:get_position().z);
			enemy.m_DireccionBala = player_controller:get_position() - enemyPos;
			executedReturn = true;
		end
		if boss_projectile_returned or boss_projectile_returned_by_chucky then
			update_horizontal_boss_shoot(ElapsedTime, enemy)
		else
			update_shoot_boss(ElapsedTime, enemy)
		end
		if not enemy.m_RenderableObject:is_action_animation_active() then
			enemy:m_FSM():newState("EsperandoImpacto")
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
			if check_random_action(5) then
				enemy:m_FSM():newState("Devolver")
			else
				enemy:m_FSM():newState("Hurt")
				coreInstance:trace("HUUUUUUURT");
			end
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
		enemy.m_RenderableObject:execute_action(7,0.1,0,1,false)
	end
end

function chucky_boss_exit_call(name)
	
end

function chucky_boss_update_call(ElapsedTime, doComprobation, name)
	local enemy = enemy_manager:get_enemy(name)
	if (enemy ~= nil) then
		if not enemy.m_RenderableObject:is_action_animation_active() then
			local array_mik = {"MikMik007", "MikMik007", "MikMik008", "MikMik009", "MikMik010", "MikMik011"}
			for i = 1, table.getn(array_mik) do
				summon_mik(array_mik[i])
				fire_mik_summon_particles(array_mik[i])
			end
			all_boss_miks_killed = false
			boss_miks_killed = 0
			enemy:m_FSM():newState("Parado")
		end
	end
end

----HURT----
function chucky_boss_enter_hurt(name)
	doing_hurt = false
	execute_hurt = false
end

function chucky_boss_exit_hurt(name)
	local enemy = enemy_manager:get_enemy(name);
	enemy.m_RenderableObject:remove_action(6);
end

function chucky_boss_update_hurt(ElapsedTime, doComprobation, name)
	local enemy = enemy_manager:get_enemy(name)
	if (enemy ~= nil) then
		if not doing_hurt then
			update_horizontal_boss_shoot(ElapsedTime, enemy)
			local mesh = get_renderable_object("enemies", enemy.m_RenderableObject.m_Room, enemy.m_ProjectileName)
			local distance_to_bala = get_distance_between_points(enemy:get_position(), enemy.m_PosicionBala);
			--coreInstance:trace("distance to bala: "..tostring(distance_to_bala))
			if distance_to_bala < 7 then
				coreInstance:trace("colisionaaaaaa")
				execute_hurt = true
				boss_projectile_returned = false
				boss_projectile_returned_by_chucky = false;
				enemy.m_IsOnCooldown = false;
				delete_shooting(mesh)
			end
		end
		if execute_hurt then
			execute_hurt = false
			enemy.m_Life = enemy.m_Life -1
			coreInstance:trace("vidas: "..tostring(enemy.m_Life))
			local animation_hurt = true
			if enemy.m_Life <=0 then
				enemy.m_Phases = enemy.m_Phases -1
				enemy.m_Life = 3;
				if enemy.m_Phases <= 0 then
					enemy:m_FSM():newState("Dead")
					animation_hurt = false;
				end
			end
			if animation_hurt then
				coreInstance:trace("doing animation hurt")
				doing_hurt = true
				enemy.m_RenderableObject:clear_cycle(0,0.3);
				enemy.m_RenderableObject:execute_action(6,0.1,0,1,false)
			end
		end
		if doing_hurt and not enemy.m_RenderableObject:is_action_animation_active() then
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
	
end

function chucky_boss_update_dead(ElapsedTime, doComprobation, name)
	local enemy = enemy_manager:get_enemy(name)
	if (enemy ~= nil) then
		if not enemy.m_RenderableObject:is_action_animation_active() then
			coreInstance:trace("YAAAAY WALL DONE!!")
			-- Llamar al fin de demo /cinematica final
		end
	end
end
