--local coreInstance = CCoreLuaWrapper().m_CoreInstance
--local physx_manager = coreInstance:get_phisics_manager()

function change_object_position(_object, _position)
	--local switch_mesh = get_renderable_object("switches", trigger_name)
	--local new_position = Vect3f(switch_mesh:get_position().x,switch_mesh:get_position().y - 0.3,switch_mesh:get_position().z )
	_object:set_position(_position)
	_object.m_Actor:set_global_position(_position)
end

function get_renderable_object(layer_name, _objectName)
	local object_manager = renderable_objects_layer_manager:get_renderable_objects_manager_by_str(layer_name)
	local object = object_manager:get_resource(_objectName)
	return object
end

function open_door(_objectName)
	coreInstance:trace("Opening door ".. _objectName)
	local door = get_renderable_object("puzzle", _objectName)
	if door ~= nil then
		door.m_Printable = false
		door.m_Actor:activate(false)
	end
	--door.m_IsOpening =true
	--play animacion subir puerta
end

function get_distance_between_points(current_position, _player_position)
	-- calcular distancia hacia player
	local distance = 0
	distance = ((_player_position.x - current_position.x) ^2 + (_player_position.y - current_position.y) ^2 + (_player_position.z - current_position.z) ^2)
	return distance
end

function clear_array(Array_Name)
	for i = 1, table.getn(Array_Name) do
		table.remove(Array_Name)
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

function reset_game()
	--/////////////////////////////////////////////////////////////////////////////////////
	--PLAYER
	--/////////////////////////////////////////////////////////////////////////////////////
	
	--Position + 3D
	player_controller:set_position(Vect3f(60,5,0));
	player_controller.m_PhysicController:set_position(Vect3f(60,5,0));
	player_controller.m_is3D = true;
	local cam = coreInstance.m_CameraController:get_resource("3DCam");
	cam.m_eTypeCamera = 6;
	coreInstance.m_CameraController:set_active_camera("3DCam");
	player_controller.m_isTurned = false;
	
	--Lifes + Pixelites
	player_controller.num_hearts = 3;
	gui_manager:set_image('LifeGUI','Life3')
	player_controller.num_lives = 3;
	gui_manager:set_image('VidesGUI','Vides3')
	gui_manager:set_is_displayed_heart(true);
	gui_manager:set_count_heart(0.0);
	player_controller.pixelites = 0;
	gui_manager:set_is_displayed_pixelite(true);
	gui_manager:set_count_pixelite(0.0);
	gui_manager:set_num_pixelite( 0 );
	
	--/////////////////////////////////////////////////////////////////////////////////////
	--COLLECTIBLES
	--/////////////////////////////////////////////////////////////////////////////////////
	
	--Pixelites
	activate_collectible(trigger_manager:get_resource("Pixelite001_UserData"), "collectible" ,"Pixelite001");
	activate_collectible(trigger_manager:get_resource("Pixelite002_UserData"), "collectible" ,"Pixelite002");
	activate_collectible(trigger_manager:get_resource("Pixelite003_UserData"), "collectible" ,"Pixelite003");
	activate_collectible(trigger_manager:get_resource("Pixelite004_UserData"), "collectible" ,"Pixelite004");
	activate_collectible(trigger_manager:get_resource("Pixelite005_UserData"), "collectible" ,"Pixelite005");
	activate_collectible(trigger_manager:get_resource("Pixelite006_UserData"), "collectible" ,"Pixelite006");
	activate_collectible(trigger_manager:get_resource("Pixelite007_UserData"), "collectible" ,"Pixelite007");
	activate_collectible(trigger_manager:get_resource("Pixelite008_UserData"), "collectible" ,"Pixelite008");
	activate_collectible(trigger_manager:get_resource("Pixelite009_UserData"), "collectible" ,"Pixelite009");
	activate_collectible(trigger_manager:get_resource("Pixelite010_UserData"), "collectible" ,"Pixelite010");
	activate_collectible(trigger_manager:get_resource("Pixelite011_UserData"), "collectible" ,"Pixelite011");
	activate_collectible(trigger_manager:get_resource("Collectible1_UserData"), "collectible" ,"Collectible1");
	
	--/////////////////////////////////////////////////////////////////////////////////////
	--ENEMIES
	--/////////////////////////////////////////////////////////////////////////////////////
	
	local enemies = enemy_manager:get_enemies();
	local num = 1
	for num = 1, #enemies do
		if enemies[num].m_isAlive == false then
			enemies[num].m_isAlive = true;
			enemies[num].m_RenderableObject.m_Printable = true;
			--local dead_pos = enemies[num].m_PhysicController:get_position()
			--dead_pos.y = dead_pos.y - 1000
			--enemies[num]:set_position(dead_pos)
			--enemies[num].m_PhysicController:set_position(dead_pos)
			--enemies[num]:move_to_position(dead_pos)
		end
	end
	player_controller.enemies_killed = 0;
end