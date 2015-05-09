--local coreInstance = CCoreLuaWrapper().m_CoreInstance

--local current_time = 0
--ai_controller = CAIController()
--ai_controller.m_Speed = 0.1
--local instance = CLuaGlobalsWrapper().m_CoreInstance;

--local wp_manager = core:get_wp_manager()
--local id_next_wp
--local id_destino_wp
--local id_path_next_wp

--local enemy = CEasyEnemy("Enemy4", "MikMikTest001", Vect3f(-3.0,2.0,10.0), 0.1, 2.5, 13, 0.0)	
--local enemy_manager = core:get_enemy_manager():insert_enemy(enemy)
--local wp1 = Vect3f(-3.0,2.0,15.0) 
--local wp2 = Vect3f(-3.0,2.0,-15.0)
--local currentwp = Vect3f(0.0,0.0,0.0)

--[[function set_enemy(_enemy)
	enemy = _enemy
end
--]]


function check_attack (_enemy)
	local player_position = coreInstance:get_player_controller():get_position()
	--core:trace(tostring(player_position.x));
		
	local player_distance = get_distance_to_player(_enemy:get_position(), player_position)
	--coreInstance:trace(tostring(player_distance))
	if player_distance <= 49 then
		coreInstance:trace("Attack!!")
		coreInstance:trace(_enemy:get_name())
		return true
	end
	return false
end

function move_enemy(ElapsedTime, _point, Enemy)
	local player =  coreInstance:get_player_controller()
	if player.m_is3D == true then 
		Enemy:move_to(ElapsedTime, _point)
	else
		Enemy:rotate_or_move(ElapsedTime, _point)
	end
end

function check_hitbox(ElapsedTime, player_position, enemy)
	
end

function get_distance_to_player(current_position, _player_position)
	-- calcular distancia hacia player
	local distance = 0
	distance = ((_player_position.x - current_position.x) ^2 + (_player_position.y - current_position.y) ^2 + (_player_position.z - current_position.z) ^2)
	return distance
end