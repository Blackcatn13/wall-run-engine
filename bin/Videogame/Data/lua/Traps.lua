

function activate_wall_traps(room_name, name)
	local trap = get_renderable_object(room_name, name)
	trap.m_Activated = true
end

function wt_enter_stopped(name)
	local trap = get_renderable_object(room_name, name)
	trap.m_CurrentTime = 0.0
	reset_crushing_sides()
end

function wt_exit_stopped()
	--coreInstance:trace("Saliendo Platform stopped");

end

function wt_update_stopped(ElapsedTime, name)
	local trap = get_renderable_object(room_name, name)
	if trap.m_CurrentTime > trap.m_TimeOut then
		instance.m_string = "ClosingTrap"
	else
		trap.m_CurrentTime = trap.m_CurrentTime +1 * ElapsedTime
	end	

end

------------CLOSING------------
function wt_enter_closing(name)


end

function wt_exit_closing()
	reset_crushing_sides() 

end

function wt_update_closing(ElapsedTime, name)
	local trap = get_renderable_object(room_name, name)
	trap:move_to_point(ElapsedTime, trap.m_FinalPosition, 0.3, trap.m_SpeedClosing)
	if trap:is_around(trap:get_position(), player_controller:get_position()) then
		check_player_impact_side(trap.m_Side)
	end
	
	if get_distance_between_points(trap:get_position(), trap.m_FinalPosition) < 2 then
		instance.m_string = "OpeningTrap"
	end
end

-------------OPENING------------------------
function wt_enter_opening(name)


end

function wt_exit_opening()
	--coreInstance:trace("Saliendo Platform stopped");

end

function wt_update_opening(ElapsedTime, name)
	local trap = get_renderable_object(room_name, name)
	trap:move_to_point(ElapsedTime, trap.m_OriginalPosition, 0.3, trap.m_SpeedOpening)

	if get_distance_between_points(trap.m_OriginalPosition, trap:get_position()) < 2 then
		instance.m_string = "Parado"
	end
	
end

function check_player_impact_side(impact_side)
	if impact_side == "right" then
		player.crush_right = true
	else if impact_side == "left" then
		player.crush_left = true
	else if impact_side == "up" then
		player.crush_up = true
	else
		player.crush_down = true
	end

	if (player.crush_right and player.crush_left) or (player.crush_down and player.crush_up) then
		reset_crushing_sides()
		player.die()
	end
	
end

function reset_crushing_sides()
	player.crush_down = false
	player.crush_up = false
	player.crush_left = false
	player.crush_right = false
end