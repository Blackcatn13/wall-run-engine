local coreInstance = CCoreLuaWrapper().m_CoreInstance;
local player = Player:get_instance();
local playerController = player.get_player_controller();
local distance = 4;
local chucky = enemy_manager:get_enemy("Chucky");
chucky.m_PhysicUserData.m_myCollisionGroup = 18;
local jumpStart = false;

-- Chucky Variables --
local Chucky_running_speed = 4;

-- Chucky Stopped --
function chucky_runner_enter_stopped(name)
	if playerController == nil then
		playerController = player.get_player_controller()
	end
	if chucky == nil then
		chucky = enemy_manager:get_enemy(name)
		chucky.m_RenderableObject:blend_cycle(0,1,0);
	end
	local characterPos = chucky.m_PhysicController:get_position();
	characterPos.y = 0;
	chucky.m_RenderableObject:set_position(characterPos);
end

function chucky_runner_exit_stopped(name)

end

function chucky_runner_update_stopped(ElapsedTime, doComprobation, name)

end

-- Chucky Running --

function chucky_runner_enter_running(name)
	coreInstance:trace("Enter running");
	chucky.m_RenderableObject:blend_cycle(1,1,0.2);
	chucky.m_RenderableObject:remove_action(2);
	chucky.m_PhysicController:use_gravity(true);
end

function chucky_runner_exit_running(name)
	coreInstance:trace("Exit running");
	chucky:set_position(chucky.m_PhysicController:get_position());
	local characterPos = chucky.m_PhysicController:get_position();
	characterPos.y = 0;
	chucky.m_RenderableObject:set_position(characterPos);
end

function chucky_runner_update_running(ElapsedTime, doComprobation, name)
	--chucky.m_RenderableObject:updateSkeleton(ElapsedTime);
	local playerPos = playerController:get_position();
	--coreInstance:trace("Player position x:" .. tostring(playerPos.x)  .. " y:" .. tostring(playerPos.y) ..  " z:" ..  tostring(playerPos.z));
	local chuckyPos = chucky:get_position();
	--coreInstance:trace("Chucky position x:" .. tostring(chuckyPos.x)  .. " y:" .. tostring(chuckyPos.y) ..  " z:" ..  tostring(chuckyPos.z));
	local mov = playerPos - chucky:get_position();
	mov.y = 0;
	--coreInstance:trace("mov x:" .. tostring(mov.x)  .. " y:" .. tostring(mov.y) ..  " z:" ..  tostring(mov.z));
	chucky.m_PhysicController:move(mov * ElapsedTime, ElapsedTime);
	chucky:set_position(chucky.m_PhysicController:get_position());
	local characterPos = chucky.m_PhysicController:get_position();
	characterPos.y = characterPos.y - 2;
	chucky.m_RenderableObject:set_position(characterPos);

end

-- Chucky Jumping --

function chucky_runner_enter_jumping(name)
	chucky.m_RenderableObject:clear_cycle(1,0);
	chucky.m_RenderableObject:execute_action(2,0,0.3,1,false);
	chucky.m_PhysicController:use_gravity(false);
	coreInstance:trace("Enter jumping" );
	jumpStart = false;
end

function chucky_runner_exit_jumping(name)
	chucky.m_RenderableObject:restartBonesPosition();
	coreInstance:trace("Exit jumping");
end

function chucky_runner_update_jumping(ElapsedTime, doComprobation, name)
	chucky.m_RenderableObject:updateSkeleton(ElapsedTime);
	local Topos = chucky.m_RenderableObject:getBonesMovement();
	Topos = Topos:rotate_y(1.7);
	coreInstance:trace("Position x:" .. tostring(Topos.x) .. " y:" .. tostring(Topos.y) .. " z:" .. tostring(Topos.z));
	chucky.m_PhysicController:move(Topos, ElapsedTime);
	chucky:set_position(chucky.m_PhysicController:get_position())
	coreInstance:trace("Jump has started? " .. tostring(jumpStart));
	if Topos.y == 0 and jumpStart == true then
		coreInstance:trace("Changing to runing");
		chucky:m_FSM():newState("Corriendo");
	end
	if doComprobation == 1 and jumpStart == false then
		local info = SCollisionInfo();
		local _dirRay = Vect3f(0,-1,0);
		coreInstance:get_phisics_manager():raycast_closest_actor(chucky:get_position() - 0.4, _dirRay, 1, info, 1000);
		_fallPosition = info.m_CollisionPoint;
		local fallPos = chucky:get_position();
		local distance = (fallPos.x - _fallPosition.x) * (fallPos.x - _fallPosition.x) + (fallPos.y - _fallPosition.y) * (fallPos.y - _fallPosition.y) + (fallPos.z - _fallPosition.z) * (fallPos.z - _fallPosition.z);
		coreInstance:trace(tostring(distance));
		if distance <= 2 then
			coreInstance:trace("Jump stated");
			jumpStart = true;
		end
	end
end

-- Chucky Catching --

function chucky_runner_enter_catching(name)

end

function chucky_runner_exit_catching(name)

end

function chucky_runner_update_catching(ElapsedTime, doComprobation, name)

end

-- Chucky Falling down --
function chucky_runner_enter_falling_down(name)

end

function chucky_runner_exit_falling_down(name)

end

function chucky_runner_update_falling_down(ElapsedTime, doComprobation, name)

end