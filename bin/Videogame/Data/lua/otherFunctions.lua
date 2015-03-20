local coreInstance = CCoreLuaWrapper().m_CoreInstance
local physx_manager = coreInstance:get_phisics_manager()

function activate_teleport(player_position)
	local player = coreInstance:get_player_controller()
	core:trace(player_position)
	local vec_array = split_str(player_position, "%s")
	local vec3f_array = Vect3f(tonumber(vec_array[1]), tonumber(vec_array[2]), tonumber(vec_array[3]))
	player.m_PhysicController:set_position(vec3f_array)
	local camera = coreInstance.m_CameraController:get_active_camera()
		
	camera.m_pObject3D:set_position(Vect3f(camera.m_pObject3D:get_position().x, camera.m_pObject3D:get_position().y, vec3f_array.z - 10))
	--camera.m_pObject3D:set_position(Vect3f(3.118870, 20.0, 271.008423))
	
	
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

function set_checkpoint()
	local coreInstance = CCoreLuaWrapper().m_CoreInstance;
		coreInstance:trace("enter checkpoint")
	local player = Player:get_instance()
	local current_position = Vect3f(player.get_player_controller():get_position())
	coreInstance:trace("Player Position set")
	local checkpoint = CheckPoint.new()
	coreInstance:trace("Checkpoint created")
	--coreInstance:trace(tostring(checkpoint:is_activated()))
	--if checkpoint:is_activated() == false then
	checkpoint.set_checkpoint(player)
	coreInstance:trace(tostring(checkpoint.is_activated))
	coreInstance:trace("checkpoint activated")
--	end

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