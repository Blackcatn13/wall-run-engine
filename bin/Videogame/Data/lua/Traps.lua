

function activate_wall_trap(room_name, name)
	local trap = get_renderable_object("solid",room_name, name)
	trap.m_Activated = true
end

function wt_enter_initial(room_name, name)
	local trap = get_renderable_object("solid", room_name, name)
	trap.m_CurrentTime = 0.0
	--if trap:get_position() ~= trap.m_OriginalPosition then
	trap:set_position(trap.m_OriginalPosition)
	trap.m_Actor:set_global_position(trap.m_OriginalPosition)
	--end
	reset_crushing_sides()
end

function wt_exit_initial()
	--coreInstance:trace("Saliendo Platform stopped");

end

function wt_update_initial(ElapsedTime, room_name, name)
	local trap = get_renderable_object("solid",room_name, name)
	if trap.m_Activated then
		trap:get_fsm():newState("ClosingTrap")
		trap.m_CurrentTime = 0.0
	else
		if trap:get_position() ~= trap.m_OriginalPosition then
			trap:set_position(trap.m_OriginalPosition)
			trap.m_Actor:set_global_position(trap.m_OriginalPosition)
		end
		
		if trap.m_CurrentTime > 0.0 then
			trap.m_CurrentTime = 0.0
		end
	end
end


--PARADO---
function wt_enter_stopped(room_name, name)
	local trap = get_renderable_object("solid", room_name, name)
	trap.m_CurrentTime = 0.0
	--if trap:get_position() ~= trap.m_OriginalPosition then
		trap:set_position(trap.m_OriginalPosition)
		trap.m_Actor:set_global_position(trap.m_OriginalPosition)
	--end
	reset_crushing_sides()
end

function wt_exit_stopped()
	--coreInstance:trace("Saliendo Platform stopped");

end

function wt_update_stopped(ElapsedTime, room_name, name)
	local trap = get_renderable_object("solid",room_name, name)
	if trap.m_Activated then
		if trap.m_CurrentTime > trap.m_TimeOut then
			--instance.m_string = "ClosingTrap"
			trap:get_fsm():newState("ClosingTrap")
			trap.m_CurrentTime = 0.0
		else
			trap.m_CurrentTime = trap.m_CurrentTime +1 * ElapsedTime
			--coreInstance:trace("Current Time " .. name ..": "..tostring(trap.m_CurrentTime))
		end	
	else
		--[[if trap:get_position() ~= trap.m_OriginalPosition then
			trap:set_position(trap.m_OriginalPosition)
			trap.m_Actor:set_global_position(trap.m_OriginalPosition)
			coreInstance:trace("Position ".. name .. tostring(trap:get_position().z))
		end
		
		if trap.m_CurrentTime > 0.0 then
			trap.m_CurrentTime = 0.0
		end--]]
		trap:get_fsm():newState("Initial")
	end
end

------------CLOSING------------
function wt_enter_closing(room_name, name)


end

function wt_exit_closing()
	reset_crushing_sides() 

end

function wt_update_closing(ElapsedTime, room_name, name)
	local trap = get_renderable_object("solid",room_name, name)
	if trap.m_Activated then
		trap:move_to_point(ElapsedTime, trap.m_FinalPosition, 0.1, trap.m_SpeedClosing)
		if trap:is_around(trap:get_position(), player_controller:get_position()) then
			check_player_impact_side(trap.m_Side)
		end
		--if trap:is_inside(trap:get_position(), player_controller:get_position()) then
			--player.player_die()
		--end
		
		if get_distance_between_points(trap:get_position(), trap.m_FinalPosition) < 1 then
			--instance.m_string = "OpeningTrap"
			trap:get_fsm():newState("OpeningTrap")
		end
	else
		trap:get_fsm():newState("Initial")
	end
end

-------------OPENING------------------------
function wt_enter_opening(room_name, name)


end

function wt_exit_opening()
	--coreInstance:trace("Saliendo Platform stopped");

end

function wt_update_opening(ElapsedTime, room_name, name)
	local trap = get_renderable_object("solid",room_name, name)
	if trap.m_Activated then
		trap:move_to_point(ElapsedTime, trap.m_OriginalPosition, 0.0, trap.m_SpeedOpening)

		if get_distance_between_points(trap.m_OriginalPosition, trap:get_position()) < 0.1 then
			--instance.m_string = "Parado"
			trap:get_fsm():newState("Parado")
		end
	else
		trap:get_fsm():newState("Initial")
	end
	
end

function check_player_impact_side(impact_side)
	if impact_side == "right" then
		player.crush_right = true
	elseif impact_side == "left" then
		player.crush_left = true
	elseif impact_side == "up" then
		player.crush_up = true
	else
		player.crush_down = true
	end

	if (player.crush_right and player.crush_left) or (player.crush_down and player.crush_up) then
		reset_crushing_sides()
		player.player_die()
	end
	
end

function reset_crushing_sides()
	player.crush_down = false
	player.crush_up = false
	player.crush_left = false
	player.crush_right = false
end

function reset_wall_trap(room_name, name)
	local trap = get_renderable_object("solid",room_name, name)
	if trap ~= nil then
		--trap:set_position(trap.m_OriginalPosition)
		--trap.m_Actor:set_global_position(trap.m_OriginalPosition)
		trap.m_Activated = false
		--trap.m_CurrentTime = 0.0
		trap:get_fsm():newState("Initial")
	end
end