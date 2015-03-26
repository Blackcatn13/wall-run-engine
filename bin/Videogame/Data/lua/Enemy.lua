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

local enemy = CEasyEnemy("Enemy4", "MikMikTest001", Vect3f(-3.0,2.0,10.0), 0.5, 2.0, 13, 0.0)	
local wp1 = Vect3f(-3.0,2.0,15.0) 
local wp2 = Vect3f(-3.0,2.0,-15.0)
local currentwp = wp1
local instance = CLuaGlobalsWrapper().m_CoreInstance;
--[[function set_enemy(_enemy)
	enemy = _enemy
end
--]]
function enemy_enter_stopped()
	currentwp = wp1
	return 0
end

function enemy_exit_stopped()
	current_time = 0
end

function enemy_update_stopped(ElapsedTime, doComprobation)
	if enemy ~= nil then
		if current_time >= 0.1 then
			instance.m_string = "Andar_WP"
		end
		current_time = current_time +1 * ElapsedTime
	else
		instance.m_string = "Parado"
	end
end


function enemy_enter_moving()

end

function enemy_exit_moving()
	instance.m_string = "Buscar_next_WP"
	if enemy ~= nil then
		current_time = 0
	else
		instance.m_string = "Parado"
	end
end

function enemy_update_moving(ElapsedTime, doComprobation)
	if enemy ~= nil then
		if currentwp == nil then
			currentwp = wp1
		end
		
		enemy:move_to(ElapsedTime, currentwp)
		
		--local player_position = core:get_player_controller():get_position()
		--core:trace(tostring(player_position.x));
		
		local wp_distance = get_distance_between_points(enemy:get_position(), currentwp)
		if wp_distance < 16 then
			instance.m_string = "Buscar_next_WP"
		end
	else
		instance.m_string = "Parado"
	end
		
end

function enemy_enter_calcwp()
	if enemy ~= nil then
		if currentwp.z == wp2.z then
			currentwp = wp
		else
			currentwp = wp2
		end
		instance.m_string = "Andar_WP"
	else
		instance.m_string = "Parado"
	end
end

function enemy_exit_calcwp()
--core:trace("Saliendo Buscar_next_WP");
-- OnExit Buscar_next_WP
	if enemy ~= nil then
		current_time = 0
	else
		instance.m_string = "Parado"
	end
end

function enemy_update_calcwp(ElapsedTime, doComprobation)
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


