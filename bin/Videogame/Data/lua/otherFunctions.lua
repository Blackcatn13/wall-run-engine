local coreInstance = CCoreLuaWrapper().m_CoreInstance
local physx_manager = coreInstance:get_phisics_manager()

function activate_teleport(player_position)
	local player = coreInstance:get_player_controller()
	coreInstance:trace(player_position)
	local vec_array = split_str(player_position, "%s")
	local vec3f_array = Vect3f(tonumber(vec_array[1]), tonumber(vec_array[2]), tonumber(vec_array[3]))
	player.m_PhysicController:set_position(vec3f_array)
	local camera = coreInstance.m_CameraController:get_active_camera()
	--	coreInstance:trace("laaaaaa")
	camera.m_pObject3D:set_position(Vect3f(camera.m_pObject3D:get_position().x, camera.m_pObject3D:get_position().y, vec3f_array.z - 10))

	--camera.m_pObject3D:set_position(Vect3f(3.118870, 20.0, 271.008423))
end
function set_is_3D()
	local player = coreInstance:get_player_controller();
	player.m_is3D = true;
	coreInstance.m_CameraController:set_active_camera("3DCam");

end

function roll_object(objectName, layer_name, dt)
	local object_manager = coreInstance:get_renderable_object_layer_manager():get_renderable_objects_manager_by_str(layer_name)
	local object = object_manager:get_resource(objectName)
	local new_yaw = object:get_yaw()+ 1* dt
	object:set_yaw(new_yaw)
end


function unlock_image(image_name)
	for i = 1, table.maxn(Cards_Vector) do
		if Cards_Vector[i]:get_name() == image_name then
			Cards_Vector[i]:set_unlocked(true)
			coreInstance:trace("Image ".. image_name .."Unclocked!!")
			return 0
		end
	end
	coreInstance:trace("Image Not found")
end

function toogle_switch(trigger_name, puzzle_name)
	coreInstance:trace("Enter switch")
	local player = Player:get_instance()
	coreInstance:trace(tostring(player.get_controller().m_isJumping))
	local puzzle = coreInstance:get_puzzle_manager():get_resource(puzzle_name)
	--local total_switches = puzzle.m_MinActivatedSwitches
	
	if player ~= nil then
		local player_controller = player.get_controller()
		--if player_controller.m_isAttack == true or player_controller.m_isJumping == true then
			local trigger_manager = coreInstance:get_trigger_manager()
			local trigger = trigger_manager:get_resource(trigger_name)
			--coreInstance:trace("Toogle switch")
			if trigger ~= nil then		
				local switch_mesh = get_renderable_object("puzzle", trigger_name)
				if trigger.m_IsSwitched ~= true then
					trigger.m_IsSwitched = true
				--	coreInstance:trace("Trigger activado?" ..tostring(trigger.m_IsSwitched))
					puzzle.m_ActivatedSwitches = puzzle.m_ActivatedSwitches + 1	
					local new_position = Vect3f(switch_mesh:get_position().x,switch_mesh:get_position().y - 0.3,switch_mesh:get_position().z )
					change_object_position(switch_mesh, new_position)
				--[[else 
					trigger.m_IsSwitched = false
					coreInstance:trace(tostring(trigger.m_IsSwitched))
					puzzle.m_ActivatedSwitches = puzzle.m_ActivatedSwitches - 1
					local new_position = Vect3f(switch_mesh:get_position().x,switch_mesh:get_position().y + 0.3,switch_mesh:get_position().z )
					change_object_position(switch_mesh, new_position)
					--]]
				end
			end
		--end
	end
	--coreInstance:trace(tostring(puzzle.m_ActivatedSwitches))
	--if activated_switches == total_switches then --TODO obtener datos del puzzle en si
		--coreInstance:trace("all switches activated")
		--open_door()
	--end
end
function change_object_position(_object, _position)
	--local switch_mesh = get_renderable_object("switches", trigger_name)
	--local new_position = Vect3f(switch_mesh:get_position().x,switch_mesh:get_position().y - 0.3,switch_mesh:get_position().z )
	_object:set_position(_position)
	_object.m_Actor:set_global_position(_position)
end

function get_renderable_object(layer_name, _objectName)
	local object_manager = coreInstance:get_renderable_object_layer_manager():get_renderable_objects_manager_by_str(layer_name)
	local object = object_manager:get_resource(_objectName)
	return object
end

function open_door(_objectName)
	coreInstance:trace("Opening door")
	local door = get_renderable_object("puzzle", _objectName)
	--door.m_IsOpening =true
	--play animacion subir puerta
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

function get_pixelite(pixelite_name)
	local trigger_name = pixelite_name .. "_UserData"
	local trigger = coreInstance:get_trigger_manager():get_resource(trigger_name)
	if trigger.m_IsSwitched == false then
		Player:get_instance().add_pixelites(1)
		deactivate_collectible(trigger,"collectible", pixelite_name)
	end
end

function get_sticker(sticker_name, img_name)
	local trigger_name = sticker_name .. "_UserData"
	local trigger = coreInstance:get_trigger_manager():get_resource(trigger_name)
	if trigger.m_IsSwitched == false then
		Player:get_instance().add_sticker()
		deactivate_collectible(trigger,"collectible", sticker_name)
	end
end


function deactivate_collectible (_trigger, layer_name, obj_name)
	_trigger.m_IsSwitched = true
	local object_manager = coreInstance:get_renderable_object_layer_manager():get_renderable_objects_manager_by_str(layer_name)
	local object = object_manager:get_resource(obj_name)
	object.m_Printable = false
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

function clear_array(Array_Name)
	for i = 1, table.maxn(Array_Name) do
		table.remove(Array_Name, i)
	end
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