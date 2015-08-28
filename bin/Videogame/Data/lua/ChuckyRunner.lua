local coreInstance = CCoreLuaWrapper().m_CoreInstance;
local player = Player:get_instance();
local playerController = player.get_player_controller();
local distance = 4;
local chucky = enemy_manager:get_enemy("Chucky");
chucky.m_PhysicUserData.m_myCollisionGroup = 18;
local jumpStart = false;
local chuky_catching_distance = 7 -- 10 para que se vea toda la animacion pero piky le trolea cosa mala si avanza
local need_to_jump = false
-- Chucky Variables --
local Chucky_super_speed_dist = 10
local Chucky_running_speed = 14;
local Chucky_super_speed = 21;
local Chucky_current_speed = 14;
local chucky_jumped = false
chuky_last_room = 1

-- Chucky Stopped --
function chucky_runner_enter_stopped(name)
	coreInstance:trace("Chucky parado")
	if playerController == nil then
		playerController = player.get_player_controller()
	end
	if chucky == nil then
		chucky = enemy_manager:get_enemy(name)
		chucky.m_RenderableObject:blend_cycle(0,1,0);
	end
	local characterPos = chucky.m_PhysicController:get_position();
	characterPos.y = player_controller:get_position().y - 1 --0
	chucky.m_RenderableObject:set_position(characterPos);
	coreInstance:trace("Chuky position renderable: " ..tostring(characterPos.x)..","..tostring(characterPos.y)..","..tostring(characterPos.z))
end

function chucky_runner_exit_stopped(name)

end

function chucky_runner_update_stopped(ElapsedTime, doComprobation, name)

end

-- Chucky Running --

function chucky_runner_enter_running(name)
	chucky.m_RenderableObject:blend_cycle(1,1,0);
	chucky.m_PhysicController:use_gravity(true);
	local chucky_renderable = chucky.m_RenderableObject
	local emitter = particle_manager:get_resource(chucky_renderable.m_ParticleEmitter)
	if(emitter ~= nil) then
		emitter.m_vPos = chucky_renderable:get_position()+ chucky_renderable.m_EmitterOffset
		emitter:set_visible(true)
	end
	--local emitter2 = particle_manager:get_resource(chucky.m_RenderableObject.m_ParticleEmitter2)
	--emitter2:set_visible(false)
end

function chucky_runner_exit_running(name)
	chucky.m_RenderableObject:clear_cycle(1,0.25);
end

function move_chucky_running(ElapsedTime)
	local playerPos = playerController:get_position();
	local chuckyPos = chucky:get_position();
	local mov = playerPos - chucky:get_position();
	mov.y = 0;
	mov:normalize(1);
	chucky.m_PhysicController:move(mov * Chucky_current_speed * ElapsedTime, ElapsedTime);
	local pos = chucky:get_position();
	chucky:set_position(chucky.m_PhysicController:get_position());
	local chucky_renderable = chucky.m_RenderableObject
	chucky_renderable:set_position(Vect3f(pos.x, pos.y - 2, pos.z));
	
	local emitter = particle_manager:get_resource(chucky.m_RenderableObject.m_ParticleEmitter)
	emitter.m_vPos = chucky_renderable:get_position()+ chucky_renderable.m_EmitterOffset
	return math.sqrt( math.pow((playerPos.x - chuckyPos.x),2) + math.pow((playerPos.y - chuckyPos.y),2) + math.pow((playerPos.z - chuckyPos.z),2) );
end

function chucky_runner_update_running(ElapsedTime, doComprobation, name)
	 local distance =  move_chucky_running(ElapsedTime)
	
	--sqrt(pow((x - x2), 2) + pow((y - y2), 2) + pow((z - z2), 2));
	
	--local distance = math.sqrt( (playerPos.x - chuckyPos.x)*(playerPos.x - chuckyPos.x) + (playerPos.y - chuckyPos.y)*(playerPos.y - chuckyPos.y) )
	--coreInstance:trace("Pos------------------ "..tostring(distance))
	
	if distance > Chucky_super_speed_dist then
		Chucky_current_speed = Chucky_super_speed;
	else
		Chucky_current_speed = Chucky_running_speed;
	end
	
	--Chucky fucks Piky.
	if distance < chuky_catching_distance then
		chucky:m_FSM():newState("Atrapando")
	end
	

end

-- Chucky Jumping --

function chucky_runner_enter_jumping(name)
	if chucky.m_RenderableObject:is_action_animation_active() then
		chucky.m_RenderableObject:remove_action(3)
	end
	chucky.m_RenderableObject:execute_action(2,0.25,0,1,true);
	chucky.m_PhysicController:use_gravity(false);
	jumpStart = false;
	coreInstance:trace("Entro saltando")
	set_chucky_need_to_jump(false)
	local emitter = particle_manager:get_resource(chucky.m_RenderableObject.m_ParticleEmitter)
	emitter:set_visible(false)
end

function chucky_runner_exit_jumping(name)
	chucky.m_RenderableObject:remove_action(2);
end

function chucky_runner_update_jumping(ElapsedTime, doComprobation, name)
	-- Solucionando las misteriosas caidas de chucky
	local chucky_renderable = chucky.m_RenderableObject
	if jumpStart then
		if chucky_renderable:is_action_animation_active() then
			chucky_renderable:remove_action(3)
		end
		chucky_renderable:execute_action(2,0.25,0,1,true);
		chucky.m_PhysicController:use_gravity(false);
		jumpStart = false;
		coreInstance:trace("Entro saltando pero no estaba dentro ya?")
	end
	--coreInstance:trace("Update saltando chuky")
	if (not chucky_renderable:is_cycle_animation_active() and not jumpStart) then
		chucky_renderable:blend_cycle(1,1,0);
		local characterPos = chucky_renderable:get_position();
		local playerPos = playerController:get_position();
		local mov = playerPos - chucky:get_position();
		mov.y = 0;
		mov:normalize(1);
		mov = mov * (8.992 + 0.164);
		local newPos = Vect3f(characterPos.x + mov.x, characterPos.y, characterPos.z + mov.z);
		chucky.m_PhysicController:set_position(Vect3f(newPos.x, newPos.y + 2, newPos.z));
		chucky_renderable:set_position(Vect3f(newPos.x, newPos.y, newPos.z));
		jumpStart = true
		local emitter2 = particle_manager:get_resource(chucky_renderable.m_ParticleEmitter2)
		emitter2.m_vPos = chucky_renderable:get_position()+ chucky_renderable.m_EmitterOffset2
		emitter2.m_FireParticles = true
		coreInstance:trace("Salgo del saltooo")
		chucky:m_FSM():newState("Corriendo");
	end

	local playerPos = playerController:get_position();
	local chuckyPos = chucky:get_position();
	local distance = math.sqrt( math.pow((playerPos.x - chuckyPos.x),2) + math.pow((playerPos.y - chuckyPos.y),2) + math.pow((playerPos.z - chuckyPos.z),2) );
	--coreInstance:trace("Pos------------------ "..tostring(distance))
	if distance < 3.0 then
		player:player_die()
		coreInstance:trace("PIKY IS DEAD, AND YOU'RE THE NEXT")
		chucky:m_FSM():newState("Parado");
	end
	
end

-- Chucky Catching --

function chucky_runner_enter_catching(name)
	chucky.m_RenderableObject:execute_action(3,0.1,0,1,false);
end

function chucky_runner_exit_catching(name)
	chucky.m_RenderableObject:remove_action(3)
end

function chucky_runner_update_catching(ElapsedTime, doComprobation, name)

--chucky.m_RenderableObject:clear_cycle(1,0.25);
	local distance = move_chucky_running(ElapsedTime)
	--local distance = math.sqrt( math.pow((playerPos.x - chuckyPos.x),2) + math.pow((playerPos.y - chuckyPos.y),2) + math.pow((playerPos.z - chuckyPos.z),2) );
		
	if not chucky.m_RenderableObject:is_action_animation_active() then
		if distance < chuky_catching_distance then
			chucky.m_RenderableObject:remove_action(3)
			chucky.m_RenderableObject:execute_action(3,0.1,0,1,false);
		else
			chucky:m_FSM():newState("Corriendo");
			coreInstance:trace("Paro de pillar y corro")
		end
	end
	
	if distance < 2.0 then
		player:player_die()
		coreInstance:trace("PIKY IS DEAD, AND YOU'RE THE NEXT")
		chucky:m_FSM():newState("Parado");
	end	
	if need_to_jump then
		chucky:m_FSM():newState("Saltando");
	end

end

-- Chucky Falling down --
function chucky_runner_enter_falling_down(name)

end

function chucky_runner_exit_falling_down(name)

end

function chucky_runner_update_falling_down(ElapsedTime, doComprobation, name)

end

function get_chuky_need_to_jump()
	return need_to_jump
end

function set_chucky_need_to_jump(jumping_trigger)
	need_to_jump = jumping_trigger
end

function get_chuky_jumped()
	return chucky_jumped
end

function set_chuky_jumped(jumped)
	chucky_jumped = jumped
end