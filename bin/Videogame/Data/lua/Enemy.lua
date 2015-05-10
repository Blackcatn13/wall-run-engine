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
		--Enemy:move_to(ElapsedTime, _point)
		move_to(Enemy, ElapsedTime, _point)
	else
		--Enemy:rotate_or_move(ElapsedTime, _point)
		rotate_or_move(Enemy, ElapsedTime, _point)
	end
end


function move_to(_enemy, ElapsedTime, _point)
	if _point:distance(_enemy:get_position()) >= 2 then
		rotate_yaw (_enemy, ElapsedTime, _point)
	
		_enemy.m_PhysicController:move(Vect3f(1, 0, 0):rotate_y(_enemy:get_yaw()) * _enemy.m_Speed, ElapsedTime)
		move_enemy_renderable(_enemy)	
	end
end


function rotate_or_move(_enemy, ElapsedTime, _point)
	if _point:distance(_enemy:get_position()) >= 2 then
		local direction = Vect3f(_point - _enemy:get_position())
		direction = direction:normalize(1.0)
		local diff = Vect3f(1.0,0.0,0.0):rotate_y(_enemy:get_yaw())
		local angle = _enemy:get_angle_diff(direction, diff)
		if angle > 0.5 then
			rotate_yaw(_enemy, ElapsedTime, _point)
		else
			_enemy.m_PhysicController:move(direction * _enemy.m_Speed, ElapsedTime)
			move_enemy_renderable(_enemy)
		end
	end
end

function move_enemy_renderable(_enemy)
	local l_position = _enemy.m_PhysicController:get_position()
	_enemy:set_position(l_position)
	if _enemy.m_RenderableObject ~= nil then
		_enemy.m_RenderableObject:set_position(l_position)
	end		
end

function rotate_yaw(_enemy, ElapsedTime, _point)
	local direction = Vect3f(_point - _enemy:get_position())
	direction.y = 0
	direction = direction:normalize(1.0)
	local diff = Vect3f(1.0,0.0,0.0):rotate_y(_enemy:get_yaw())
	local angle = _enemy:get_angle_diff(direction, diff)
	rotate_renderable(ElapsedTime, angle, _enemy)
end

function rotate_renderable(ElapsedTime, angle, _enemy)
	_enemy:set_yaw(_enemy:get_yaw() - angle * _enemy.m_TurnSpeed * ElapsedTime)

	if _enemy.m_RenderableObject ~= nil then
		_enemy.m_RenderableObject:set_yaw(_enemy:get_yaw())
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