local coreInstance = CCoreLuaWrapper().m_CoreInstance

local current_time = 0
--ai_controller = CAIController()
--ai_controller.m_Speed = 0.1
instance = CLuaGlobalsWrapper().m_CoreInstance;

core = CCoreLuaWrapper().m_CoreInstance;
--local wp_manager = core:get_wp_manager()
--local id_next_wp
--local id_destino_wp
--local id_path_next_wp

--local enemy = CEasyEnemy("Enemy4", "MikMikTest001", Vect3f(-3.0,2.0,10.0), 0.1, 2.5, 13, 0.0)	
--local enemy_manager = core:get_enemy_manager():insert_enemy(enemy)
--local wp1 = Vect3f(-3.0,2.0,15.0) 
--local wp2 = Vect3f(-3.0,2.0,-15.0)
--local currentwp = Vect3f(0.0,0.0,0.0)
local instance = CLuaGlobalsWrapper().m_CoreInstance;
--[[function set_enemy(_enemy)
	enemy = _enemy
end
--]]
function enemy_enter_stopped(name)
	--local enemy = coreInstance:get_enemy_manager():get_enemy(name)
	--currentwp = wp1
	return 0
end

function enemy_exit_stopped(name)

	current_time = 0
end

function enemy_update_stopped(ElapsedTime, doComprobation, name)
	--local enemy = coreInstance:get_renderable_object_layer_manager():get_renderable_objects_manager_by_str("enemies"):get_resource(name)
	local enemy = coreInstance:get_enemy_manager():get_enemy(name)
	if enemy ~= nil then
		if current_time >= 0.1 then
			instance.m_string = "Andar_WP"
		end
		current_time = current_time +1 * ElapsedTime
	else
		instance.m_string = "Parado"
	end
end


function enemy_enter_moving(name)
	return 0;
end

function enemy_exit_moving(name)
	--local enemy = coreInstance:get_renderable_object_layer_manager():get_renderable_objects_manager_by_str("enemies"):get_resource(name)
	local enemy = coreInstance:get_enemy_manager():get_enemy(name)
	instance.m_string = "Buscar_next_WP"
	if enemy ~= nil then
		current_time = 0
	else
		instance.m_string = "Parado"
	end
end

function enemy_update_moving(ElapsedTime, doComprobation, name)

	--local enemy = coreInstance:get_renderable_object_layer_manager():get_renderable_objects_manager_by_str("enemies"):get_resource(name)
	local enemy = coreInstance:get_enemy_manager():get_enemy(name)
	if enemy ~= nil then
		if enemy.m_CurrentWp == nil then
		--	coreInstance:trace("I have a breakpoint")
			enemy.m_CurrentWp = enemy:get_next_wp()
		end
		
	--	coreInstance:trace(tostring(currentwp.x))
		enemy:move_to(ElapsedTime, enemy.m_CurrentWp)
	--		coreInstance:trace("Am I moving??")
		--local player_position = core:get_player_controller():get_position()
		--core:trace(tostring(player_position.x));
		
		local wp_distance = get_distance_between_points(enemy:get_position(), enemy.m_CurrentWp)
		if wp_distance < 16 then
			instance.m_string = "Buscar_next_WP"
		end
	else
		instance.m_string = "Parado"
	end
		
end

function enemy_enter_calcwp(name)
	
	--local enemy = coreInstance:get_renderable_object_layer_manager():get_renderable_objects_manager_by_str("enemies"):get_resource(name)
	local enemy = coreInstance:get_enemy_manager():get_enemy(name)
	
	if enemy ~= nil then
		--[[if currentwp.z == wp2.z then
			currentwp = wp
		else
			currentwp = wp2
		end--]]
		enemy.m_CurrentWp = enemy:get_next_wp()
		instance.m_string = "Andar_WP"
	else
		instance.m_string = "Parado"
	end
end

function enemy_exit_calcwp(name)
--core:trace("Saliendo Buscar_next_WP");
-- OnExit Buscar_next_WP
	--local enemy = coreInstance:get_renderable_object_layer_manager():get_renderable_objects_manager_by_str("enemies"):get_resource(name)
	local enemy = coreInstance:get_enemy_manager():get_enemy(name)
	if enemy ~= nil then
		current_time = 0
	else
		instance.m_string = "Parado"
	end
end

function enemy_update_calcwp(ElapsedTime, doComprobation, name)
	--local enemy = coreInstance:get_renderable_object_layer_manager():get_renderable_objects_manager_by_str("enemies"):get_resource(name)
	local enemy = coreInstance:get_enemy_manager():get_enemy(name)
	if enemy ~= nil then
		--[[if enemy.wp.z == 15 then
			enemy.wp = Vect3f(2.0,2.0,-15.0)
		else
			enemy.wp = Vect3f(2.0,2.0,15.0)
		end--]]
		instance.m_string = "Andar_WP"
	else
		instance.m_string = "Parado"
	end
end


