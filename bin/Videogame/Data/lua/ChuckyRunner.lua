local coreInstance = CCoreLuaWrapper().m_CoreInstance;
local player = Player:get_instance();
local playerController = player.get_player_controller();
local distance = 4;
local chucky = enemy_manager:get_enemy("Chucky");

-- Chucky Variables --
local Chucky_running_speed = 4;

-- Chucky Stopped --
function chucky_runner_enter_stopped(name)
	coreInstance:trace("======!!!!!!!!!!!!!===============!!!!!!!!!!!!!!!!!=================!!!!!!!!!!!");
	if playerController == nil then
		playerController = player.get_player_controller()
	end
	if chucky == nil then
		chucky = enemy_manager:get_enemy(name)
		chucky.m_RenderableObject:blend_cycle(0,1,0);
		coreInstance:trace("enemy name")
		coreInstance:trace(tostring(name))
		coreInstance:trace(tostring(chucky:get_name()))
		coreInstance:trace("========================")
	end
end

function chucky_runner_exit_stopped(name)

end

function chucky_runner_update_stopped(ElapsedTime, doComprobation, name)

end

-- Chucky Running --

function chucky_runner_enter_running(name)
	--local
	chucky.m_RenderableObject:blend_cycle(1,1,0);
end

function chucky_runner_exit_running(name)

end

function chucky_runner_update_running(ElapsedTime, doComprobation, name)
	local playerPos = playerController:get_position();
	--local mov = playerPos - chucky.get_player_controller():get_position();
	--chucky.get_player_controller():move(mov, ElapsedTime);
	--chucky.m_RenderableObject:set_position(chucky.get_player_controller():get_position());
end

-- Chucky Jumping --

function chucky_runner_enter_jumping(name)

end

function chucky_runner_exit_jumping(name)

end

function chucky_runner_update_jumping(ElapsedTime, doComprobation, name)

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