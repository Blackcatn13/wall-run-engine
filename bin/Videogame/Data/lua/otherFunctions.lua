local coreInstance = CCoreLuaWrapper().m_CoreInstance
local physx_manager = coreInstance:get_phisics_manager()

function activate_teleport(player_position)
	local player = coreInstance:get_player_controller()
	coreInstance:trace(player_position)
	local vec_array = split_str(player_position, "%s")
	local vec3f_array = Vect3f(tonumber(vec_array[1]), tonumber(vec_array[2]), tonumber(vec_array[3]))
	player.m_PhysicController:set_position(vec3f_array)
	local camera = coreInstance.m_CameraController:get_active_camera()
		coreInstance:trace("laaaaaa")
	camera.m_pObject3D:set_position(Vect3f(camera.m_pObject3D:get_position().x, camera.m_pObject3D:get_position().y, vec3f_array.z - 10))

	--camera.m_pObject3D:set_position(Vect3f(3.118870, 20.0, 271.008423))
end
function set_is_3D()
	local player = coreInstance:get_player_controller();
	player.m_is3D = true;
	coreInstance.m_CameraController:set_active_camera("3DCam");

end

function trigger_change_view(offset_axis)
	local player = coreInstance:get_player_controller();
	if player.m_is3D == true then 
		player.m_is3D = false;
		local cam = coreInstance.m_CameraController:get_resource("3DCam");
		cam.m_eTypeCamera = 5;
		coreInstance.m_CameraController:set_active_camera("3DCam");
		if player.m_isTurned == true then
			player.m_isTurned = false
		end
		
		--player:set_yaw(coreInstance.m_CameraController:get_active_camera().m_pObject3D:get_yaw())
		--coreInstance:trace(tostring(player:get_yaw()))
		local camera_object = coreInstance.m_CameraController:get_active_camera().m_pObject3D
		local pos_x =  player:get_position().x
		local pos_z =  player:get_position().z
		if offset_axis == "x" then
			pos_x = camera_object:get_position().x
		elseif offset_axis == "z" then
			pos_z = camera_object:get_position().z
		else
			pos_x = camera_object:get_position().x
			pos_z = camera_object:get_position().z
		end
		
		local position_2d = Vect3f(pos_x, player:get_position().y ,pos_z)
		coreInstance:trace(tostring(player:get_position().z))
		
		player.m_PhysicController:set_position(position_2d)
		move_character_controller_mesh(player, position_2d);
		coreInstance:trace(tostring(position_2d.z))
	else
		player.m_is3D = true;
		local cam = coreInstance.m_CameraController:get_resource("3DCam");
		cam.m_eTypeCamera = 6;
		coreInstance.m_CameraController:set_active_camera("3DCam");
	end	
end

function trigger_set_3D()
	local player = coreInstance:get_player_controller();
	player.m_is3D = true;
	coreInstance.m_CameraController:set_active_camera("ThPS");

end

function activate_trigger_update(trigger_name)
	local coreInstance = CCoreLuaWrapper().m_CoreInstance;
	local trigger_manager = coreInstance:get_trigger_manager()
	local trigger = trigger_manager:get_resource(trigger_name)
	trigger.m_Update=true

end 

function set_player_respawn_spikes_point()
	local player = Player:get_instance()
	player.last_spikes_position = Vect3f(player.get_player_controller():get_position())
end

function player_die()
	local coreInstance = CCoreLuaWrapper().m_CoreInstance;
	coreInstance:trace("dieeeeee")
	local player = Player:get_instance()
	player.player_die()
end

function set_checkpoint(trigger_name)
	local coreInstance = CCoreLuaWrapper().m_CoreInstance;
		coreInstance:trace("enter checkpoint")
	local player = Player:get_instance()
	local current_position = Vect3f(player.get_player_controller():get_position())
	coreInstance:trace("Player Position set")
	local checkpoint = CheckPoint.new()
	coreInstance:trace("Checkpoint created")
	--coreInstance:trace(tostring(checkpoint:is_activated()))
	--if checkpoint:is_activated() == false then
	if player.check_visited_checkpoints(trigger_name) == false then
		checkpoint.set_checkpoint(player, trigger_name)
	end
	coreInstance:trace(tostring(checkpoint.is_activated))
	coreInstance:trace("checkpoint activated")
--	end

end

function get_distance_between_points(current_position, _player_position)
	-- calcular distancia hacia player
	local distance = 0
	distance = ((_player_position.x - current_position.x) ^2 + (_player_position.y - current_position.y) ^2 + (_player_position.z - current_position.z) ^2)
	return distance
end

function split_str(inputstr, sep)
        if sep == nil then
                sep = "%s"
        end
        local t={} ; i=1
        for str in string.gmatch(inputstr, "([^"..sep.."]+)") do
				--coreInstance:trace(str)
                t[i] = str
                i = i + 1
        end
        return t
end